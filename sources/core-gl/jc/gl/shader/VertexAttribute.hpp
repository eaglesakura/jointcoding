/*
 * VertexAttribute.hpp
 *
 *  Created on: 2013/04/21
 */

#ifndef VERTEXATTRIBUTE_HPP_
#define VERTEXATTRIBUTE_HPP_

#include    "jc/gl/shader/VertexAttributeBase.hpp"

namespace jc {
namespace gl {

/**
 * 頂点構造体を利用した頂点属性
 *
 * @param vertex_struct 頂点構造体型
 * @param attr_size 属性数(Vec3 = 3, Vec2 = 2)
 * @param attr_type 属性タイプ(float = GL_FLOAT, u8 = GL_UNSIGNED_BYTE）
 * @param attr_normalized GL_UNSIGNED_BYTE -> GL_FLOAT変換時に0.0〜1.0に丸める場合はGL_TRUE
 * @param offset_header 頂点構造体から実際の属性までのオフセット値 {vec3, vec2}で後半にアクセスする場合はsizeof(vec3)を指定する
 */
template<typename vertex_struct, GLsizei attr_size, GLenum attr_type, GLboolean attr_normalized, GLsizei offset_header>
class VertexAttribute: public VertexAttributeBase {
public:
    VertexAttribute() {
    }

    VertexAttribute(MGLShaderProgram program, const charactor *name) {
        setLocation(program, name);
    }

    ~VertexAttribute() {
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
     * 頂点属性の設定を行う
     */
    jcboolean attributePointer(MGLState state, const GLvoid* ptr = NULL, const jcboolean withEnable = jctrue) {
        // 無効な場合は何もしない
        if (!valid()) {
            return jcfalse;
        }

        if (withEnable) {
            enable(state);
        }
        return state->vertexAttribPointer(location, attr_size, attr_type, attr_normalized, sizeof(vertex_struct), ptr, offset_header);
    }
};

}
}

#endif /* VERTEXATTRIBUTE_HPP_ */
