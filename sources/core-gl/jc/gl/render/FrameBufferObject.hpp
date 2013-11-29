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
#include    "jc/gl/texture/TextureImage.h"

namespace jc {
namespace gl {

/**
 * オフスクリーンレンダリング用フレームバッファを管理する
 *
 * IRenderingSurface : レンダリング対象のサーフェイスとして機能する
 */
class FrameBufferObject: public Object, public IRenderingSurface {
protected:
    /**
     * フレームバッファ
     */
    GLObject framebuffer;

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

    /**
     * 焼きこみ先色テクスチャ
     */
    MTextureImage colorTexture;

    /**
     * 焼きこみ先深度テクスチャ
     */
    MTextureImage depthTexture;

    /**
     * バッファ幅
     */
    s32 width;

    /**
     * バッファ高
     */
    s32 height;

public:
    FrameBufferObject(MDevice device) {
        assert(device);

        width = height = 0;
        framebuffer.alloc(device->getVRAM(VRAM_FrameBuffer));
        assert(framebuffer);
    }

    virtual ~FrameBufferObject() {
    }

    /**
     * 管理名を取得する
     */
    virtual GLuint getName() const {
        return framebuffer.get();
    }

    virtual const GLObject& getObject() const {
        return framebuffer;
    }

    /**
     * バインドを行う
     */
    virtual void bind(MGLState state) {
        assert(framebuffer);

        state->bindFramebuffer(GL_FRAMEBUFFER, framebuffer.get());
        assert(glIsFramebuffer(framebuffer.get()));
    }

    /**
     * バインドされていたら、バインド解除を行う
     */
    virtual void unbind(MGLState state) {
        if (state->isBindedFramebuffer(framebuffer.get())) {
            state->bindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        if (color) {
            color->unbind(state);
        }

        if (depth) {
            depth->unbind(state);
        }

        if (stencil) {
            stencil->unbind(state);
        }
    }

    /**
     * @param attachment  GL_DEPTH_ATTACHMENT
     */
    virtual void attach(MGLState state, const GLenum attachment, MTextureImage texture) {
        bind(state);

        texture->bind(state);
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->getName(), 0);
            assert_gl();
        }
        texture->unbind(state);


        // アタッチメントの種類によって保存先を変更する
        switch (attachment) {
            case GL_COLOR_ATTACHMENT0:
                this->colorTexture = texture;
                break;
            case GL_DEPTH_ATTACHMENT:
                this->depthTexture = texture;
                break;
            default:
                assert(false);
                break;
        }
    }

    /**
     * フレームバッファへレンダリングバッファを関連付ける
     * @param attachiment GL_COLOR_ATTACHIMENT/GL_DEPTH_ATTACHMENT/GL_STENCIL_ATTACHMENT
     */
    virtual void attach(MGLState state, const GLenum attachiment, MRenderBufferObject renderbuffer) {
        bind(state);
        renderbuffer->bind(state);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachiment, GL_RENDERBUFFER, renderbuffer->getName());
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
            default:
                assert(false);
                break;
        }

