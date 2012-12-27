/*
 * Mesh.cpp
 *
 *  Created on: 2012/12/17
 */

#include "jointcoding-fbx.h"
#include <vector>
#include "jc/graphics/figure/mesh/BoneWeight.h"
#include "jcfbx/node/Mesh.h"
#include "jc/math/Math.h"
#include "jcfbx/attribute/VertexContainer.h"
#include "jcfbx/attribute/IndicesContainer.h"
#include "prv_FbxDeformer.h"
#include    "jcfbx/convert/MeshFragmentConverter.h"

#include    "jc/io/FileOutputStream.h"
#include    "jc/data/BinaryOutputStream.h"

namespace jc {
namespace fbx {

// vertices
void createCoords(std::vector<Vector2f> *result, KFbxMesh *mesh);
void createPositions(std::vector<Vector3f> *result, KFbxMesh *mesh);
void createNormals(std::vector<Vector3f> *result, KFbxMesh *mesh);
void createWeights(std::vector<SimpleBoneWeight> *result, const std::vector<Vector3f> &positions, KFbxMesh *mesh);

// polygons
void createMaterials(std::vector<MFigureMaterial> *result, KFbxMesh *mesh);
void createPolygonList(std::vector<ConvertedPolygon> *result, KFbxMesh *mesh);

Mesh::Mesh(KFbxNode *meshNode, s32 nodeNumber) :
        Node(meshNode, nodeNumber) {
    jclogf("Mesh(NodeNumber %d)", nodeNumber);

    if (!meshNode->GetMesh()) {
        throw create_exception_t(FbxException, FbxException_NoMesh);
    }

    // まずは頂点情報をまとめあげる
    {
        KFbxMesh *mesh = meshNode->GetMesh();

        // 頂点情報
        VertexContainer vertices;
        // ポリゴン情報
        IndicesContainer indices;

        // 頂点情報を整理する
        {
            createPositions(&vertices.positions, mesh);
            createCoords((std::vector<Vector2f>*) &vertices.coords, mesh);
            createNormals(&vertices.normals, mesh);
            createWeights(&vertices.weights, vertices.positions, mesh);
        }
        // ポリゴン情報を整理する
        {
            createMaterials(&indices.materials, mesh);
            createPolygonList(&indices.polygons, mesh);
        }

        jclogf("pos(%d) uv(%d) normal(%d) weight(%d) tri-poly(%d) mat(%d)", vertices.positions.size(), vertices.coords.size(), vertices.normals.size(), vertices.weights.size(), indices.polygons.size(), indices.materials.size());

        // 実利用可能なように、メッシュ構造を最適化して完了
        MeshFragmentConverter::convertMesh(&fragments, vertices, indices);
    }

}

Mesh::~Mesh() {

}

/**
 * 出力を行う
 */
void Mesh::serialize(FbxExportManager *exportManager) {

    // メッシュ情報を出力
    {
        MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("node%03d.meshinfo", getNodeNumber()));

        stream->writeS8(getFragmentCount()); // マテリアル数

        // マテリアルごとのコンテキスト数
        for (int i = 0; i < getFragmentCount(); ++i) {
            MFigureMeshFragment fragment = fragments[i];

            stream->writeS8(fragment->getDrawingContextCount());
        }
    }

