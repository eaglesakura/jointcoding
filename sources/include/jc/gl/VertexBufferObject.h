/*
 * JCGLVertexBufferObject.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCGLVERTEXBUFFEROBJECT_H_
#define JCGLVERTEXBUFFEROBJECT_H_

#if 0
#include    "jc/gl/Resource.h"

namespace jc {
namespace gl {

/**
 * 1つのVBOを扱うクラス。
 */
class VertexBufferObject: public Resource {
protected:
    /**
     * 生成されたバッファ
     */
    GLuint buffer;
public:
    VertexBufferObject(MGLState state);
    virtual ~VertexBufferObject();

    /**
     * バッファを関連付ける
     */
    virtual void bind();

    /**
     * バッファを切り離す
     */
    virtual void unbind();

    /**
     * リソースの解放を行う
     */
    virtual void dispose();

    /**
     * バッファにデータを転送する。
     * @param source 転送元のデータポインタ
     * @param length ソースのバイト数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    virtual void bufferData(const void* source, const u32 length, const GLenum usage) {
        glBufferData(GL_ARRAY_BUFFER, length, source, usage);
    }

    /**
     * 生成された頂点オブジェクトを取得する
     */
    virtual GLuint getVertexBufferObject() const {
        return buffer;
    }
};

/**
 * managed
 */
typedef jc_sp<VertexBufferObject> MGLVertexBufferObject;

}
}

#endif
#endif /* JCGLVERTEXBUFFEROBJECT_H_ */