        // リサイズの必要があるならリサイズを行わせる
        if (width > 0 && height > 0) {
            renderbuffer->resize(state, width, height);
        }
    }

    /**
     * カラーアタッチメントを持っているならtrue
     */
    virtual jcboolean hasColorAttachiment() const {
        return (bool) color;
    }

    virtual MRenderBufferObject getColorBuffer() const {
        return color;
    }

    virtual MTextureImage getColorTexture() const {
        return colorTexture;
    }

    /**
     * 深度アタッチメントを持っているならtrue
     */
    virtual jcboolean hasDepthAttachiment() const {
        return (bool) depth;
    }

    virtual MRenderBufferObject getDepthBuffer() const {
        return depth;
    }

    virtual MTextureImage getDepthTexture() const {
        return depthTexture;
    }

    /**
     * ステンシルアタッチメントを持っているならtrue
     */
    virtual jcboolean hasStencilAttachiment() const {
        return (bool) stencil;
    }

    virtual MRenderBufferObject getStencilBuffer() const {
        return stencil;
    }

    /**
     * フレームバッファの2Dサイズをリサイズする
     * アタッチされていないバッファについては何もしない
     */
    virtual void resize(MGLState state, const u32 width, const u32 height) {
        assert(state);

        this->width = width;
        this->height = height;

        if (color) {
            color->resize(state, width, height);
        }

        if (depth) {
            depth->resize(state, width, height);
        }

        if (stencil) {
            stencil->resize(state, width, height);
        }
    }

    /**
     * カラーレンダリングバッファを確保する
     */
    virtual void allocColorRenderbuffer(MDevice device, const PixelFormat_e pixelFormat) {
        assert(pixelFormat != PixelFormat_BGRA8888);

        // カラーフォーマットテーブルを用意しておく
        GLenum formats[] = {
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
            jclogf("alloc depth req(%d) -> D(32bit)", bits);
        } else if (bits >= 24 && GPUCapacity::isSupport(GPUExtension_Renderbuffer_Depth24)) {
            internalformat = GL_DEPTH_COMPONENT24_OES;
            jclogf("alloc depth req(%d) -> D(24bit)", bits);
        } else {
            internalformat = GL_DEPTH_COMPONENT16;
            jclogf("alloc depth req(%d) -> D(16bit)", bits);
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

    /**
     * レンダリングターゲットの幅を取得する
     */
    virtual s32 getWidth() const {
        return width;
    }

    /**
     * レンダリングターゲットの幅を取得する
     */
    virtual s32 getHeight() const {
        return height;
    }

    /**
     * レンダリングターゲットのアスペクト比を取得する
     */
    virtual float getAspect() const {
        return (float) getWidth() / (float) getHeight();
    }

    /**
     * レンダリングターゲット用のテクスチャを生成する
     * テクスチャとしてバインドするターゲットを指定する
     */
    virtual void allocColorRenderTexture(MDevice device, const PixelFormat_e texturePixelFormat) {
        assert(width > 0);
        assert(height > 0);

        bind(device->getState());

        colorTexture.reset(new TextureImage(GL_TEXTURE_2D, width, height, device));
        MGLState state = device->getState();
        colorTexture->bind(state);
        colorTexture->allocPixelMemory(texturePixelFormat, 0, state);
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->getName(), 0);
            assert_gl();
        }
        colorTexture->unbind(state);
    }

    /**
     * レンダリングターゲット用の深度テクスチャを生成する
     * テクスチャとしてバインドするターゲットを指定する
     *
     * 注）
     * 現行機ではDepthTextureがサポートされていない端末が存在する(Tegra系）
     * その場合、このメソッドはfalseを返して何も行わない。
     * 基本的にこの呼出は非推奨となる。depthを明示的に記録する場合（シャドウマップ等）はcolorbufferで適当な代用を行うこと。
     */
    virtual jcboolean allocDepthRenderTexture(MDevice device) {
        if (!GPUCapacity::isSupport(GPUExtension_Texture_Depth)) {
            return jcfalse;
        }

        bind(device->getState());

        depthTexture.reset(new TextureImage(GL_TEXTURE_2D, width, height, device));
        depthTexture->bind(device->getState());
        {
            // 深度テクスチャがサポートされているため、深度として直接関連付けられる
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
            assert_gl();

            // 深度設定
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getName(), 0);
            assert_gl();
        }
        depthTexture->onAllocated();
        depthTexture->unbind(device->getState());
        return jctrue;
    }

    /**
     * レンダリングデバイスを移譲する場合に呼び出す必要がある
     * このメソッドを呼び出さない場合、Nexus10で問題が生じる場合がある。
     */
    virtual void setRenderDevice(MDevice device) {
        MGLState state = device->getState();
        bind(state);

        if(colorTexture) {
            attach(state, GL_COLOR_ATTACHMENT0, colorTexture);
        }
        if (depthTexture) {
            attach(state, GL_DEPTH_ATTACHMENT, depthTexture);
        }
    }

    /**
     * フレームバッファが正常にであることを検証する
     */
    virtual void checkFramebufferStatus() {
#ifdef DEBUG
        GLint check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (check != GL_FRAMEBUFFER_COMPLETE) {
            jclogf("glCheckFramebufferStatus(0x%x)", check);
        }
        assert(check == GL_FRAMEBUFFER_COMPLETE);
#endif
    }
}
;

/**
 * managed
 */
typedef jc_sp<FrameBufferObject> MFrameBufferObject;

}
}

#endif /* FRAMEBUFFEROBJECT_HPP_ */
