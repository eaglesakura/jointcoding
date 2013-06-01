/*
 * FigureLoader.cpp
 *
 *  Created on: 2013/05/20
 */

#include    "jc/framework/assets/FigureAsset.hpp"
#include    "jc/framework/assets/figure/FigureLoader.h"

namespace jc {
namespace fw {

FigureLoader::FigureLoader(MDevice device, MFigureDataFactory factory, MTextureFactory texFactory) :
        FigureDataLoader(factory) {
    assert(device);
    assert(factory);

    this->device = device;
    this->textureFactory = texFactory;
}

FigureLoader::~FigureLoader() {

}

/**
 * 頂点バッファキャッシュを生成する
 * キャッシュは一括してVertexBufferへ転送する
 */
void FigureLoader::createCacheBuffer(const s32 vertices_num, const s32 indices_num) {
    buffer.vertices.alloc(vertices_num);
    buffer.skinnings.alloc(vertices_num);
    buffer.indices.alloc(indices_num);
}
/**
 * フィギュアの基本情報を読み込んだ後に呼び出される
 * サブクラスではノード数分のメモリ確保等を行う。
 */
void FigureLoader::onFigureInfoLoadComplete(const FigureInfo &figureInfo) {
    jclogf("onFigureInfoLoadComplete nodes(%d) vert(%d) idx(%d) mtl(%d)", figureInfo.node_num, figureInfo.vertex_num, figureInfo.index_num, figureInfo.material_num);

    // 指定されたノード数を確保する
    figure->initialize(device, figureInfo.node_num, figureInfo.material_num);

    // 必要な頂点数・インデックス数を確保する
    createCacheBuffer(figureInfo.vertex_num, figureInfo.index_num);
}

/**
 * ノード情報を読み込んだ場合に呼び出される
 */
void FigureLoader::onNodeLoadComplete(const jc::FigureDataLoader::NodeInfo &nodeInfo) {
    jclogf("  onNodeLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());

    FigureNode *pNode = figure->getNode(nodeInfo.number);
    assert(pNode);

    // ノードに名前を指定する
    pNode->setName(nodeInfo.name);

    // ノードの階層情報を設定する
    {
        std::vector<FigureNode*> nodes;
        for (s32 i = 0; i < nodeInfo.children_num; ++i) {
            nodes.push_back(figure->getNode(nodeInfo.children[i]));
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

    FigureNode *pNode = figure->getNode(nodeInfo.number);
    assert(pNode);

    jclogf("    fragments(%d)", meshInfo.material_num);
    // グループに必要なマテリアル数を確保する
    pNode->initialize(device, meshInfo.material_num);

    // 各フラグメントごとのコンテキストを生成する
    for (s32 i = 0; i < meshInfo.material_num; ++i) {
        jclogf("      context(%d)", meshInfo.context_num[i]);

        // 指定コンテキスト数を確保する
        MeshGroup *group = pNode->getMeshGroup(i);
        group->initialize(device, meshInfo.context_num[i]);

        // マテリアルの設定を行う
        {
            const MaterialInfo &material = meshInfo.materials[i];

            MeshMaterial *pMaterial = figure->getResource()->getMaterialOrFree(material.name);
            assert(pMaterial);

            // マテリアルを関連付ける
            group->setMaterial(pMaterial);

            // テクスチャ名を保存しておく
            {
                diffuses.insert(std::map<MeshMaterial*, String>::value_type(pMaterial, material.texture_name));
            }
        }
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

    FigureNode *pNode = figure->getNode(nodeInfo.number);
    assert(pNode);

    MeshGroup *fragment = pNode->getMeshGroup(material_num);
    MeshFragment *context = fragment->getFragment(context_num);

    // 操作対象の各種ポインタを取り出す
    unsafe_array<BasicVertex> vertices = getCurrentVertices();
    unsafe_array<SkinningVertex> skinnings = getCurrentSkinnings();
    unsafe_array<u16> indices = getCurrentIndices();

    // データによって処理を分ける
    switch (loaded.type) {
        case MeshDataType_Positions: {
            // 頂点数を保存しておく
            currentFragment.vertices = loaded.data_length;

            // データコピー
            Vector3f *positions = (Vector3f*) loaded.data.get();
            for (s32 i = 0; i < loaded.data_length; ++i) {
                vertices[i].position = positions[i];
            }
        }
            break;
        case MeshDataType_Coords: {

            // データコピー
            Vector2f *coords = (Vector2f*) loaded.data.get();
            for (s32 i = 0; i < loaded.data_length; ++i) {
                vertices[i].coord = coords[i];
            }

        }
            break;
        case MeshDataType_Normals: {

            // データコピー
            Vector3f *normals = (Vector3f*) loaded.data.get();
            for (s32 i = 0; i < loaded.data_length; ++i) {
                vertices[i].normal = normals[i];
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
            // インデックス数を保存しておく
            currentFragment.indices = loaded.data_length;

            // インデックスバッファはキャッシュせずに直接転送する
            u16 *pIndices = (u16*) loaded.data.get();
            s32 length = loaded.data_length;

            // 頂点ヘッダ分だけ、インデックスをずらす必要がある
            const u16 vert_head = (u16) (buffer.vertex_header);

            // 頂点ヘッダ分だけずらしながら保存する

            u16 *pWriter = indices.ptr;
            while (length) {
                *pWriter = (vert_head + *pIndices);
                ++pWriter;
                ++pIndices;
                --length;
            }

        }
            break;
    }
}

/**
 * 指定したグループが全て読み込み終わった。
 * 次のグループを読み込む準備を行う
 */
void FigureLoader::onMeshMaterialContextLoadComplete(const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num) {
    jclogf("  onMeshMaterialContextLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());
    FigureNode *pNode = figure->getNode(nodeInfo.number);
    assert(pNode);

    MeshGroup *pGroup = pNode->getMeshGroup(material_num);
    MeshFragment *pFragment = pGroup->getFragment(context_num);

    // インデックスバッファの範囲を指定する
    pFragment->setIndicesRange(buffer.index_header, currentFragment.indices);

    // グループ読込完了を通知
    onGroupLoadComplete();
#if 0

    // 基本データをバッファへ転送する
    {
        VertexBufferObject<void> *basic_vbo = pFragment->getVertexBuffer();
        basic_vbo->bind(device->getState());
        {
            basic_vbo->bufferData((void*) cacheBasicVertices.ptr, sizeof(BasicVertex), cacheBasicVertices.length, GL_STATIC_DRAW);
        }
        basic_vbo->unbind(device->getState());
    }
#endif
}

/**
 * 読み込むべきフィギュアデータの読み込みが完了した
 * バッファへ一括コピーを行う
 */
void FigureLoader::onLoadCompleted() {
    jclog("  onLoadCompleted");

    MMeshResource resource = figure->getResource();

    FigureVertices *pVertices = resource->getVertices().get();
    IndexBufferObject *pIndices = resource->getIndices().get();

    pVertices->bind(device->getState());
    {
        pVertices->bufferData(buffer.vertices.ptr, sizeof(BasicVertex), buffer.vertices.length, GL_STATIC_DRAW);
    }
    pVertices->unbind(device->getState());

    pIndices->bind(device->getState());

    // テクスチャ読込を行う
    if (textureFactory) {
        std::map<MeshMaterial*, String>::iterator itr = diffuses.begin(), end = diffuses.end();

        while (itr != end) {

            MeshMaterial *pMaterial = itr->first;
            String fileName = itr->second;
            jclogf(" texture(%s)", fileName.c_str());

            // 実際の読込を行う
            {
                MTextureImage texture = textureFactory->load(device, fileName);
                pMaterial->setDiffuse(texture);
            }

            ++itr;
        }
    }

    {
#if 0
        // MEMO ベリファイを行う
        for (int i = 0; i < buffer.indices.length; ++i) {
            // インデックスを捕捉する
            const u16 vertex_index = buffer.indices[i];

            // 頂点を取り出す
            BasicVertex v = buffer.vertices[vertex_index];

            // 仕組み上、必ずuniqueになるはずなので、vertex_index未満のインデックスには頂点がヒットしないはずである
            for (int k = 0; k < vertex_index; ++k) {
                BasicVertex temp = buffer.vertices[k];

                if (temp.position == v.position) {
                    jclogf("hit vertex index[%d] -> temp[%d]", vertex_index, k);
                    // インデックスを書き換えてやる
                    buffer.indices[i] = k;
//                    assert(false);
                }
            }
        }
#endif

        pIndices->bufferData(buffer.indices.ptr, buffer.indices.length, GL_STATIC_DRAW);
    }
    pIndices->unbind(device->getState());
}

}
}
