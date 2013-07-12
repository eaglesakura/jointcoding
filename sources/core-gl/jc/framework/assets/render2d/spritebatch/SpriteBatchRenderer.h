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
 * レンダリング用の１グループ設定
 */
class SpriteRenderer: public Object {
    /**
     * レンダリング時の管理ステート
     */
    SpriteBatchEnvironmentState state;

    /**
     * レンダリンググループ
     */
    SpriteBatchGroup group;
public:
    SpriteRenderer();

    virtual ~SpriteRenderer();

    /**
     * レンダリングを行わせる
     */
    virtual void rendering(MGLState state);

    /**
     * レンダリンググループを取得する
     */
    virtual SpriteBatchGroup* getBatchGroup() const {
        return &group;
    }

    /**
     *
     */
    virtual SpriteBatchEnvironmentState* getEnvironment() const {
        return &state;
    }
};

typedef jc_sp<SpriteRenderer> MSpriteRenderer;

}
}

#endif /* SPRITEBATCHRENDERER_H_ */
