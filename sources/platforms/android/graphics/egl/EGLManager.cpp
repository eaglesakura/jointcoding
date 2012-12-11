/*
 * EGLManager.cpp
 *
 *  Created on: 2012/07/21

 */

#include    "jcException.h"
#include    "ndkEGLImpl.h"
#include    <vector>
#include    "ndkNativeContext.h"

using namespace jc;
using namespace jc::gl;

namespace ndk {

EGLManager::EGLManager() {
    display = getDefaultDisplay();

    {
        jclogf("egl display = %x", display);
        EGLint major, minor;
        eglInitialize(display, &major, &minor);
        jclogf("EGL Version = %d.%d", major, minor);
    }
}

EGLManager::~EGLManager() {
    this->dispose();
}

/**
 * ステータスを取得する
 */
EGLStatus_e EGLManager::getStatus() const {
    // スレッドIDを得ていなければ、どのスレッドにも属していない
    if (!threadId) {
        return EGLStatus_Ready;
    }

    // 現在のスレッドIDを取得
    ThreadID current_id;

    // 同一スレッドだったらAttached、それ以外のスレッドだったらBusy
    return current_id.equals(threadId) ? EGLStatus_Attached : EGLStatus_Busy;
}

/**
 * 指定したコンテキストとサーフェイスをカレントスレッドに割り当てる。
 */
void EGLManager::current(jc_sp<EGLContextProtocol> context, jc_sp<EGLSurfaceProtocol> surface) {
    if( context.get() && surface.get() ) {
//        this->stashEGLCurrents();
        EGLContextManager *contextManager = dynamic_cast<EGLContextManager*>(context.get());
        EGLSurfaceManager *surfaceManager = dynamic_cast<EGLSurfaceManager*>(surface.get());

        EGLDisplay eglDisplay = contextManager->getDisplay();
        EGLContext eglContext = contextManager->getContext();
        EGLSurface eglSurface = surfaceManager->getSurface();

// カレントに設定できなければ例外を投げる
        if( !eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) ) {
            throw create_exception_t(EGLException, EGLException_ContextAttachFailed);
        }
    } else {
        EGLDisplay eglDisplay = display;
        EGLSurface eglReadSurface = EGL_NO_SURFACE;
        EGLSurface eglDrawSurface = EGL_NO_SURFACE;
        EGLContext eglContext = EGL_NO_CONTEXT;

#if 0
        if( NativeContext::isUIThread() ) {
            jclog("egl -> UIThread");
            eglDisplay = androidEGL.display;
            eglContext = androidEGL.context;
            eglReadSurface = androidEGL.readSurface;
            eglDrawSurface = androidEGL.drawSurface;
        }
#endif

        // コンテキストとサーフェイスが揃っていないから、設定できない
        if( !eglMakeCurrent(eglDisplay, eglDrawSurface, eglReadSurface, eglContext) ) {
            throw create_exception_t(EGLException, EGLException_ContextAttachFailed);
        }
    }

}

/**
 * 描画用のディスプレイにバックバッファの内容を転送する。
 * 転送方法については各デバイスの実装に従う。
 * このメソッドは完了するまでブロックされる。
 */
jcboolean EGLManager::postFrontBuffer(MEGLSurfaceProtocol displaySurface) {
    EGLSurfaceManager *surfaceManager = dynamic_cast<EGLSurfaceManager*>(displaySurface.get());
    return eglSwapBuffers(display, surfaceManager->getSurface());
}
/**
 * 現在のEGLパラメーターを一時的に格納する
 */
void EGLManager::stashEGLCurrents() {
    androidEGL.display = eglGetCurrentDisplay();
    androidEGL.context = eglGetCurrentContext();
    androidEGL.readSurface = eglGetCurrentSurface(EGL_READ);
    androidEGL.drawSurface = eglGetCurrentSurface(EGL_DRAW);

    jclogf("Android EGLDisplay(%x)", androidEGL.display);
    jclogf("Android EGLContext(%x)", androidEGL.context);
    jclogf("Android EGLSurface-Read(%x)", androidEGL.readSurface);
    jclogf("Android EGLSurface-Draw(%x)", androidEGL.drawSurface);
}

/**
 * 資源の開放を行う
 */
void EGLManager::dispose() {
    if (display) {
        eglTerminate(display);
    }
    display = NULL;
}

/**
 * 最も適切なEGLConfigを取得する
 */
