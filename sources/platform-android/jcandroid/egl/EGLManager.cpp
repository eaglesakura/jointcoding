/*
 * EGLManager.cpp
 *
 *  Created on: 2012/07/21

 */

#include    "jc/system/Exception.h"
#include    "jcandroid/egl/EGLImpl.h"
#include    <vector>
#include    "android-classes/ndkNativeContext.h"
#include    "android-gclasses/EGLSupport.h"

using namespace jc;
using namespace jc::gl;

namespace ndk {

namespace {

#if defined(DEBUG) && !defined(NO_EGL_ASSERT)
#define     assert_egl(...)    { assert(printEGLError(__FILE__, __LINE__) == jcfalse); }
#else
// release
#define     assert_egl(...)    { }
#endif

/**
 * エラーを出力する
 * GL_NOERROR以外だったらjctrueを返す
 */
jcboolean printEGLError(const charactor* file, const s32 line, GLenum error) {
    if (error == EGL_SUCCESS) {
        return jcfalse;
    }
#define LOG_EGL( error_enum )    case error_enum: ::jc::__logDebugF(error_enum != EGL_SUCCESS ? LogType_Alert : LogType_Debug, ::jc::__getFileName(file), "L %d | %s", line, #error_enum); return error != EGL_SUCCESS ? jctrue : jcfalse;
    switch (error) {
        LOG_EGL(EGL_NOT_INITIALIZED);
        LOG_EGL(EGL_BAD_ACCESS);
        LOG_EGL(EGL_BAD_ALLOC);
        LOG_EGL(EGL_BAD_ATTRIBUTE);
        LOG_EGL(EGL_BAD_CONFIG);
        LOG_EGL(EGL_BAD_CONTEXT);
        LOG_EGL(EGL_BAD_CURRENT_SURFACE);
        LOG_EGL(EGL_BAD_DISPLAY);
        LOG_EGL(EGL_BAD_MATCH);
        LOG_EGL(EGL_BAD_NATIVE_PIXMAP);
        LOG_EGL(EGL_BAD_NATIVE_WINDOW);
        LOG_EGL(EGL_BAD_PARAMETER);
        LOG_EGL(EGL_BAD_SURFACE);
        LOG_EGL(EGL_CONTEXT_LOST);
    }

    jclogf("EGL unknown error = 0x%x", error);
    return jctrue;
#undef LOG_EGL
}

/**
 * GLのエラー出力を行う
 */
jcboolean printEGLError(const charactor* file, const s32 line) {
    return printEGLError(file, line, eglGetError());
}

}

EGLManager::EGLManager() {
    display = getDefaultDisplay();

    {
        jclogf("egl display = %x", display);
        EGLint major, minor;
        eglInitialize(display, &major, &minor);
        if (printEGLError(__FILE__, __LINE__)) {
            throw create_exception(RuntimeException, "eglInitialize Error");
        }
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
    if( context && surface ) {
        EGLContextManager *contextManager = dynamic_cast<EGLContextManager*>(context.get());
        EGLSurfaceManager *surfaceManager = dynamic_cast<EGLSurfaceManager*>(surface.get());

        EGLDisplay eglDisplay = contextManager->getDisplay();
        EGLContext eglContext = contextManager->getContext();
        EGLSurface eglSurface = surfaceManager ? surfaceManager->getSurface() : EGL_NO_SURFACE;

//        jclogf("request disp(%x) read(%x) draw(%d) ctx(%x)", eglDisplay, eglSurface, eglSurface, eglContext);

// カレントに設定できなければ例外を投げる
        if( !eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) ) {
//            printEGLError(__FILE__, __LINE__);
//            throw create_exception_t(EGLException, EGLException_ContextAttachFailed);
        }
        assert_egl();

//#ifdef  EGL_TRIPLEBUFFER_MODE
//        {
//            // トリプルバッファ対応させる
//            eglSwapInterval(display, 2);
//        }
//#endif

    } else {
        EGLDisplay eglDisplay = display;
        EGLSurface eglReadSurface = EGL_NO_SURFACE;
        EGLSurface eglDrawSurface = EGL_NO_SURFACE;
        EGLContext eglContext = EGL_NO_CONTEXT;

        jcboolean backToDefault = false;
#if 1
        if( NativeContext::isUIThread() ) {
//            jclog("egl -> UIThread");
            eglDisplay = androidEGL.display;
            eglContext = androidEGL.context;
            eglReadSurface = androidEGL.readSurface;
            eglDrawSurface = androidEGL.drawSurface;
            backToDefault = true;
        }
#endif
        // コンテキストとサーフェイスが揃っていないから、設定できない
        if( !eglMakeCurrent(eglDisplay, eglDrawSurface, eglReadSurface, eglContext) ) {
            if(backToDefault) {
                // zeroにも戻せない
                if(!eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) ) {
                    printEGLError(__FILE__, __LINE__);
                    throw create_exception_t(EGLException, EGLException_ContextAttachFailed);
                }
            } else {
                if(!eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) ) {
                    printEGLError(__FILE__, __LINE__);
                    EGLSupport::unlockEGLMakeCurrent((jint)eglDisplay, (jint)EGL_NO_SURFACE, (jint)EGL_NO_SURFACE, (jint)EGL_NO_CONTEXT);
                }
            }
        }
        assert_egl();
    }

}

/**
 * 描画用のディスプレイにバックバッファの内容を転送する。
 * 転送方法については各デバイスの実装に従う。
 * このメソッドは完了するまでブロックされる。
 */
jcboolean EGLManager::postFrontBuffer(MEGLSurfaceProtocol displaySurface) {
    EGLSurfaceManager *surfaceManager = dynamic_cast<EGLSurfaceManager*>(displaySurface.get());
    const EGLSurface targetSurface = surfaceManager->getSurface();
    const EGLSurface currentSurface = eglGetCurrentSurface(EGL_DRAW);
    if (currentSurface != targetSurface) {
        jclogf("current surface(%x) != app surface(%x)", currentSurface, targetSurface);
        return jcfalse;
    }
    jcboolean result = eglSwapBuffers(display, targetSurface);

    if (!result || printEGLError(__FILE__, __LINE__)) {
        jclogf("Bad Surface(%x)", targetSurface);
    }
    return result;
}
/**
 * 現在のEGLパラメーターを一時的に格納する
 */
void EGLManager::stashEGLCurrents() {
    androidEGL.display = eglGetCurrentDisplay();
    androidEGL.context = eglGetCurrentContext();
    androidEGL.readSurface = eglGetCurrentSurface(EGL_READ);
    androidEGL.drawSurface = eglGetCurrentSurface(EGL_DRAW);

    jclogf("Current EGLDisplay(%x) EGLSurface-Read(%x) EGLSurface-Draw(%x) EGLContext(%x)", androidEGL.display, androidEGL.readSurface, androidEGL.drawSurface, androidEGL.context);
}

/**
 * 資源の開放を行う
 */
void EGLManager::dispose() {
    if (display) {
        eglTerminate(display);
        printEGLError(__FILE__, __LINE__);
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
    } else {
        // pixelformat error
        assert(false);
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

    // コンフィグが正常に取得できなければならない
    assert(numConfig > 0);

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
        assert_egl();
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

