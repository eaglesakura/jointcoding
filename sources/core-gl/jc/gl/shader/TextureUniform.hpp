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

    /**
     * アップロード対象のコンテキスト
     */
    MGLState state;
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
     * アップロード対象のコンテキストを指定する
     */
    void setState(MGLState state) {
        this->state = state;
        bindUnit = -1;
    }

    /**
     * GPUにアップロードを行う
     */
    jcboolean upload(MTextureImage tex) {
        assert(tex.get() != NULL);

        if (!valid()) {
            return jcfalse;
        }

        const u32 texUnit = tex->bind(state);
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
