/*
 * GLIndexBufferObject.cpp
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#include    "jc/gl/IndexBufferObject.h"

namespace jc {
namespace gl {

IndexBufferObject::IndexBufferObject(MDevice device) {
    this->state = device->getState();
    this->indices.alloc(device->getVRAM(), VRAM_Indices);
    this->indices_length = 0;
}

IndexBufferObject::~IndexBufferObject() {
    dispose();
}

/**
 * GLへ関連付ける
 */
void IndexBufferObject::bind() {
    state->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.get());
}

/**
 * GLとの関連付けを解除する
 */
void IndexBufferObject::unbind() {
    if (state->isBindedBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.get())) {
        state->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

/**
 * 解放を行う
 */
void IndexBufferObject::dispose() {
    unbind();
    indices.release();
}

/**
 * レンダリングを行う
 */
void IndexBufferObject::rendering() {
    CLEAR_GL_ERROR
    glDrawArrays(GL_TRIANGLES, 0, indices_length);
    PRINT_GL_ERROR;
}

}
}
