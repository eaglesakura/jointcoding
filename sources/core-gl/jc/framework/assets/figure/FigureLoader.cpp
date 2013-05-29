/*
 * FigureLoader.cpp
 *
 *  Created on: 2013/05/20
 */

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

FigureLoader::FigureLoader(MDevice device, MFigureDataFactory factory) :
        FigureDataLoader(factory) {
    assert(device);
    assert(factory);

    this->device = device;
}

FigureLoader::~FigureLoader() {

}

/**
 * 頂点バッファキャッシュを生成する
 * キャッシュは一括してVertexBufferへ転送する
 */
void FigureLoader::createVerticesCache(const s32 vertices_num) {
    cacheBasicVertices.reserve(vertices_num);
    cacheSkinningVertices.reserve(vertices_num);
}
/**
 * フィギュアの基本情報を読み込んだ後に呼び出される
 * サブクラスではノード数分のメモリ確保等を行う。
 */
void FigureLoader::onFigureInfoLoadComplete(const FigureInfo &figureInfo) {
    jclogf("onFigureInfoLoadComplete nodes(%d) vert(%d) idx(%d) mtl(%d)", figureInfo.node_num, figureInfo.vertex_num, figureInfo.index_num, figureInfo.material_num);

    // 指定されたノード数を確保する
    loadTarget->initializeNodes(device, figureInfo.node_num);
}

/**
 * ノード情報を読み込んだ場合に呼び出される
 */
void FigureLoader::onNodeLoadComplete(const jc::FigureDataLoader::NodeInfo &nodeInfo) {
    jclogf("  onNodeLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());

    FigureNode *pNode = loadTarget->getNode(nodeInfo.number);
    assert(pNode);

    // ノードに名前を指定する
    pNode->setName(nodeInfo.name);

    // ノードの階層情報を設定する
    {
        std::vector<FigureNode*> nodes;
        for (s32 i = 0; i < nodeInfo.children_num; ++i) {
            nodes.push_back(loadTarget->getNode(nodeInfo.children[i]));
        }

        jclogf("    children(%d)", nodes.size());
        pNode->setChildrenNum(&nodes[0], nodes.size());
    }

    // ノードのデフォルト行列と逆行列を設定する
    {
        Transform trans;
        trans.scale = nodeInfo.def_transform.translate;
        trans.rotate.set(nodeInfo.def_transform.rotate.x, nodeInfo.def_transform.rotate.y, nodeInfo.def_transform.rotate.z, nodeInfo.def_transform.rotate.w);
        trans.translate = nodeInfo.def_transform.translate;

        // デフォルト行列の計算
        trans.getMatrix(pNode->getDefaultTransform());

        // 逆行列の計算
        pNode->calcInvertTransform();
    }
}

/**
 * onNodeLoadComplete()完了後、メッシュ情報を読み込んだ後に呼び出される
 *
 * @param load_request 読み込みを行うデータのリクエストを行う
 */
void FigureLoader::onMeshInfoLoadComplete(const jc::FigureDataLoader::NodeInfo &nodeInfo, const jc::FigureDataLoader::MeshInfo &meshInfo, jc::FigureDataLoader::MeshDataRequest *load_request) {
    jclogf("  onMeshInfoLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());

    FigureNode *pNode = loadTarget->getNode(nodeInfo.number);
    assert(pNode);

    jclogf("    fragments(%d)", meshInfo.material_num);
    // マテリアル数を確保する
    pNode->initialize(device, meshInfo.material_num);
    // 各フラグメントごとのコンテキストを生成する
    for (s32 i = 0; i < meshInfo.material_num; ++i) {
        jclogf("      context(%d)", meshInfo.context_num[i]);

        // 指定コンテキスト数を確保する
        MeshGroup *group = pNode->getMeshGroup(i);
        group->initializeFragments(device, meshInfo.context_num[i]);
    }

    // 各種拡張データのリクエストも行う
    load_request->bone_weight = jctrue;
    load_request->coords = jctrue;
    load_request->normals = jctrue;
}

/**
 * メッシュデータの読み込みを完了した
 *
 * @param material_num マテリアル番号
 * @param context_num コンテキスト番号
 * @param dataType 読み込みを行ったデータタイプ
 * @param loaded_data 読み込みを行ったデータ
 */
void FigureLoader::onMeshDataLoadComplete(const FigureDataLoader::NodeInfo &nodeInfo, const FigureDataLoader::MeshInfo &meshInfo, const s32 material_num, const s32 context_num, const FigureDataLoader::MeshData &loaded) {
    jclogf("  onMeshDataLoadComplete node(%d) name(%s) type(%d)", nodeInfo.children_num, nodeInfo.name.c_str(), loaded.type);

    FigureNode *pNode = loadTarget->getNode(nodeInfo.number);
    assert(pNode);

    MeshGroup *fragment = pNode->getMeshGroup(material_num);
    MeshFragment *context = fragment->getFragment(context_num);
    // データによって処理を分ける
    switch (loaded.type) {
        case MeshDataType_Positions: {
            // 頂点キャッシュを生成する
            createVerticesCache(loaded.data_length);

            Vector3f *positions = (Vector3f*) loaded.data.get();

            // データコピー
            for (s32 i = 0; i < loaded.data_length; ++i) {
                cacheBasicVertices[i].position = positions[i];
            }
        }
            break;
        case MeshDataType_Coords: {

            Vector2f *coords = (Vector2f*) loaded.data.get();

            // データコピー
            for (s32 i = 0; i < loaded.data_length; ++i) {
                cacheBasicVertices[i].coord = coords[i];
            }

        }
            break;
        case MeshDataType_Normals: {
            Vector3f *normals = (Vector3f*) loaded.data.get();

            // データコピー
            for (s32 i = 0; i < loaded.data_length; ++i) {
                cacheBasicVertices[i].normal = normals[i];
            }
        }
            break;
        case MeshDataType_BoneIndices: {

        }
            break;
        case MeshDataType_BoneWeight: {

        }
            break;
        case MeshDataType_BonePickTables: {

        }
            break;
        case MeshDataType_Indices: {
            // インデックスバッファはキャッシュせずに直接転送する
            u16 *indices = (u16*) loaded.data.get();
            const s32 length = loaded.data_length;

            IndexBufferObject *ibo = context->getIndicesBuffer();
            ibo->bind(device->getState());
            {
                ibo->bufferData(indices, length, GL_STATIC_DRAW);
            }
            ibo->unbind(device->getState());
        }
            break;
    }
}

/**
 * 指定したコンテキストが全て読み込み終わった。
 * サブクラスではVBO転送を期待する。
 */
void FigureLoader::onMeshMaterialContextLoadComplete(const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num) {
    jclogf("  onMeshMaterialContextLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());

    FigureNode *pNode = loadTarget->getNode(nodeInfo.number);
    assert(pNode);

    MeshGroup *pGroup = pNode->getMeshGroup(material_num);
    MeshFragment *pFragment = pGroup->getFragment(context_num);

    // 基本データをバッファへ転送する
    {
        VertexBufferObject<void> *basic_vbo = pFragment->getVertexBuffer();
        basic_vbo->bind(device->getState());
        {
            basic_vbo->bufferData((void*) cacheBasicVertices.ptr, sizeof(BasicVertex), cacheBasicVertices.length, GL_STATIC_DRAW);
        }
        basic_vbo->unbind(device->getState());
    }
}

/**
 * 読み込むべきフィギュアデータの読み込みが完了した
 */
void FigureLoader::onLoadCompleted() {
    jclog("  onLoadCompleted");
}

}
}
