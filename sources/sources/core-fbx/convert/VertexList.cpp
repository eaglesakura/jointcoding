/*
 * VertexList.cpp
 *
 *  Created on: 2012/12/18
 */
#include    "jcfbx/convert/VertexList.h"

namespace jc {
namespace fbx {

/**
 * 読み込んだFBXのraw情報をもとにして、頂点一覧を作成する
 */
FbxVertexTable::FbxVertexTable(const VertexContainer &vertices, const IndicesContainer &indices) {

    /**
     *
     * 最適化した頂点リスト
     */
    std::vector<FigureVertex> compact_vertices;

    /**
     * 最適化したインデックスリスト
     */
    std::vector<u32> compact_indices;

    // マテリアル数だけフラグメントを構築する
    for (int material_index = 0; material_index < (int) indices.materials.size(); ++material_index) {
        fragments.push_back(MMeshFragment(new MeshFragment()));
    }

    // すべての頂点を結合して、最適化する
    {
        const int polygon_num = (int) indices.polygons.size();
        for (int polygon_index = 0; polygon_index < polygon_num; ++polygon_index) {
            const ConvertedPolygon &polygon = indices.polygons[polygon_index];

            for (int k = 0; k < 3; ++k) {
                FigureVertex v;
                v.position = vertices.positions[polygon.position[k]];
                if (vertices.weights.size()) {
                    v.weight = vertices.weights[polygon.position[k]];
                }

                if (vertices.coords.size()) {
                    v.uv = vertices.coords[polygon.attributes[k]];
                }
                if (vertices.normals.size()) {
                    v.normal = vertices.normals[polygon.attributes[k]];
                }
                compact_indices.push_back(registerVertex(&compact_vertices, v));
            }
        }
    }

    // 描画命令ごとにフラグメント化する
    // 一度に描画可能なボーン数(uniform vector)には制限があるため、適度な個数で分割する
    {
        int current = 0;
        const int polygon_num = (int) indices.polygons.size();

        for (int polygon_index = 0; polygon_index < polygon_num; ++polygon_index) {
            const ConvertedPolygon &polygon = indices.polygons[polygon_index];

            // マテリアルごとに登録するFragmentを拾う
            MMeshFragment targetFragment = fragments[polygon.material];

            // ポリゴンを登録する
            targetFragment->addIndices(compact_vertices, compact_indices[current + 0], compact_indices[current + 1], compact_indices[current + 2]);

            // カレントを進める
            current += 3;
        }

    }

    jclogf("    converted vertices(%d vertex)", compact_vertices.size());
    for (int i = 0; i < (int) fragments.size(); ++i) {
        jclogf("      material[%d] fragments(%d)", i, fragments[i]->getContextCount());
        for (int k = 0; k < fragments[i]->getContextCount(); ++k) {
            jclogf("        vertices(%d) poly(%d) bone(%d)", fragments[i]->getVerticesCount(k), fragments[i]->getPolygonCount(k), fragments[i]->getBoneCount(k));
        }
    }
#if 0
    for (s32 i = 0; i < triangles.length(); ++i) {
        for (s32 k = 0; k < 3; ++k) {
            SmartPtr < FbxVertex > v = createVertex(&triangles[i], k);

            //! 頂点を登録する
            s32 index = fvt.regist(v);
            indices[triangles[i].material].push_back(index);
            ++ptr;
        }
    }

    //! 出力用の頂点バッファを作成する
    {
        outVertices = new FbxVertex[fvt.vertices.size()];
        for (s32 i = 0; i < fvt.vertices.size(); ++i) {
            outVertices[i] = *fvt.vertices[i];
        }
    }

    //! 出力用のインデックスバッファを作成する
    for (s32 i = 0; i < indices.size(); ++i) {
        s32 indexLength = indices[i].size();
        materials[i].indices = new s32[indexLength];

        for (s32 k = 0; k < indexLength; ++k) {
            materials[i].indices[k] = indices[i][k];
        }
    }
#endif
}

FbxVertexTable::~FbxVertexTable() {

}

}
}
