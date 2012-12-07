/*
 * ndkGLNativeTextureViewContext.h
 *
 *  Created on: 2012/12/05
 */

#ifndef NDKGLNATIVETEXTUREVIEWCONTEXT_H_
#define NDKGLNATIVETEXTUREVIEWCONTEXT_H_

#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"
#include  "ndkEGLImpl.h"
#include "jcglDevice.h"

namespace ndk {

/**
 * GLNativeSurfaceView用のコンテキスト。
 * 内部ではDeviceを管理している。
 */
class GLNativeTextureViewContext: public Object {
    /**
     * EGLデバイス
     */
    MDevice device;

    /**
     * EGL初期化フラグ
     */
    u32 eglFlags;

    /**
     * コンテキスト作成用のconfig
     */
    EGLConfig config;

    /**
     * EGLと関連付けたDisplay
     */
    EGLDisplay display;

    /**
     * 初期化済みならtrue
     */
    jcboolean initialized;

    /**
     * サーフェイスの幅
     */
    s32 width;

    /**
     * サーフェイスの高さ
     */
    s32 height;

public:
    GLNativeTextureViewContext(const u32 eglFlags);
    virtual ~GLNativeTextureViewContext();

    /**
     * サーフェイスが作成された
     */
    virtual void onGLInitialize(jobject surfaceTexture);

    /**
     * サーフェイス状態が変更になった
     */
    virtual void onSurfaceSizeChanged(jobject surfaceTexture, const s32 width, const s32 height);

    /**
     * サーフェイスを休止する
     */
    virtual void onGLSuspend();

    /**
     * コンテキストの廃棄作業を行う
     */
    virtual void dispose();

    /**
     * レンダリング用デバイスを取得する。
     */
    virtual MDevice getDevice() {
        return device;
    }

    /**
     * Context変換
     */
    static jc_sp<GLNativeTextureViewContext>& getNativeContext(jobject glNativeSurfaceView) {
        return *((jc_sp<GLNativeTextureViewContext>*)GLNativeTextureView::getNativeContextPointer_(glNativeSurfaceView));
    }
};

}

#endif /* NDKGLNATIVETEXTUREVIEWCONTEXT_H_ */
