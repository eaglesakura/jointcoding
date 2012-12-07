/*
 * ndkGLNativeSurfaceViewContext.cpp
 *
 *  Created on: 2012/09/20
 */
#include    "ndkGLNativeSurfaceViewContext.h"
#include    <vector>
#include    "ndkEGLImpl.h"
#include    "ndkEGLDeviceUtil.h"

namespace ndk {

GLNativeSurfaceViewContext::GLNativeSurfaceViewContext(const u32 eglFlags) {
    this->initialized = jcfalse;
    this->eglFlags = eglFlags;
    this->device = EGLDeviceUtil::createInstance();
    this->pixelFormat = 0;
    this->width = 0;
    this->height = 0;

    // デフォルトディスプレイで設定する
    // デフォルトディスプレイはEGLが初期化してくれているから、特に追加初期化を行う必要はない
    this->display = EGLManager::getDefaultDisplay();

    jclogf("new context %x", this);
}

GLNativeSurfaceViewContext::~GLNativeSurfaceViewContext() {
    jclogf("delete context %x", this);
}

/**
 * レンダリング用コンフィグ情報を取得する
 */
EGLConfig GLNativeSurfaceViewContext::getConfig() {

    PixelFormat_e format = PixelFormat_RGBA8888;
    if (pixelFormat == PixelFormatProtocol::ANDROID_PIXELFORMAT_RGB565) {
        format = PixelFormat_RGB565;
    } else if (pixelFormat == PixelFormatProtocol::ANDROID_PIXELFORMAT_RGB888) {
        format = PixelFormat_RGB888;
    }

    return EGLManager::chooseConfig(display, format, jctrue, jctrue);
}

/**
 * 初期化を行う
 */
jcboolean GLNativeSurfaceViewContext::initialize() {
    if (initialized) {
        return true;
    }

    this->config = getConfig();
    if (this->config == NULL) {
        // 要件を満たすコンフィグの抽出に失敗するのは異常事態
        throw create_exception_t(EGLException, EGLException_ConfigChooseFailed);
    }

    const EGLint attr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext context = eglCreateContext(display, config, NULL, attr);
    if (context == EGL_NO_CONTEXT ) {
        // コンテキストが作成できないのは異常事態
        throw create_exception_t(EGLException, EGLException_CreateContextFailed);
    }

    // コンテキストを作成する
    {
        jc_sp< EGLContextProtocol > contextProtocol( new ndk::EGLContextManager(context, display, config) );
        this->device->setContext(contextProtocol);
    }

    initialized = jctrue;
    return jctrue;
}

/**
 * サーフェイスが作成された
 */
void GLNativeSurfaceViewContext::onSurfaceCreated(jobject surface) {
    jclogf("surface created %x", surface);
    // 廃棄フラグを消して、レンダリング可能な状態に戻す
    getDevice()->removeFlags(DeviceFlag_RequestDestroy);

}

/**
 * サーフェイス状態が変更になった
 */
void GLNativeSurfaceViewContext::onSurfaceChanged(jobject surface, const s32 pixelFormat, const s32 width, const s32 height) {
    MutexLock _lock(getDevice()->getGPUMutex()); // GPUアクセス中のロックを得ておく

    // 廃棄フラグを消して、レンダリング可能な状態に戻す
    getDevice()->removeFlags(DeviceFlag_RequestDestroy);

    jclogf("surface changed %x (W=%d, H=%d)", surface, width, height);
    this->pixelFormat = pixelFormat;
    this->width = width;
    this->height = height;

    // コンテキストの初期化を行う
    // 実際には一度しか実行されず、二回目以降は何もしない
    if (!this->initialize()) {
        jclogf("initialize failed... this(%x)", this);
    } else {
        jclogf("initialize completed this(%x)", this);
    }

    // EGLSurfaceの再構築を行う
    {
        CALL_JNIENV();
        ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
        jclogf("native window = %x", window);

        // ディスプレイサーフェイスを作成
        EGLSurface eglSurface = eglCreateWindowSurface(display, config, window, NULL);
        if (surface == EGL_NO_SURFACE ) {
            jclog("error surface");
            throw create_exception_t(EGLException, EGLException_SurfaceCreateFailed);
        }

        jclogf("window surface = %x", eglSurface);

        // サーフェイスを設定する
        this->device->setSurface(MEGLSurfaceProtocol(new EGLSurfaceManager(display, eglSurface)));
    }
}

/**
 * サーフェイスが廃棄された
 */
void GLNativeSurfaceViewContext::onSurfaceDestroyed(jobject surface) {
    // デバイスに廃棄フラグを追加してからロックを行わせる
    if (device) {
        device->addFlags(DeviceFlag_RequestDestroy);
    }

    MutexLock _lock(getDevice()->getGPUMutex()); // GPUアクセス中のロックを得ておく
    jclogf("surface destroyed %x", surface);

    MEGLSurfaceProtocol eglSurface = device->getSurface();
    if (eglSurface) {
        eglSurface->dispose();

        // デバイスにセットされているサーフェイスをリセットする
        device->setSurface(EGL_NULL_SURFACE);
    }
}

void GLNativeSurfaceViewContext::dispose() {
    // デバイスに廃棄フラグを追加してからロックを行わせる
    if (device) {
        device->addFlags(DeviceFlag_RequestDestroy);
    }

    MutexLock _lock(getDevice()->getGPUMutex()); // GPUアクセス中のロックを得ておく

    if (device) {
        device->dispose();
        device.reset();
    }
}

}
