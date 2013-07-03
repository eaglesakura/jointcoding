/*
 * TextureUniform.hpp
 *
 *  Created on: 2013/04/20
 */

#ifndef TEXTUREUNIFORM_HPP_
#define TEXTUREUNIFORM_HPP_

#include    "jc/gl/shader/UniformBase.hpp"

namespace jc {
namespace gl {

/**
 * テクスチャのUniformをサポートする
 * 無効なUniform値（コンパイラ最適化で削除された等）の場合はアップロードを無視する
 */
class TextureUniform: public UniformBase {

    enum {
        MAX_TEXTURES = 32
    };
    GLint bindUnit[MAX_TEXTURES];
public:
    TextureUniform() {

        for (int i = 0; i < MAX_TEXTURES; ++i) {
            bindUnit[i] = 0;
        }
    }

    ~TextureUniform() {

    }

    /**
     * GPUにアップロードを行う
     */
    jcboolean upload(MGLState state, MTextureImage *textures, const u32 textures_length) {
        if (!valid()) {
            return jcfalse;
        }

        assert(state);
        assert(textures);
        assert(textures_length <= GPUCapacity::getMaxTextureUnits());

        // キャッシュと比べ、アップロードの必要があったらtrueにする
        jcboolean upload = jctrue;
        for (int i = 0; i < textures_length; ++i) {
            s32 unit = 0;
            if (!textures[i]->isBinded(&unit)) {
                unit = textures[i]->bind(state);
            }

            // 違うユニットが割り当てられていたらアップロードが必要
            if (bindUnit[i] != unit) {
                upload = jctrue;
            }
            bindUnit[i] = unit;
        }

        // アップロードフラグをチェックする
        if (upload) {
//            glUniform1iv(location, textures_length, bindUnit);
            glUniform1i(location, bindUnit[0]);
            assert_gl();
            return jctrue;
        }

        return jcfalse;
    }

    /**
     * GPUにアップロードを行う
     */
    jcboolean upload(MGLState state, MTextureImage tex) {
        if (!valid()) {
            return jcfalse;
        }

        s32 texUnit = -1;
        // バインドされていなければ、テクスチャをバインドする
        if (!tex->isBinded(&texUnit)) {
            texUnit = tex->bind(state);
        }
        if (bindUnit[0] != texUnit) {
            glUniform1i(location, texUnit);
            bindUnit[0] = texUnit;
            assert_gl();
            return jctrue;
        }

        return jcfalse;
    }
};

}
}

#endif /* TEXTUREUNIFORM_HPP_ */
