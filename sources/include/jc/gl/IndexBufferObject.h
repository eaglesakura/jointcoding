/*
 * JCGLIndexBufferObject.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCGLINDEXBUFFEROBJECT_H_
#define JCGLINDEXBUFFEROBJECT_H_

#include    "jointcoding.h"
#include    "jc/gl/VRAM.h"
#include    "jc/gl/Device.h"

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
     * GLのステーと管理
     */
    MGLState state;
public:
    IndexBufferObject(MDevice device);
    virtual ~IndexBufferObject();

    /**
     * GLへ関連付ける
     */
    virtual void bind();

    /**
     * GLとの関連付けを解除する
     */
    virtual void unbind();

    /**
     * 解放を行う
     */
    virtual void dispose();

    /**
     * データを転送する
     * @param data 転送元のデータ
     * @param size 転送するバイト数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    virtual void bufferData(const u16 *indices, const u32 indices_length, const GLenum usage) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length * sizeof(u16), (GLvoid*) indices, usage);
        this->indices_length = indices_length;
    }

    /**
     * レンダリングを行う
     */
    virtual void rendering();
};

/**
 * managed
 */
typedef jc_sp<IndexBufferObject> MIndexBufferObject;

}
}

#endif /* JCGLINDEXBUFFEROBJECT_H_ */
