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
     * ロケーション設定を行う
     */
    void setLocation(const GLint location) {
        // disable indexか0以上のロケーション設定が必要になる
        assert(location == UNIFORM_DISABLE_INDEX || location >= 0);

        this->location = location;
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
        return valid();
    }
};

}
}

#endif /* UNIFORMBASE_HPP_ */
