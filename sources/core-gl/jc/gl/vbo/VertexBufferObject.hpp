/*
 * JCGLVertexBufferObject.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCGLVERTEXBUFFEROBJECT_H_
#define JCGLVERTEXBUFFEROBJECT_H_

#include    "jointcoding.h"
#include    "jc/gl/gpu/VRAM.h"
#include    "jc/gl/gpu/Device.h"

namespace jc {
namespace gl {

template<typename VertexType>
class VertexBufferObject: public Object {
    /**
     * 確保したVBO
     */
    SharedResource vertices;

    /**
     * バッファ
     */
    MGLState state;
public:
    VertexBufferObject(MDevice device) {
        this->state = device->getState();
        vertices.alloc(device->getVRAM(), VRAM_VertexBufferObject);
    }

    virtual ~VertexBufferObject() {
        dispose();
    }
    /**
     * バッファにデータを転送する。
     * 必ずbind()済みの状態で呼び出すこと
     * @param source 転送元のデータポインタ
     * @param length ソースの頂点数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    virtual void bufferData(const VertexType *vertices, const u32 vertices_length, const GLenum usage) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType) * vertices_length, (GLvoid*) vertices, usage);
    }

    /**
     * GLのステートへと関連付ける
     */
    virtual void bind() {
        state->bindBuffer(GL_ARRAY_BUFFER, vertices.get());
    }

    /**
     * GLのステートから関連を外す
     */
    virtual void unbind() {
        if (state->isBindedBuffer(GL_ARRAY_BUFFER, vertices.get())) {
            state->bindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    /**
     * 解放を行う
     */
    virtual void dispose() {
        unbind();
        vertices.release();
    }
};

}
}

#endif /* JCGLVERTEXBUFFEROBJECT_H_ */
