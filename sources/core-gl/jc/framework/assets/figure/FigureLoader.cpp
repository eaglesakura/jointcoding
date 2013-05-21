/*
 * FigureLoader.cpp
 *
 *  Created on: 2013/05/20
 */

#include    "jc/framework/assets/figure/FigureAsset.hpp"

namespace jc {
namespace fw {

FigureLoader::FigureLoader(MDevice device, MFigureDataFactory factory) :
        FigureDataLoader(factory) {
    assert(device);
    assert(factory);

    this->state = device->getState();
    this->vram = device->getVRAM();
}

FigureLoader::~FigureLoader() {

}

/**
 * 頂点バッファキャッシュを生成する
 * キャッシュは一括してVertexBufferへ転送する
 */
void FigureLoader::createVerticesCache(const s32 vertices_num) {
    u8* vertices = (u8*) new SkinVertex[vertices_num];
    this->cacheVertices.reset(vertices);
}
/**
 * フィギュアの基本情報を読み込んだ後に呼び出される
 * サブクラスではノード数分のメモリ確保等を行う。
 */
void FigureLoader::onFigureInfoLoadComplete(const FigureInfo &figureInfo) {
    jclogf("onFigureInfoLoadComplete (%d nodes)", figureInfo.node_num);
}

/**
 * ノード情報を読み込んだ場合に呼び出される
 */
void FigureLoader::onNodeLoadComplete(const jc::FigureDataLoader::NodeInfo &nodeInfo) {
    jclogf("  onNodeLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());
}

/**
 * onNodeLoadComplete()完了後、メッシュ情報を読み込んだ後に呼び出される
 *
 * @param load_request 読み込みを行うデータのリクエストを行う
 */
void FigureLoader::onMeshInfoLoadComplete(const jc::FigureDataLoader::NodeInfo &nodeInfo, const jc::FigureDataLoader::MeshInfo &meshInfo, jc::FigureDataLoader::MeshDataRequest *load_request) {
    jclogf("  onMeshInfoLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());
}

/**
 * 指定したコンテキストが全て読み込み終わった。
 * サブクラスではVBO転送を期待する。
 */
void FigureLoader::onMeshMaterialContextLoadComplete(const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num) {
    jclogf("  onMeshMaterialContextLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());
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
    jclogf("  onMeshDataLoadComplete node(%d) name(%s)", nodeInfo.children_num, nodeInfo.name.c_str());
}

/**
 * 読み込むべきフィギュアデータの読み込みが完了した
 */
void FigureLoader::onLoadCompleted() {
    jclog("  onLoadCompleted");
}

}
}
