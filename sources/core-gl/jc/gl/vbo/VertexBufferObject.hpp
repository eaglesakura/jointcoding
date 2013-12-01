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

/**
 * 頂点バッファオブジェクトを管理する
 *
 * 生成は内部的に行うが、廃棄はVRAMのgcに任せる
 * Context間を移動する際は必ずunbind()を行い、stateから外すこと。
 */
template<typename VertexType>
class VertexBufferObject: public Object {
    /**
     * 確保したVBO
     */
    GLObject vertices;

public:
    VertexBufferObject(MDevice device) {
        assert(device);
        vertices.alloc(device->getVRAM(VRAM_VertexBufferObject));
    }

    virtual ~VertexBufferObject() {
    }

    /**
     * バッファにデータを転送する。
     * 必ずbind()済みの状態で呼び出すこと
     * @param source 転送元のデータポインタ
     * @param sizeof_vertex ソースの頂点サイズ
     * @param length ソースの頂点数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    virtual void bufferData(const void *vertices, const s32 sizeof_vertex, const u32 vertices_length, const GLenum usage) {
        glBufferData(GL_ARRAY_BUFFER, sizeof_vertex * vertices_length, (GLvoid*) vertices, usage);
        assert_gl();
    }

    /**
     * GLのステートへと関連付ける
     */
    virtual void bind(MGLState state) {
        state->bindBuffer(GL_ARRAY_BUFFER, vertices.get());
    }

    /**
     * GLのステートから関連を外す
     */
    virtual void unbind(MGLState state) {
        if (state->isBindedBuffer(GL_ARRAY_BUFFER, vertices.get())) {
            state->bindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
};

}
}

#endif /* JCGLVERTEXBUFFEROBJECT_H_ */
