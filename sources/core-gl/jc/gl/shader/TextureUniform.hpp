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
    u32 bindUnit;
public:
    TextureUniform() {
        bindUnit = -1;
    }

    TextureUniform(MGLShaderProgram program, const charactor *name) {
        bindUnit = -1;
        setLocation(program, name);
    }

    ~TextureUniform() {

    }

    /**
     * GPUにアップロードを行う
     */
    jcboolean upload(MGLState state, MTextureImage tex) {
        assert(tex);

        if (!valid()) {
            return jcfalse;
        }

        s32 texUnit = -1;
        // バインドされていなければ、テクスチャをバインドする
        if (!tex->isBinded(&texUnit)) {
            texUnit = tex->bind(state);
        }
        if (bindUnit != texUnit) {
            glUniform1i(location, texUnit);
            assert_gl();
            bindUnit = texUnit;
            return jctrue;
        }
        return jcfalse;
    }
};

}
}

#endif /* TEXTUREUNIFORM_HPP_ */
