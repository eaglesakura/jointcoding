/*
 * VertexAttribute.hpp
 *
 *  Created on: 2013/04/20
 */

#ifndef VERTEXATTRIBUTEBASE_HPP_
#define VERTEXATTRIBUTEBASE_HPP_

#include    "jc/gl/shader/ShaderProgram.h"

namespace jc {
namespace gl {

class VertexAttributeBase {
protected:
    /**
     * ロケーション
     */
    GLint location;

    /**
     * 有効・無効フラグ
     * falseが設定されている場合、強制的にdisableする
     */
    jcboolean attrEnable;
public:
    VertexAttributeBase() {
        location = ATTRIBUTE_DISABLE_INDEX;
        attrEnable = jctrue;
    }

    ~VertexAttributeBase() {

    }

    /**
     * ロケーション取得する
     */
    GLint getLocation() const {
        return location;
    }

    /**
     * locationが設定済みの場合jctrueを返す
     * 最適化によってprogramから除かれたlocationも!valid()となる。
     */
    jcboolean valid() const {
        return location != ATTRIBUTE_DISABLE_INDEX;
    }

    /**
     * ロケーション設定を行う
     */
    void setLocation(const GLint location) {
        // disable indexか0以上のロケーション設定が必要になる
        assert(location == ATTRIBUTE_DISABLE_INDEX || location > 0);

        this->location = location;
    }

    /**
     * ロケーション設定を行う
     */
    jcboolean setLocation(const MGLShaderProgram shader, const charactor* name) {
        assert(shader.get() != NULL);
        assert(name != NULL);

        location = shader->getAttribLocation(name);
        return valid();
    }

    /**
     * 有効化する
     */
    jcboolean enable(MGLState state) {
        if (!valid()) {
            return jcfalse;
        }
        return state->enableVertexAttribArray(location);
    }

    /**
     * 無効化する
     */
    jcboolean disable(MGLState state) {
        if (!valid()) {
            return jcfalse;
        }
        return state->disableVertexAttribArray(location);
    }

    /**
     * 属性を利用する
     */
    void use() {
        attrEnable = jctrue;
    }

    /**
     * 属性を利用しない
     */
    void unuse() {
        attrEnable = jcfalse;
    }
};

}
}

#endif /* VERTEXATTRIBUTEBASE_HPP_ */
