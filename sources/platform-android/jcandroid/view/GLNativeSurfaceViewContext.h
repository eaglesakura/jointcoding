/*
 * ndkGLNativeSurfaceViewContext.h
 *
 *  Created on: 2012/09/20
 */

#ifndef NDKGLNATIVESURFACEVIEWCONTEXT_H_
#define NDKGLNATIVESURFACEVIEWCONTEXT_H_

#include    "jointcoding-android.h"
#include    "android-classes/ndkGLNativeSurfaceView.h"
#include    "jcandroid/egl/EGLImpl.h"
#include    "jc/gl/Device.h"

namespace ndk {

/**
 * GLNativeSurfaceView用のコンテキスト。
 * 内部ではDeviceを管理している。
 */
class GLNativeSurfaceViewContext: public Object {
    /**
     * EGLデバイス
     */
    MDevice device;

    /**
     * EGL初期化フラグ
     */
    u32 eglFlags;

    /**
     * レンダリング用コンフィグ情報を取得する
     */
    EGLConfig getConfig();

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
     * 初期化を行う
     */
    virtual jcboolean initialize();

    /**
     * サーフェイスのピクセルフォーマット
     */
    s32 pixelFormat;

    /**
     * サーフェイスの幅
     */
    s32 width;

    /**
     * サーフェイスの高さ
     */
    s32 height;

public:
    GLNativeSurfaceViewContext(const u32 eglFlags);
    virtual ~GLNativeSurfaceViewContext();

    /**
     * サーフェイスが作成された
     */
    virtual void onSurfaceCreated(jobject surface);

    /**
     * サーフェイス状態が変更になった
     */
    virtual void onSurfaceChanged(jobject surface, const s32 pixelFormat, const s32 width, const s32 height);

    /**
     * サーフェイスが廃棄された
     */
    virtual void onSurfaceDestroyed(jobject surface);

    /**
     * コンテキストの廃棄作業を行う
     */
    virtual void dispose();

    /**
     * レンダリング用デバイスを取得する。
     */
    virtual MDevice getDevice() {
        return  device;
    }

    /**
     * Context変換
     */
    static jc_sp<GLNativeSurfaceViewContext>& getNativeContext(jobject glNativeSurfaceView) {
        return *((jc_sp<GLNativeSurfaceViewContext>*)GLNativeSurfaceView::getNativeContextPointer(glNativeSurfaceView));
    }
};

}

#endif /* NDKGLNATIVESURFACEVIEWCONTEXT_H_ */
