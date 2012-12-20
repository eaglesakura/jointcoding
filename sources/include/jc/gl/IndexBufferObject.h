/*
 * JCGLIndexBufferObject.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCGLINDEXBUFFEROBJECT_H_
#define JCGLINDEXBUFFEROBJECT_H_

#include    "jc/gl/VRAM.h"
#include    "jc/gl/State.h"

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
     * レンダリングを行う
     */
    virtual void rendering();
};

}
}

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
