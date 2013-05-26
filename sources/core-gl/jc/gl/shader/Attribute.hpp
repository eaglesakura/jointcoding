/*
 * Attribute.hpp
 *
 *  Created on: 2013/05/26
 */

#ifndef ATTRIBUTE_HPP_
#define ATTRIBUTE_HPP_

#include    "jc/gl/shader/ShaderProgram.h"

namespace jc {
namespace gl {

class Attribute {
protected:
    /**
     * Uniformインデックス
     */
    GLint location;

    /**
     * 利用を行うフラグ
     * falseにした場合、この属性をdisableに指定する
     */
    jcboolean is_use;
public:
    Attribute() {
        location = ATTRIBUTE_DISABLE_INDEX;
        is_use = jctrue;
    }
    ~Attribute() {
    }

    /**
     * attribute値が有効ならばtrueを返す
     */
    jcboolean valid() const {
        return location != ATTRIBUTE_DISABLE_INDEX;
    }

    /**
     * ロケーション設定を行う
     */
    void setLocation(const GLint location) {
        // disable indexか0以上のロケーション設定が必要になる
        assert(location == ATTRIBUTE_DISABLE_INDEX || location >= 0);

        this->location = location;
    }

    /**
     * attrubuteロケーションを取得する。
     * 無効な場合、UNIFORM_DISABLE_INDEXが帰る.
     */
    GLint getLocation() const {
        return location;
    }

    /**
     * attribute locationを取得する
     */
    jcboolean setLocation(const MGLShaderProgram program, const charactor* name) {
        location = program->getAttribLocation(name);
        return valid();
    }

    /**
     * 利用しない
     */
    void unuse(MGLState state) {
        is_use = jcfalse;
    }

    /**
     * 利用を行う
     */
    void use(MGLState state) {
        is_use = jctrue;
    }

    /**
     * GPUへ属性設定を行う
     */
    jcboolean attribute(MGLState state, const GLint size, const GLenum type, const GLboolean normalized, const GLsizei stride, const GLvoid *ptr, const u32 offset) {
        if (!valid()) {
            return jcfalse;
        }

        if (is_use) {
            // 属性を有効にする
            state->enableVertexAttribArray(location);

            // 関連付けを行う
            state->vertexAttribPointer(location, size, type, normalized, stride, ptr, offset);
        } else {
            // 属性を無効化する
            state->disableVertexAttribArray(location);
        }
        return jctrue;
    }
};

}
}

#endif /* ATTRIBUTE_HPP_ */
