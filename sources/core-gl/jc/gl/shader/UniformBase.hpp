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
    GLint location;

    UniformBase() {
        location = UNIFORM_DISABLE_INDEX;
    }
public:

    ~UniformBase() {
    }

    /**
     * Uniform値が有効ならばtrueを返す
     */
    jcboolean valid() const {
        return location != UNIFORM_DISABLE_INDEX;
    }

    /**
     * Uniformロケーションを取得する。
     * 無効な場合、UNIFORM_DISABLE_INDEXが帰る.
     */
    GLint getLocation() const {
        return location;
    }

    /**
     * uniform locationを取得する
     */
    jcboolean setLocation(const MGLShaderProgram program, const charactor* name) {
        location = program->getUniformLocation(name);
        if (valid()) {
            return jctrue;
        } else {
            return jcfalse;
        }
    }
};

}
}

#endif /* UNIFORMBASE_HPP_ */
