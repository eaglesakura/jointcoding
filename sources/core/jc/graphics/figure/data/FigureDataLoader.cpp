/*
 * FigureDataLoader.cpp
 *
 *  Created on: 2012/12/26
 */

#include    "jc/graphics/figure/data/FigureDataLoader.h"

namespace jc {

FigureDataLoader::FigureDataLoader(MFigureDataFactory factory) {
    this->factory = factory;
}

FigureDataLoader::~FigureDataLoader() {

}

/**
 * 読み込みを行う。
 * ファイル名等の初期化は継承したサブクラスで行う。
 */
void FigureDataLoader::load() {

    // load figure info
    {
        MBinaryInputStream stream = factory->openFigureInfo();
        FigureInfo figure;
        figure.node_num = stream->readU32();

        onFigureInfoLoadComplete(figure);
    }

    // 最初のノードを読み込む
    loadNode(0);

    onLoadCompleted();
}
/**
 * 指定した番号のノードを読み込む
 */
void FigureDataLoader::loadNode(const s32 nodeNumber) {

    NodeInfo node;
    MeshInfo mesh;
    MeshDataRequest request;

    // ノード情報を読み込む
    {
        MBinaryInputStream stream = factory->openNode(nodeNumber);

        node.name = stream->readString();
        node.number = stream->readU16();
        node.type = (NodeType_e) stream->readU16();

        node.children_num = stream->readU16();

        if (node.children_num) {
            node.children.reset(mark_new s32[node.children_num]);

            // 子ノードの番号を取り出す
            for (u32 i = 0; i < node.children_num; ++i) {
                node.children[i] = stream->readU16();
            }
        }

        // transform
        {
            // scale
            node.def_transform.scale.x = stream->readFixed32();
            node.def_transform.scale.y = stream->readFixed32();
            node.def_transform.scale.z = stream->readFixed32();

            // rotate
            node.def_transform.rotate.order = (TransformRotateOrder_e) stream->readS8();
            node.def_transform.rotate.x = stream->readFixed32();
            node.def_transform.rotate.y = stream->readFixed32();
            node.def_transform.rotate.z = stream->readFixed32();
            node.def_transform.rotate.w = stream->readFixed32();

            // translate
            node.def_transform.translate.x = stream->readFixed32();
            node.def_transform.translate.y = stream->readFixed32();
            node.def_transform.translate.z = stream->readFixed32();
        }

        onNodeLoadComplete(node);
    }

    // メッシュ情報を読み込む
    if (node.type == NodeType_Mesh) {

        // load main
        {
            MBinaryInputStream stream = factory->openMeshInfo(nodeNumber);
            mesh.material_num = stream->readS8();

            mesh.context_num.reset(mark_new u32[mesh.material_num]);
            for (u32 i = 0; i < mesh.material_num; ++i) {
                mesh.context_num[i] = stream->readS8();
            }
        }

        // load material
        for (u32 i = 0; i < mesh.material_num; ++i) {
            MBinaryInputStream stream = factory->openMaterialInfo(nodeNumber, i);
            FigureDataLoader::MaterialInfo material;

            material.name = stream->readString();
            material.texture_name = stream->readString();

            {
                material.diffuse.tag.r = stream->readU8();
                material.diffuse.tag.g = stream->readU8();
                material.diffuse.tag.b = stream->readU8();
                material.diffuse.tag.a = stream->readU8();
            }
            {
                material.ambient.tag.r = stream->readU8();
                material.ambient.tag.g = stream->readU8();
                material.ambient.tag.b = stream->readU8();
                material.ambient.tag.a = stream->readU8();
            }
            {
                material.emissive.tag.r = stream->readU8();
                material.emissive.tag.g = stream->readU8();
                material.emissive.tag.b = stream->readU8();
                material.emissive.tag.a = stream->readU8();
            }

            mesh.materials.push_back(material);
        }

        onMeshInfoLoadComplete(node, mesh, &request);

        // マテリアル数×コンテキスト数だけデータを読み込む
        for (u32 i = 0; i < mesh.material_num; ++i) {
            for (u32 k = 0; k < mesh.context_num[i]; ++k) {
                // indices
                {
                    loadMeshData(node, mesh, i, k, MeshDataType_Indices);
                }

                // vertices
                {
                    loadMeshData(node, mesh, i, k, MeshDataType_Positions);
                }

                // coords
                if (request.coords) {
                    loadMeshData(node, mesh, i, k, MeshDataType_Coords);
                }

                // normals
                if (request.normals) {
                    loadMeshData(node, mesh, i, k, MeshDataType_Normals);
                }

                // bone weight
                if (request.bone_weight) {
                    loadMeshData(node, mesh, i, k, MeshDataType_BoneWeight);
                    loadMeshData(node, mesh, i, k, MeshDataType_BoneIndices);
                    loadMeshData(node, mesh, i, k, MeshDataType_BonePickTables);
                }

                // コンテキストの全情報を読み込み終わった
                // サブクラスでVBO転送を期待する
                onMeshMaterialContextLoadComplete(node, mesh, i, k);
            }

            // 1マテリアルの情報を読み終わった
            onMeshMaterialLoadComplete(node, mesh, i);
        }

        // メッシュ全体の読み込みが完了した
        onMeshLoadComplete(node, mesh);
    }

    // 子も読み込む
    for (u32 i = 0; i < node.children_num; ++i) {
        loadNode(node.children[i]);
    }
}
/**
 *
 */
void FigureDataLoader::loadMeshData(const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num, const MeshDataType_e type) {
    MBinaryInputStream stream = factory->openMeshData(type, nodeInfo.number, material_num, context_num);
    MeshData data;

    jclogf("loadMeshData(%d-%d)", material_num, context_num);

    data.type = type;
    switch (type) {
        case MeshDataType_Indices: {
            data.data_length = stream->readU16();

            u16 *ptr = (u16*) malloc(data.data_length * sizeof(u16));
            data.data.reset((void*) ptr, free);

            for (u32 i = 0; i < data.data_length; ++i) {
                ptr[i] = stream->readU16();
            }
        }
            break;
        case MeshDataType_Positions: {
            data.data_length = stream->readU16();

            Vector3f *ptr = (Vector3f*) malloc(data.data_length * sizeof(Vector3f));
            data.data.reset((void*) ptr, free);

            for (u32 i = 0; i < data.data_length; ++i) {
                ptr[i].x = stream->readFixed32();
                ptr[i].y = stream->readFixed32();
                ptr[i].z = stream->readFixed32();
            }

        }
            break;
        case MeshDataType_Coords: {
            data.data_length = stream->readU16();

            Vector2f *ptr = (Vector2f*) malloc(data.data_length * sizeof(Vector2f));
            data.data.reset((void*) ptr, free);

            for (u32 i = 0; i < data.data_length; ++i) {
                ptr[i].x = stream->readFixed32();
                ptr[i].y = stream->readFixed32();
            }
        }

            break;
        case MeshDataType_Normals: {
            data.data_length = stream->readU16();

            Vector3f *ptr = (Vector3f*) malloc(data.data_length * sizeof(Vector3f));
            data.data.reset((void*) ptr, free);

            for (u32 i = 0; i < data.data_length; ++i) {
                ptr[i].x = stream->readFixed32();
                ptr[i].y = stream->readFixed32();
                ptr[i].z = stream->readFixed32();
            }
        }
            break;
        case MeshDataType_BoneIndices: {
            data.data_length = stream->readU16();

            // １頂点に含まれるbone weightの数
            const int weight_num = stream->readU8();

            u8 *ptr = (u8*) malloc(data.data_length * weight_num);
            data.data.reset((void*) ptr, free);

            for (u32 i = 0; i < (data.data_length * weight_num); ++i) {
                ptr[i] = stream->readU8();
            }
        }
            break;

        case MeshDataType_BoneWeight: {
            data.data_length = stream->readU16();

            // １頂点に含まれるbone weightの数
            const int weight_num = stream->readU8();

            float *ptr = (float*) malloc(data.data_length * weight_num * sizeof(float));
            data.data.reset((void*) ptr, free);

            for (u32 i = 0; i < (data.data_length); ++i) {
                float weight_sum = 0;
                for (int k = 0; k < weight_num; ++k) {
                    ptr[i * weight_num + k] = stream->readFixed32();

                    // 合計値を計算する
                    if (k) {
                        weight_sum += ptr[i * weight_num + k];
                    }
                }

                // index0のウェイト＝最も重みが強いウェイトに、ウェイトの誤差が集中するようにする
                ptr[i * weight_num + 0] = 1.0f - weight_sum;
            }
        }
            break;
        case MeshDataType_BonePickTables: {
            data.data_length = stream->readU16();

            u16 *ptr = (u16*) malloc(data.data_length * sizeof(u16));
            data.data.reset((void*) ptr, free);

            for (u32 i = 0; i < (data.data_length); ++i) {
                ptr[i] = stream->readU16();
            }
        }

            break;

        default:
            throw create_exception(UnsupportedOperationException, "data type not support...");
    }

    onMeshDataLoadComplete(nodeInfo, meshInfo, material_num, context_num, data);
}

}
