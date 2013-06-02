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

    /**
     * 焼きこみ先色テクスチャ
     */
    MTextureImage colorTexture;

    /**
     * 焼きこみ先深度テクスチャ
     */
    MTextureImage depthTexture;
public:
    FrameBufferObject(MDevice device) {
        assert(device);

        fbo = device->getVRAM()->alloc(VRAM_FrameBuffer);
        assert(fbo.get());
    }

    virtual ~FrameBufferObject() {
    }

    /**
     * 管理名を取得する
     */
    virtual GLuint getName() const {
        return fbo.get();
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
    virtual void allocDepthRenderbuffer(MDevice device, const s32 bits, const s32 width, const s32 height, const jcboolean withTexture) {

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
        render->resize(device->getState(), width, height);

        if (withTexture) {
            render->bind(device->getState());

            depthTexture.reset(new TextureImage(GL_TEXTURE_2D, width, height, device));
            depthTexture->bind(device->getState());
            if (GPUCapacity::isSupport(GPUExtension_Texture_Depth)) {
                jclog("support depthtexture");
                // 深度テクスチャの有無によってランタイムで切り分ける
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
                assert_gl();
            } else if (GPUCapacity::isSupport(GPUExtension_Texture_HalfFloat)) {
                jclog("not support depthtexture | support halffloat");
                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_HALF_FLOAT_OES, NULL);
                assert_gl();
            } else {
                jclog("not support depthtexture | not support halffloat");
                // 何らかの復帰手段が必要
                assert(false);
            }

            depthTexture->onAllocated();
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getName(), 0);
                assert_gl();
            }
            depthTexture->unbind();
        }
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
    virtual void allocDepthAndStencil(MDevice device, const s32 width, const s32 height, const jcboolean withTexture) {

        if (GPUCapacity::isSupport(GPUExtension_Renderbuffer_PackedDepth24Stencil8)) {
            // パックが可能なので、同一バッファを割り当てる
            MRenderBufferObject render(new RenderBufferObject(device, GL_DEPTH24_STENCIL8_OES));

            attach(device->getState(), GL_DEPTH_ATTACHMENT, render);
            attach(device->getState(), GL_STENCIL_ATTACHMENT, render);
        } else {
            // パックが出来ないので、各自に確保する
            allocDepthRenderbuffer(device, 24, width, height, withTexture);
            allocStencilRenderbuffer(device);
        }
    }

    /**
     * レンダリングターゲットの幅を取得する
     */
    virtual u32 getWidth() const {
        if (color) {
            return color->getWidth();
        } else if (depth) {
            return depth->getWidth();
        } else if (stencil) {
            return stencil->getWidth();
        }
        assert(false);
        return 0;
    }

    /**
     * レンダリングターゲットの幅を取得する
     */
    virtual u32 getHeight() const {
        if (color) {
            return color->getHeight();
        } else if (depth) {
            return depth->getHeight();
        } else if (stencil) {
            return stencil->getHeight();
        }
        assert(false);
        return 0;
    }

    /**
     * レンダリングターゲット用のテクスチャを生成する
     * テクスチャとしてバインドするターゲットを指定する
     */
    virtual void allocColorRenderTexture(MDevice device, const PixelFormat_e texturePixelFormat) {
        assert(color);

        bind(device->getState());

        colorTexture.reset(new TextureImage(GL_TEXTURE_2D, (s32) color->getWidth(), (s32) color->getHeight(), device));
        colorTexture->bind(device->getState());
        colorTexture->allocPixelMemory(texturePixelFormat, 0);
        {
            color->bind(device->getState());
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->getName(), 0);
            assert_gl();
            color->unbind(device->getState());
        }
        colorTexture->unbind();
    }

    /**
     * レンダリングターゲット用の深度テクスチャを生成する
     * テクスチャとしてバインドするターゲットを指定する
     */
    virtual void allocDepthRenderTexture(MDevice device) {
        assert(depth);

        bind(device->getState());

        depthTexture.reset(new TextureImage(GL_TEXTURE_2D, (s32) depth->getWidth(), (s32) depth->getHeight(), device));
        depthTexture->bind(device->getState());
        if (GPUCapacity::isSupport(GPUExtension_Texture_Depth)) {
            jclog("support depthtexture");
            // 深度テクスチャの有無によってランタイムで切り分ける
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depth->getWidth(), depth->getHeight(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
            assert_gl();
        } else if (GPUCapacity::isSupport(GPUExtension_Texture_HalfFloat)) {
            jclog("not support depthtexture | support halffloat");
            // FIXME for Tegra3
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, depth->getWidth(), depth->getHeight(), 0, GL_LUMINANCE, GL_HALF_FLOAT_OES, NULL);
            assert_gl();
        } else {
            jclog("not support depthtexture | not support halffloat");
            depthTexture->unbind();
            depthTexture.reset();
            return;
        }

        depthTexture->onAllocated();
        {
            depth->bind(device->getState());
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getName(), 0);
            assert_gl();
            depth->unbind(device->getState());
        }
        depthTexture->unbind();
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
