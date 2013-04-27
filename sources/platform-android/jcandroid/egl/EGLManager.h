/*
 * ndkEGLManager.h
 *
 *  Created on: 2012/07/21

 */

#ifndef NDKEGLMANAGER_H_
#define NDKEGLMANAGER_H_

#include "EGLImpl.h"

namespace ndk {
using namespace jc;
using namespace jc::gl;


/**
 * EGLの初期化・終了処理を受け持つ。
 */
class EGLManager: public EGLProtocol {
    /**
     * ディスプレイ
     */
    EGLDisplay display;

    /**
     * アタッチされているスレッドID
     */
    MThreadID threadId;

    struct {
        /**
         * OSが保持していたDisplay
         */
        EGLDisplay display;

        /**
         * OSが保持していたContext
         */
        EGLContext context;

        /**
         * OSが保持していたSurface
         */
        EGLSurface readSurface;

        /**
         * OSが保持していたSurface
         */
        EGLSurface drawSurface;
    } androidEGL;

public:
    EGLManager();

    virtual ~EGLManager();

    virtual EGLDisplay getDisplay() const {
        return display;
    }

    /**
     * アタッチ済みのスレッドのIDを取得する。
     */
    virtual MThreadID getThreadID() const {
        return threadId;
    }

    /**
     * ステータスを取得する
     */
    virtual EGLStatus_e getStatus() const;

    /**
     * 指定したコンテキストとサーフェイスをカレントスレッドに割り当てる。
     */
    virtual void current(jc_sp<EGLContextProtocol> context, jc_sp<EGLSurfaceProtocol> surface);

    /**
     * 描画用のディスプレイにバックバッファの内容を転送する。
     * 転送方法については各デバイスの実装に従う。
     * このメソッドは完了するまでブロックされることを保証しない（デバイス依存、基本的にブロッキングになるように調整している）
     */
    virtual jcboolean postFrontBuffer(MEGLSurfaceProtocol displaySurface);

    /**
     * 現在のEGLパラメーターを一時的に格納する
     */
    virtual void stashEGLCurrents();

    /**
     * 資源の開放を行う
     */
    virtual void dispose();

    /**
     * 最も適切なEGLConfigを取得する
     */
    static EGLConfig chooseConfig( const EGLDisplay display, const PixelFormat_e pixelFormat, const jcboolean hasDepth, const jcboolean hasStencil);

    /**
     * eglGetDisplay(EGL_DEFAULT_DISPLAY)を呼び出す
     */
    static EGLDisplay getDefaultDisplay();
};

}

#endif /* NDKEGLMANAGER_H_ */