EGLConfig EGLManager::chooseConfig(const EGLDisplay display, const PixelFormat_e pixelFormat, const jcboolean hasDepth, const jcboolean hasStencil) {
    std::vector<EGLint> specs;

    s32 red_bits = 0;
    s32 blue_bits = 0;
    s32 green_bits = 0;
    s32 alpha_bits = 0;
    s32 depth_bits = 0;
    s32 stencis_bits = 0;

    if (pixelFormat == PixelFormat_RGB565) {
        specs.push_back(EGL_RED_SIZE);
        specs.push_back(5);
        specs.push_back(EGL_GREEN_SIZE);
        specs.push_back(6);
        specs.push_back(EGL_BLUE_SIZE);
        specs.push_back(5);

        red_bits = 5;
        green_bits = 6;
        blue_bits = 5;
    } else if (pixelFormat == PixelFormat_RGB888) {
        specs.push_back(EGL_RED_SIZE);
        specs.push_back(8);
        specs.push_back(EGL_GREEN_SIZE);
        specs.push_back(8);
        specs.push_back(EGL_BLUE_SIZE);
        specs.push_back(8);

        red_bits = 8;
        green_bits = 8;
        blue_bits = 8;
    } else if (pixelFormat == PixelFormat_RGBA8888) {
        specs.push_back(EGL_RED_SIZE);
        specs.push_back(8);
        specs.push_back(EGL_GREEN_SIZE);
        specs.push_back(8);
        specs.push_back(EGL_BLUE_SIZE);
        specs.push_back(8);
        specs.push_back(EGL_ALPHA_SIZE);
        specs.push_back(8);

        red_bits = 8;
        green_bits = 8;
        blue_bits = 8;
        alpha_bits = 8;
    }

    // 深度バッファ
    if (hasDepth) {
        specs.push_back(EGL_DEPTH_SIZE);
        specs.push_back(16);

        depth_bits = 16;
    }
    // ステンシルバッファ
    if (hasStencil) {
        specs.push_back(EGL_STENCIL_SIZE);
        specs.push_back(8);
        stencis_bits = 8;
    }

    // ウィンドウタイプ
    {
        specs.push_back(EGL_SURFACE_TYPE);
        specs.push_back(EGL_WINDOW_BIT);
    }

    // GL ES 2.0で初期化する
    {
        specs.push_back(EGL_RENDERABLE_TYPE);
        specs.push_back(EGL_OPENGL_ES2_BIT);
    }

    // 閉じる
    specs.push_back(EGL_NONE);

    // スペックを取り出す
    const EGLint MAX_CONFIG = 32;
    EGLConfig configs[MAX_CONFIG] = { NULL };
    EGLint numConfig;
    eglChooseConfig((void*) display, (const int*) &specs[0], (void**) configs, MAX_CONFIG, &numConfig);

    // 最も適切なスペックを選択する
    for (int i = 0; i < numConfig; ++i) {
        const EGLConfig cf = configs[i];

        s32 cfRedSize = 0;
        s32 cfGreenSize = 0;
        s32 cfBlueSize = 0;
        s32 cfAlphaSize = 0;
        s32 cfDepthSize = 0;
        s32 cfStencilSize = 0;

        // 各サイズを抽出する
        eglGetConfigAttrib(display, cf, EGL_RED_SIZE, &cfRedSize);
        eglGetConfigAttrib(display, cf, EGL_GREEN_SIZE, &cfGreenSize);
        eglGetConfigAttrib(display, cf, EGL_BLUE_SIZE, &cfBlueSize);
        eglGetConfigAttrib(display, cf, EGL_ALPHA_SIZE, &cfAlphaSize);
        eglGetConfigAttrib(display, cf, EGL_DEPTH_SIZE, &cfDepthSize);
        eglGetConfigAttrib(display, cf, EGL_STENCIL_SIZE, &cfStencilSize);

        // 全スペックが最も一致するものを返す
        if (cfDepthSize >= depth_bits && cfStencilSize >= stencis_bits && cfRedSize == red_bits && cfGreenSize == green_bits && cfBlueSize == blue_bits && cfAlphaSize == alpha_bits) {
            jclogf("match config = index[%d]", i);
            return cf;
        }
    }

    return NULL;
}

/**
 * eglGetDisplay(EGL_DEFAULT_DISPLAY)を呼び出す
 */
EGLDisplay EGLManager::getDefaultDisplay() {
    return eglGetDisplay(EGL_DEFAULT_DISPLAY );
}

}

