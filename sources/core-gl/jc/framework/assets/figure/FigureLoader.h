/*
 * FigureLoader.h
 *
 *  Created on: 2013/05/20
 */

#ifndef FIGURELOADER_H_
#define FIGURELOADER_H_

#include    "jc/framework/assets/FigureAsset.hpp"
#include    "jc/graphics/figure/data/FigureDataLoader.h"

namespace jc {
namespace fw {

/**
 * フィギュアデータの生成を行う
 */
class FigureLoader: public jc::FigureDataLoader {
    /**
     * 読み込み対象デバイス情報
     */
    MDevice device;

    /**
     * 読み込み対象のフィギュア
     */
    jc_selp<Figure> loadTarget;

protected:
    /**
     * 基本頂点情報
     */
    safe_array<BasicVertex> cacheBasicVertices;

    /**
     * 拡張用スキニング頂点
     */
    safe_array<SkinningVertex> cacheSkinningVertices;

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

    /**
     * 読み込み対象フィギュアを設定する
     */
    virtual void setLoadTarget(jc_selp<Figure> loadTarget) {
        this->loadTarget = loadTarget;
    }

    /**
     * 読み込みを行う。
     * ファイル名等の初期化は継承したサブクラスで行う。
     */
    virtual void load() {
        // 読み込み対象が存在しなければならない
        assert(loadTarget);
        FigureDataLoader::load();
    }
protected:

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

    /**
     * 読み込むべきフィギュアデータの読み込みが完了した
     */
    virtual void onLoadCompleted();

};

}
}

#endif /* FIGURELOADER_H_ */
