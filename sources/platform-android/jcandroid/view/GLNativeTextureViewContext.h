/*
 * ndkGLNativeTextureViewContext.h
 *
 *  Created on: 2012/12/05
 */

#ifndef NDKGLNATIVETEXTUREVIEWCONTEXT_H_
#define NDKGLNATIVETEXTUREVIEWCONTEXT_H_

#include    "jointcoding-android.h"
#include    "android-classes/ndkGLNativeTextureView.h"
#include    "jcandroid/egl/ndk/EGLImpl.h"
#include    "jc/gl/gpu/Device.h"

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

    /**
     * 強制ロック
     */
    jc_sp<DeviceLock> device_lock;

    /**
     * 画面が死んでる間用の仮サーフェイス
     */
    MEGLSurfaceProtocol pbufferSurface;
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
    static GLNativeTextureViewContext* getNativeContext(jobject glNativeTextureView) {
        assert(glNativeTextureView != NULL);
        return getJointContextPointer<GLNativeTextureViewContext>(glNativeTextureView, 0);
    }

    /**
     * デバイスを取得する
     */
    static MDevice getDevice(jobject jGLNativeTextureView) {
        GLNativeTextureViewContext *context = getNativeContext(jGLNativeTextureView);
        assert(context != NULL);
        return context->getDevice();
    }
};

}

#endif /* NDKGLNATIVETEXTUREVIEWCONTEXT_H_ */
