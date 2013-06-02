/*
 * FrameBufferObject.hpp
 *
 *  Created on: 2013/06/02
 */

#ifndef FRAMEBUFFEROBJECT_HPP_
#define FRAMEBUFFEROBJECT_HPP_

#include    "jointcoding.h"
#include    "jc/gl/gpu/VRAM.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/gl/render/RenderBufferObject.hpp"

namespace jc {
namespace gl {

/**
 * オフスクリーンレンダリング用フレームバッファを管理する
 */
class FrameBufferObject: public Object {
    vram_handle fbo;

    /**
     * カラーバッファ
     */
    MRenderBufferObject color;

    /**
     * 深度バッファ
     */
    MRenderBufferObject depth;

    /**
     * ステンシルバッファ
     */
    MRenderBufferObject stencil;
public:
    FrameBufferObject(MDevice device) {
        assert(device);

        fbo = device->getVRAM()->alloc(VRAM_FrameBuffer);
        assert(fbo.get());
    }

    virtual ~FrameBufferObject() {
    }

    /**
     * バインドを行う
     */
    virtual void bind(MGLState state) {
        state->bindFramebuffer(GL_FRAMEBUFFER, fbo.get());
    }

    /**
     * バインドされていたら、バインド解除を行う
     */
    virtual void unbind(MGLState state) {
        if (state->isBindedFramebuffer(fbo.get())) {
            state->bindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    /**
     * フレームバッファへレンダリングバッファを関連付ける
     * @param attachiment GL_COLOR_ATTACHIMENT/GL_DEPTH_ATTACHMENT/GL_STENCIL_ATTACHMENT
     */
    virtual void attach(MGLState state, const GLenum attachiment, MRenderBufferObject renderbuffer) {
        bind(state);
        renderbuffer->bind(state);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachiment, renderbuffer->getName());
        assert_gl();

        // アタッチメントの種類によって保存先を変更する
        switch (attachiment) {
            case GL_COLOR_ATTACHMENT0:
                this->color = renderbuffer;
                break;
            case GL_DEPTH_ATTACHMENT:
                this->depth = renderbuffer;
                break;
            case GL_STENCIL_ATTACHMENT:
                this->stencil = renderbuffer;
                break;
        }
    }

    /**
     * カラーアタッチメントを持っているならtrue
     */
    virtual jcboolean hasColorAttachiment() const {
        return (bool) color;
    }

    /**
     * 深度アタッチメントを持っているならtrue
     */
    virtual jcboolean hasDepthAttachiment() const {
        return (bool) depth;
    }

    /**
     * ステンシルアタッチメントを持っているならtrue
     */
    virtual jcboolean hasStencilAttachiment() const {
        return (bool) stencil;
    }

    /**
     * カラーレンダリングバッファを確保する
     */
    virtual void allocColorRenderbuffer(MDevice device, const PixelFormat_e pixelFormat) {
        assert(pixelFormat != PixelFormat_BGRA8888);

        // カラーフォーマットテーブルを用意しておく
        const GLenum formats[] = {
        //
                GL_RGB565,
                //
                GL_RGB5_A1,
                //
                GL_RGB8_OES,
                //
                GL_RGBA8_OES, };

        // RGB8/RGBA8をサポートしていないGPUの場合、フォーマットを書き換える
        if (!GPUCapacity::isSupport(GPUExtension_Renderbuffer_RGB8_RGBA8)) {
            formats[PixelFormat_RGB888] = GL_RGB565;
            formats[PixelFormat_RGBA8888] = GL_RGB5_A1;
        }

        // フォーマットを取得する
        GLenum format = formats[pixelFormat];

        // カラーバッファを生成する
        MRenderBufferObject render(new RenderBufferObject(device, formats[pixelFormat]));

        // カラーとしてアタッチする
        attach(device->getState(), GL_COLOR_ATTACHMENT0, render);
    }

    /**
     * 深度バッファを確保する
     * 深度バッファの大きさは可能な限り大きいものを取得する
     *
     * @param bits 確保したい深度ビット数 32/24/16で指定する。それ以外は必ず16bitで指定される。拡張機能が利用できない場合、16bitで指定される。
     */
    virtual void allocDepthRenderbuffer(MDevice device, const s32 bits) {

        GLenum internalformat = 0;
        if (bits >= 32 && GPUCapacity::isSupport(GPUExtension_Renderbuffer_Depth32)) {
            internalformat = GL_DEPTH_COMPONENT32_OES;
        } else if (bits >= 24 && GPUCapacity::isSupport(GPUExtension_Renderbuffer_Depth24)) {
            internalformat = GL_DEPTH_COMPONENT24_OES;
        } else {
            internalformat = GL_DEPTH_COMPONENT16;
        }

        // 深度バッファを生成する
        MRenderBufferObject render(new RenderBufferObject(device, internalformat));

        attach(device->getState(), GL_DEPTH_ATTACHMENT, render);
    }

    /**
     * ステンシルバッファを確保する
     */
    virtual void allocStencilRenderbuffer(MDevice device) {
        // 深度バッファを生成する
        MRenderBufferObject render(new RenderBufferObject(device, GL_STENCIL_INDEX8));
        attach(device->getState(), GL_STENCIL_ATTACHMENT, render);
    }

    /**
     * 深度ステンシルを確保する
     * パックが可能な場合、パッキングしたバッファを取得する。
     * パックが出来ない場合は個々にバッファを確保する。その場合、両者の互換性を保つためにD24bitを要求する。
     */
    virtual void allocDepthAndStencil(MDevice device) {

        if (GPUCapacity::isSupport(GPUExtension_Renderbuffer_PackedDepth24Stencil8)) {
            // パックが可能なので、同一バッファを割り当てる
            MRenderBufferObject render(new RenderBufferObject(device, GL_DEPTH24_STENCIL8_OES));

            attach(device->getState(), GL_DEPTH_ATTACHMENT, render);
            attach(device->getState(), GL_STENCIL_ATTACHMENT, render);
        } else {
            // パックが出来ないので、各自に確保する
            allocDepthRenderbuffer(device, 24);
            allocStencilRenderbuffer(device);
        }
    }

};

}
}

#endif /* FRAMEBUFFEROBJECT_HPP_ */
