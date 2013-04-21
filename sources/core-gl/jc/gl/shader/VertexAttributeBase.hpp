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
    GLint location;
public:
    VertexAttributeBase() {
        location = ATTRIBUTE_DISABLE_INDEX;
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
    jcboolean setLocation(const MGLShaderProgram shader, const charactor* name) {
        location = shader->getAttribLocation(name);
        return valid();
    }

    /**
     * 頂点属性の設定を行う
     */
    jcboolean setAttribute(MGLState state, const GLint size, const GLenum type, const GLboolean normalized, const GLsizei stride, const GLvoid* ptr, const u32 offset) {
        // 無効な場合は何もしない
        if (!valid()) {
            return jcfalse;
        }

        return state->vertexAttribPointer(location, size, type, normalized, stride, ptr, offset);
    }
};

}
}

#endif /* VERTEXATTRIBUTEBASE_HPP_ */
