/*
 * JCGLIndexBufferObject.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCGLINDEXBUFFEROBJECT_H_
#define JCGLINDEXBUFFEROBJECT_H_

#include    "jointcoding.h"
#include    "jc/gl/gpu/VRAM.h"
#include    "jc/gl/gpu/Device.h"

namespace jc {
namespace gl {

class IndexBufferObject: public Object {
    /**
     * 確保したインデックスバッファ
     */
    SharedResource indices;

    /**
     * インデックスバッファの配列長
     */
    u32 indices_length;

    /**
     * GLのステート管理
     */
    MGLState state;
public:
    IndexBufferObject(MDevice device) {
        assert(device.get() != NULL);

        this->state = device->getState();
        this->indices.alloc(device->getVRAM(), VRAM_Indices);
        this->indices_length = 0;
    }

    virtual ~IndexBufferObject() {
        dispose();
    }

    /**
     * GLへ関連付ける
     */
    virtual void bind() {
        state->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.get());
    }

    /**
     * GLとの関連付けを解除する
     */
    virtual void unbind() {
        if (state->isBindedBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.get())) {
            state->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    /**
     * 解放を行う
     */
    virtual void dispose() {
        unbind();
        indices.release();
    }

    /**
     * データを転送する
     * @param data 転送元のデータ
     * @param size 転送するバイト数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    virtual void bufferData(const u16 *indices, const u32 indices_length, const GLenum usage) {
        this->indices_length = indices_length;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length * sizeof(u16), (GLvoid*) indices, usage);
        assert_gl();
    }

    /**
     * レンダリングを行う
     */
    virtual void rendering() {
        glDrawElements(GL_TRIANGLES, indices_length, GL_UNSIGNED_SHORT, NULL);
        assert_gl();
    }
};

/**
 * managed
 */
typedef jc_sp<IndexBufferObject> MIndexBufferObject;

}
}

#endif /* JCGLINDEXBUFFEROBJECT_H_ */
