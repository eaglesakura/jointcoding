/*
 * GLIndexBufferObject.cpp
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#include "jcglIndexBufferObject.h"

namespace jc {
namespace gl {

IndexBufferObject::IndexBufferObject(MGLState state) :
        Resource(state) {
    glGenBuffers(1, &indices);
}

IndexBufferObject::~IndexBufferObject() {
    dispose();
}

/**
 * GLに関連付ける
 */
void IndexBufferObject::bind() {
    state->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
}

/**
 * GLから切り離す
 */
void IndexBufferObject::unbind() {
    if (state->isBindedBuffer(GL_ELEMENT_ARRAY_BUFFER, indices)) {
        state->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

/**
 * リソースを開放する
 */
void IndexBufferObject::dispose() {
    if (indices) {
        unbind();
        glDeleteBuffers(1, &indices);
        indices = 0;
    }
}

}
}
