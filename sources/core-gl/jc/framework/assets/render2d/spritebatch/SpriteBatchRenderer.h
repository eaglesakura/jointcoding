/*
 * SpriteBatchList.h
 *
 *  Created on: 2013/07/11
 */

#ifndef SPRITEBATCHRENDERER_H_
#define SPRITEBATCHRENDERER_H_

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {
namespace fw {

/**
 * スプライトレンダリング中の引き継ぎキャッシュ保持
 */
struct SpriteRendererCache {
    /**
     * レンダリングする頂点のヘッダ情報
     */
    s32 vertex_head;
    SpriteRendererCache() {
        vertex_head = 0;
    }
};

/**
 * レンダリング用の１グループ設定
 * 一つのレンダリング単位となる。
 * MAX_TEXTURE_UNITSの数が最低限グループ化出来る数になるため、あまりにTexture切り替えが多い描画の場合はさほど効率的にならない可能性がある。
 */
class SpriteRenderer: public Object {
    /**
     * レンダリング時の管理ステート
     */
    SpriteBatchEnvironmentState state;

    /**
     * レンダリンググループ
     */
    PolygonBatchGroup group;
public:
    SpriteRenderer();

    virtual ~SpriteRenderer();

    /**
     * レンダリングを行わせる
     */
    virtual void rendering(MGLState state, SpriteRendererCache *cache);

    /**
     * レンダリンググループを取得する
     */
    virtual PolygonBatchGroup* getBatchGroup() const {
        return const_cast<PolygonBatchGroup*>(&group);
    }

    /**
     *
     */
    virtual SpriteBatchEnvironmentState* getEnvironment() const {
        return const_cast<SpriteBatchEnvironmentState*>(&state);
    }

    /**
     * レンダリングリクエストを送る
     */
    virtual jcboolean request(SpriteBatchSource *batchSource, const PolygonRequest *request) {
        return getBatchGroup()->requestRendering(getEnvironment(), batchSource, request);
    }
};

typedef jc_sp<SpriteRenderer> MSpriteRenderer;

}
}

#endif /* SPRITEBATCHRENDERER_H_ */
