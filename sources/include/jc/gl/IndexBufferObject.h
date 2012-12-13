/*
 * JCGLIndexBufferObject.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCGLINDEXBUFFEROBJECT_H_
#define JCGLINDEXBUFFEROBJECT_H_

#if 0
#include    "jcglResource.h"
#include    "jcglState.h"

namespace jc {
namespace gl {

class IndexBufferObject: public Resource {
    GLuint indices;
public:
    IndexBufferObject(MGLState state);
    virtual ~IndexBufferObject();

    /**
     * GLに関連付ける
     */
    virtual void bind();

    /**
     * GLから切り離す
     */
    virtual void unbind();

    virtual GLuint getIndexBufferObject() const {
        return indices;
    }

    /**
     * データを転送する
     * @param data 転送元のデータ
     * @param size 転送するバイト数
     * @param usage データの利用方法
     */
    virtual void bufferData(const void* data, const u32 size, const GLenum usage) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (GLvoid*) data, usage);
    }

    /**
     * リソースを開放する
     */
    virtual void dispose();
};

/**
 * managed
 */
typedef jc_sp<IndexBufferObject> MGLIndexBufferObject;

}
}


#endif
#endif /* JCGLINDEXBUFFEROBJECT_H_ */
