/*
 * FigureDataLoader.cpp
 *
 *  Created on: 2012/12/26
 */

#include    "jc/graphics/figure/data/FigureDataLoader.h"

namespace jc {

FigureDataLoader::FigureDataLoader() {

}

FigureDataLoader::~FigureDataLoader() {

}

/**
 * 読み込みを行う。
 * ファイル名等の初期化は継承したサブクラスで行う。
 */
void FigureDataLoader::load() {

    // 最初のノードを読み込む
    loadNode(0);
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
        MBinaryInputStream stream = openNode(nodeNumber);

        node.name = stream->readString();
        node.number = stream->readU16();
        node.type = (NodeType_e) stream->readU16();

        node.children_num = stream->readU16();

        if (node.children_num) {
            node.children.reset(new s32[node.children_num]);

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
            node.def_transform.rotate.order = (EulerRotateOrder_e) stream->readS8();
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
        MBinaryInputStream stream = openMeshInfo(nodeNumber);
        mesh.material_num = stream->readS8();
        mesh.context_num.reset(new u32[mesh.material_num]);
        for (u32 i = 0; i < mesh.material_num; ++i) {
            mesh.context_num[i] = stream->readS8();
        }

        onMeshLoadComplete(node, mesh, &request);
    }

    // 子も読み込む
    for (u32 i = 0; i < node.children_num; ++i) {
        loadNode(node.children[i]);
    }
}

}
