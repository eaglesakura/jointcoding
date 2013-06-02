/*
 * RenderBufferObject.hpp
 *
 *  Created on: 2013/06/02
 */

#ifndef RENDERBUFFEROBJECT_HPP_
#define RENDERBUFFEROBJECT_HPP_

#include    "jointcoding.h"
#include    "jc/gl/gpu/VRAM.h"
#include    "jc/gl/gpu/Device.h"

namespace jc {
namespace gl {

/**
 * オフスクリーンレンダリング用のレンダリングバッファを管理する
 */
class RenderBufferObject: public Object {
    vram_handle buffer;

    GLenum internalformat;

    /**
     * バッファのXYサイズ
     */
    Vector2i size;
public:
    RenderBufferObject(MDevice device, const GLenum internalformat) {
        assert(device);

        buffer = device->getVRAM()->alloc(VRAM_RenderBuffer);
        assert(buffer.get());

        this->internalformat = internalformat;
    }

    virtual ~RenderBufferObject() {
    }

    /**
     *
     */
    virtual GLuint getName() {
        return buffer.get();
    }

    /**
     * GLへ関連付ける
     */
    virtual void bind(MGLState state) {
        state->bindRenderbuffer(GL_RENDERBUFFER, buffer.get());
    }

    /**
     * GLとの関連付けを解除する
     */
    virtual void unbind(MGLState state) {
        if (state->isBindedRenderbuffer(buffer.get())) {
            state->bindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }

    /**
     * WHを指定してバッファのリサイズを行う
     */
    virtual void resize(MGLState state, const u32 width, const u32 height) {
        // 同一サイズのため、何も行う必要はない
        if (width == size.x || height == size.y) {
            return;
        }

        bind(state);
        glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);

        size.x = width;
        size.y = height;
    }

    /**
     * 幅を取得する
     */
    virtual s32 getWidth() const {
        return size.x;
    }

    /**
     * 高さを取得する
     */
    virtual s32 getHeight() const {
        return size.y;
    }
};

/**
 * managed
 */
typedef jc_sp<RenderBufferObject> MRenderBufferObject;

}
}

#endif /* RENDERBUFFEROBJECT_HPP_ */
