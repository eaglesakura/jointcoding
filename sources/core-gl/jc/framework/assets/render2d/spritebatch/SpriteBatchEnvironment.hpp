/*
 * SpriteBatch.hpp
 *
 *  Created on: 2013/07/01
 */

#ifndef SPRITEBATCHENVIRONMENT_HPP_
#define SPRITEBATCHENVIRONMENT_HPP_

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {

namespace fw {

/**
 * バッチ処理用の頂点
 */
struct PrimitiveBatchVertex {
    /**
     * 位置情報
     */
    Vector3f position;

    /**
     * UV位置情報
     */
    Vector2f coord;

    /**
     * 頂点カラー
     *
     * α等を処理する
     */
    GLuint color;

    /**
     * 回転角情報
     */
    GLshort rotate;

    /**
     * レンダリング対象のテクスチャユニット
     * 負の値の場合、通常のテクスチャを適用せずに描画する
     */
    GLshort texture_index;
};

/**
 * スプライトレンダリング時の環境情報を設定する
 *
 */
class SpriteBatchEnvironmentState: public Object {
    enum {
        /**
         * テクスチャユニット数制限
         */
        MAX_TEXTURES = 16,
    };

    /**
     * ブレンディング情報
     */
    GLBlendType_e blend;

    /**
     * レンダリング用テクスチャ
     */
    std::vector<MTextureImage> textures;

public:
    SpriteBatchEnvironmentState() {
        blend = GLBlendType_Alpha;
    }

    virtual ~SpriteBatchEnvironmentState() {
    }

    /**
     * ブレンディング関数を設定する
     */
    virtual void setBlend(const GLBlendType_e type) {
        this->blend = type;
    }

    virtual GLBlendType_e getBlend() const {
        return blend;
    }

    /**
     * テクスチャを環境へ追加する
     */
    virtual jcboolean addTexture(const MTextureImage texture, s32 *result_index) {
        assert(texture);
        assert(result_index);

        // テクスチャが限界を超えていたら追加できない
        if (textures.size() >= jc::min<u32>(MAX_TEXTURES, GPUCapacity::getMaxTextureUnits())) {
            return jcfalse;
        }

        std::vector<MTextureImage>::iterator itr = textures.begin(), end = textures.end();
        int index = 0;
        while (itr != end) {

            // 既に登録済みなのでその値を返却する
            if ((*itr) == texture) {
                *result_index = index;
                return jctrue;
            }

            ++index;
            ++itr;
        }

        // 末尾に追加する
        textures.push_back(texture);
        *result_index = textures.size();

        return jctrue;
    }

    /**
     * レンダリング準備を行わせる
     */
    virtual void bind(MGLState state) {
        // ブレンド情報を設定する
        state->blendFunc(blend);
    }
};

/**
 * managed
 */
typedef jc_sp<SpriteBatchEnvironmentState> MSpriteBatchEnvironmentState;

}
}

#endif /* SPRITEBATCH_HPP_ */
