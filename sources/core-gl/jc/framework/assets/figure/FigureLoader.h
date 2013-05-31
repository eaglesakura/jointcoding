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
    jc_selp<Figure> figure;

protected:

    /**
     * 現在の描画フラグメント情報
     */
    struct CurrentFragment {
        s32 vertices;
        s32 indices;

        CurrentFragment() {
            vertices = indices = 0;
        }
    }currentFragment;

    /**
     * キャッシュしてあるバッファ
     * 全部の読み込み完了後、一括転送を行う
     */
    struct Buffer {
        /**
         * 基本頂点
         */
        safe_array<BasicVertex> vertices;

        /**
         * 拡張用スキニング頂点
         */
        safe_array<SkinningVertex> skinnings;

        /**
         * インデックスバッファ
         */
        safe_array<u16> indices;

        /**
         * 現在の頂点のヘッダ
         */
        s32 vertex_header;

        /**
         * 現在のインデックスバッファヘッダ
         */
        s32 index_header;

        Buffer() {
            index_header = vertex_header = 0;
        }
    }buffer;

    /**
     * キャッシュ用の一時メモリを作成する
     * キャッシュは一括してVertexBufferへ転送する
     *
     * @param vertices_num 頂点数
     * @param indices_num インデックス数
     */
    virtual void createCacheBuffer(const s32 vertices_num, const s32 indices_num);

    /**
     * 現在の操作対象位置のバッファを取得する
     */
    virtual unsafe_array<BasicVertex> getCurrentVertices() const {
        return buffer.vertices.slice(buffer.vertex_header);
    }

    /**
     * 現在の操作対象位置のバッファを取得する
     */
    virtual unsafe_array<SkinningVertex> getCurrentSkinnings() const {
        return buffer.skinnings.slice(buffer.vertex_header);
    }

    /**
     * 現在の操作対象位置のインデックスバッファを取得する
     */
    virtual unsafe_array<u16> getCurrentIndices() const {
        return buffer.indices.slice(buffer.index_header);
    }

    /**
     * 一つのメッシュグループ読み込みが完了した
     */
    virtual void onGroupLoadComplete() {
        buffer.vertex_header += currentFragment.vertices;
        buffer.index_header += currentFragment.indices;

        currentFragment = CurrentFragment();
    }
public:
    FigureLoader(MDevice device, MFigureDataFactory factory);

    virtual ~FigureLoader();

    /**
     * 読み込み対象フィギュアを設定する
     */
    virtual void setLoadTarget(jc_selp<Figure> loadTarget) {
        this->figure = loadTarget;
    }

    /**
     * 読み込みを行う。
     * ファイル名等の初期化は継承したサブクラスで行う。
     */
    virtual void load() {
        // 読み込み対象が存在しなければならない
        assert(figure);
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
