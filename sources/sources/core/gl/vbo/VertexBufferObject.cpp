/*
 * GLVertexBufferObject.cpp
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#if 0
#include    "jc/gl/GL.h"
#include    "jc/gl/VertexBufferObject.h"

namespace jc {
namespace gl {

VertexBufferObject::VertexBufferObject(MGLState state) : Resource(state) {
    glGenBuffers(1, &buffer);
}

VertexBufferObject::~VertexBufferObject() {
    this->dispose();
}

/**
 * バッファを関連付ける
 */
void VertexBufferObject::bind() {
    state->bindBuffer(GL_ARRAY_BUFFER, buffer);
}

/**
 * バッファを切り離す
 */
void VertexBufferObject::unbind() {
    if (state->isBindedBuffer(GL_ARRAY_BUFFER, buffer)) {
        state->bindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

/**
 * リソースの解放を行う
 */
void VertexBufferObject::dispose() {
    if (buffer) {
        unbind();
        glDeleteBuffers(1, &buffer);
        buffer = BUFFER_NULL;
    }

}

}
}

#endif
