/*
 * TextureUniform.hpp
 *
 *  Created on: 2013/04/20
 */

#ifndef TEXTUREUNIFORM_HPP_
#define TEXTUREUNIFORM_HPP_

#include    "jc/gl/shader/UniformBase.hpp"
#define JC_TEXTURE_FORCE_UPLOAD

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
    jcboolean upload(MTextureImage tex) {
        assert(tex.get() != NULL);

        if (!valid()) {
            return jcfalse;
        }
        const u32 texUnit = tex->bind();


#ifndef JC_TEXTURE_FORCE_UPLOAD
        if (bindUnit != texUnit) {
#endif
            glUniform1i(location, texUnit);
            assert_gl();
            bindUnit = texUnit;
            return jctrue;
#ifndef JC_TEXTURE_FORCE_UPLOAD
        }
        return jcfalse;
#endif
    }
};

}
}

#endif /* TEXTUREUNIFORM_HPP_ */
