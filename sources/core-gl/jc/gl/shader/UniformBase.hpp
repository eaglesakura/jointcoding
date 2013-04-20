/*
 * UniformBase.hpp
 *
 *  Created on: 2013/04/20
 */

#ifndef UNIFORMBASE_HPP_
#define UNIFORMBASE_HPP_

#include    "jc/gl/shader/ShaderProgram.h"

namespace jc {
namespace gl {

class UniformBase {
protected:
    /**
     * Uniformインデックス
     */
    GLint uniform;

    /**
     * 関連付けられたシェーダー
     */
    MGLShaderProgram program;

    UniformBase() {
        uniform = UNIFORM_DISABLE_INDEX;
    }
public:

    ~UniformBase() {
    }

    /**
     * Uniform値が有効ならばtrueを返す
     */
    jcboolean valid() const {
        return uniform != UNIFORM_DISABLE_INDEX;
    }
    /**
     * uniform locationを取得する
     */
    jcboolean setUniofrmLocation(const MGLShaderProgram program, const charactor* name) {
        uniform = program->getUniformLocation(name);
        if (valid()) {
            this->program = program;
            return jctrue;
        } else {
            return jcfalse;
        }
    }

    /**
     * uniofrm locationを取得する
     */
    jcboolean setUniformLocation(const MGLShaderProgram program, const String &name) {
        return setUniofrmLocation(program, (const charactor*) name.c_str());
    }

};

}
}

#endif /* UNIFORMBASE_HPP_ */