    // メッシュ情報を出力
    for (int i = 0; i < getFragmentCount(); ++i) {
        MFigureMeshFragment fragment = fragments[i];

        // material
        {
            const String name = String::format("node%03d_mtl%02d.material", getNodeNumber(), i);
            MFigureDataOutputStream stream = exportManager->createOutputStream(this, name);

            // 出力対象のマテリアル
            MFigureMaterial material = fragment->getMaterial();

            stream->writeString(material->name);
            stream->writeString(material->textureName);

            // diffuse
            {
                stream->writeU8(material->diffuse.r());
                stream->writeU8(material->diffuse.g());
                stream->writeU8(material->diffuse.b());
                stream->writeU8(material->diffuse.a());
            }
            // ambient
            {
                stream->writeU8(material->ambient.r());
                stream->writeU8(material->ambient.g());
                stream->writeU8(material->ambient.b());
                stream->writeU8(material->ambient.a());
            }
            // emissive
            {
                stream->writeU8(material->emissive.r());
                stream->writeU8(material->emissive.g());
                stream->writeU8(material->emissive.b());
                stream->writeU8(material->emissive.a());
            }
        }

        for (int ctx_index = 0; ctx_index < fragment->getDrawingContextCount(); ++ctx_index) {
            FigureMeshFragment::DrawingContext *pContext = fragment->getDrawingContext(ctx_index).get();

            const String name = String::format("node%03d_mtl%02d_ctx%02d", getNodeNumber(), i, ctx_index);
            // 位置リスト
            {
                MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("%s.vertices", name.c_str()));

                // まずは位置情報だけを書いてちゃんと吐かれているか確認する
                stream->writeU16(pContext->vertices_length);

                // 頂点リストを書き出す
                stream->writeVertices((const void*) pContext->vertices.get(), pContext->vertices_length, sizeof(FigureVertex));
            }

            // 法線リスト
            {
                MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("%s.normals", name.c_str()));
                // まずは位置情報だけを書いてちゃんと吐かれているか確認する
                stream->writeU16(pContext->vertices_length);

                // 頂点リストを書き出す
                stream->writeVertices((const void*) (((u8*) pContext->vertices.get()) + sizeof(Vector3f)), pContext->vertices_length, sizeof(FigureVertex));
            }

            // UVリスト
            {
                MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("%s.coords", name.c_str()));

                // まずは位置情報だけを書いてちゃんと吐かれているか確認する
                stream->writeU16(pContext->vertices_length);

                // 頂点リストを書き出す
                for (u32 coord_index = 0; coord_index < pContext->vertices_length; ++coord_index) {
                    stream->writeCoord(pContext->vertices[coord_index].uv);
                }
            }

            // ボーン関連
            {
                // ボーンインデックス
                {
                    MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("%s.weightindices", name.c_str()));

                    stream->writeU16(pContext->vertices_length);

                    // 1頂点に含まれるウェイト数
                    stream->writeU8(SIMPLE_BONE_NUM);

                    // ウェイトインデックスリストを書き出す
                    for (u32 vert_index = 0; vert_index < pContext->vertices_length; ++vert_index) {
                        for (int weight_index = 0; weight_index < SIMPLE_BONE_NUM; ++weight_index) {
                            const u8 index = pContext->vertices[vert_index].weight.indices[weight_index];
                            jclogf("  weight index(%d)", index);
                            stream->writeU8(index);
                        }
                    }
                }
                // ボーンウェイト
                {
                    MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("%s.weight", name.c_str()));

                    stream->writeU16(pContext->vertices_length);
                    // 1頂点に含まれるウェイト数
                    stream->writeU8(SIMPLE_BONE_NUM);

                    // ウェイトインデックスリストを書き出す
                    for (u32 vert_index = 0; vert_index < pContext->vertices_length; ++vert_index) {
                        for (int weight_index = 0; weight_index < SIMPLE_BONE_NUM; ++weight_index) {
                            const float weight = pContext->vertices[vert_index].weight.weights[weight_index];

                            jclogf("  weight(%f)", weight);
                            stream->writeFixed32(weight);
                        }
                    }
                }
                // ボーンのピックアップテーブル
                {
                    MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("%s.bonepicktable", name.c_str()));

                    stream->writeU16(pContext->bone_pick_table_length);

                    // ウェイトインデックスリストを書き出す
                    for (u32 pick_index = 0; pick_index < pContext->bone_pick_table_length; ++pick_index) {
                        stream->writeU8(pContext->bone_pick_table[pick_index]);
                    }
                }
            }

            // インデックスバッファ
            {
                MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("%s.indices", name.c_str()));
                stream->writeU16(pContext->indices_length);

                for (u32 index = 0; index < pContext->indices_length; ++index) {
                    stream->writeU16(pContext->indices[index]);
                }
            }
        }
    }

    // ベース情報を出力
    Node::serialize(exportManager);
}

MMesh Mesh::createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager) {

    MMesh result(new Mesh(node, importManager->nextNodeId()));

    return result;
}

}
}
