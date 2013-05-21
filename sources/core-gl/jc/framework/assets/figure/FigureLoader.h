/*
 * FigureLoader.h
 *
 *  Created on: 2013/05/20
 */

#ifndef FIGURELOADER_H_
#define FIGURELOADER_H_

#include    "jc/framework/assets/figure/FigureAsset.hpp"
#include    "jc/graphics/figure/data/FigureDataLoader.h"

namespace jc {
namespace gl {

/**
 * フィギュアデータの生成を行う
 */
class FigureLoader: public jc::FigureDataLoader {
    /**
     * 頂点バッファキャッシュ
     */
    jc_sp<u8> cacheVertices;

    /**
     * 読み込み対象のコンテキスト
     */
    MGLState state;

    /**
     * 生成元のVRAM
     */
    VRAM vram;

protected:
    /**
     * 頂点バッファキャッシュを生成する
     * キャッシュは一括してVertexBufferへ転送する
     *
     * @param vertices_num 頂点数
     */
    virtual void createVerticesCache(const s32 vertices_num);

public:
    FigureLoader(MDevice device, MFigureDataFactory factory);

    virtual ~FigureLoader();

protected:
    /**
     * 読み込むべきフィギュアデータの読み込みが完了した
     */
    virtual void onLoadCompleted();

    /**
     * フィギュアの基本情報を読み込んだ後に呼び出される
     * サブクラスではノード数分のメモリ確保等を行う。
     */
    virtual void onFigureInfoLoadComplete(const FigureInfo &figureInfo);

    /**
     * ノード情報を読み込んだ場合に呼び出される
     */
    virtual void onNodeLoadComplete(const jc::FigureDataLoader::NodeInfo &nodeInfo);

    /**
     * onNodeLoaded()完了後、メッシュ情報を読み込んだ後に呼び出される
     *
     * @param load_request 読み込みを行うデータのリクエストを行う
     */
    virtual void onMeshInfoLoadComplete(const jc::FigureDataLoader::NodeInfo &nodeInfo, const jc::FigureDataLoader::MeshInfo &meshInfo, jc::FigureDataLoader::MeshDataRequest *load_request);

    /**
     * 指定したコンテキストが全て読み込み終わった。
     * サブクラスではVBO転送を期待する。
     */
    virtual void onMeshMaterialContextLoadComplete(const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num);

    /**
     * メッシュデータの読み込みを完了した
     *
     * @param material_num マテリアル番号
     * @param context_num コンテキスト番号
     * @param dataType 読み込みを行ったデータタイプ
     * @param loaded_data 読み込みを行ったデータ
     */
    virtual void onMeshDataLoadComplete(const FigureDataLoader::NodeInfo &nodeInfo, const FigureDataLoader::MeshInfo &meshInfo, const s32 material_num, const s32 context_num, const FigureDataLoader::MeshData &loaded);
};

}
}

#endif /* FIGURELOADER_H_ */
