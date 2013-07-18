/*
 * SpriteBatchList.h
 *
 *  Created on: 2013/07/12
 */

#ifndef SPRITEBATCHLIST_H_
#define SPRITEBATCHLIST_H_

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {
namespace fw {

/**
 * スプライトレンダリングリスト
 *
 * 更新用・描画用でダブルバッファリングを行うため、実際に描画する際は2つのインスタンスを入れ替えて行う。
 */
class SpriteBatchList {
    /**
     * レンダリング用グループ
     */
    std::list<MSpriteRenderer> sprites;

    /**
     * プリミティブ配列
     */
    SpriteBatchSource primitives;

    /**
     * レンダリング用シェーダー
     */
    MPrimitiveBatchShader shader;

    /**
     * レンダリングソース
     */
    struct {
        /**
         * インデックスバッファオブジェクト
         */
        MIndexBufferObject indices;

        /**
         *
         */
        jc_sp<VertexBufferObject<PrimitiveBatchVertex> > vertices;
    }vbo;

    enum {
        /**
         * リクエストを受け付けている状態
         */
        BatchState_Requesting,

        /**
         * アップロードが完了している状態
         */
        BatchState_Uploaded,

        /**
         * レンダリング中な状態
         */
        BatchState_Rendering,

        /**
         * 何も行なっていない状態
         */
        BatchState_Wait,
    }state;
public:
    SpriteBatchList();

    virtual ~SpriteBatchList();

    /**
     * レンダリング用シェーダーを設定する
     */
    virtual void setShader(MPrimitiveBatchShader shader) {
        assert(shader);
        this->shader = shader;
    }

    /**
     * 再度初期化を行う。
     * 毎フレーム、操作を開始する度に行う。
     */
    virtual void initialize(MDevice device);

    /**
     * GPUへレンダリングソースをアップロードする
     */
    virtual void uploadGPU(MDevice device);

    /**
     * レンダリングを行わせる
     */
    virtual void rendering(MGLState state);

    /**
     * スプライトレンダリングをリクエストする
     */
    virtual void request(const PolygonRequest *request);
};

/**
 * managed
 */
typedef jc_sp<SpriteBatchList> MSpriteBatchList;

}
}

#endif /* SPRITEBATCHLIST_H_ */
