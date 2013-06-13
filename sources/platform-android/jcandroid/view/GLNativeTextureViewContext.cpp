/*
 * ndkGLNativeTextureViewContext.cpp
 *
 *  Created on: 2012/12/05
 */

#include    "jcandroid/view/GLNativeTextureViewContext.h"
#include    "jcandroid/egl/ndk/EGLDeviceUtil.h"
#include    "android-gclasses/EGLSupport.h"
#include    "jc/gl/gpu/DeviceLock.hpp"

namespace ndk {

GLNativeTextureViewContext::GLNativeTextureViewContext(const u32 eglFlags) {
    this->eglFlags = eglFlags;
    this->device = EGLDeviceUtil::createInstance();
    config = NULL;
    display = NULL;
    height = 0;
    width = 0;
    initialized = jcfalse;
}

GLNativeTextureViewContext::~GLNativeTextureViewContext() {

}

/**
 * サーフェイスが作成された
 */
void GLNativeTextureViewContext::onGLInitialize(jobject surfaceTexture) {
    if (initialized) {
        throw create_exception(RuntimeException, "TextureView is initialized!!");
    }

    if (!display) {
        display = EGLManager::getDefaultDisplay();
        if (!display) {
            throw create_exception_t(EGLException, EGLException_InitializeFailed);
        }
    }

    config = EGLManager::chooseConfig(display, PixelFormat_RGBA8888, jctrue, jctrue);

    if (config == NULL) {
        // 要件を満たすコンフィグの抽出に失敗するのは異常事態
        throw create_exception_t(EGLException, EGLException_ConfigChooseFailed);
    }

    const EGLint attr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext context = eglCreateContext(display, config, NULL, attr);
    if (context == EGL_NO_CONTEXT) {
        // コンテキストが作成できないのは異常事態
        throw create_exception_t(EGLException, EGLException_CreateContextFailed);
    }

// コンテキストを作成する
    {
        jc_sp< EGLContextProtocol > contextProtocol( new ndk::EGLContextManager(context, display, config) );
        this->device->setContext(contextProtocol);
    }

    // 仮サーフェイスを生成する
    {
        const EGLint attr[] = { EGL_HEIGHT, 1, EGL_WIDTH, 1, EGL_NONE };
        EGLSurface eglPBuffer = eglCreatePbufferSurface(display, config, attr);
        assert(eglPBuffer);

        pbufferSurface.reset(new EGLSurfaceManager(display, eglPBuffer));
    }
    initialized = jctrue;
}

/**
 * サーフェイス状態が変更になった
 */
void GLNativeTextureViewContext::onSurfaceSizeChanged(jobject surfaceTexture, const s32 width, const s32 height) {

    if (width == this->width && height == this->height) {
        // surfaceサイズが変わらない
        jcalertf("surface size not changed =(%d, %d)", width, height);
//        return;
    }

// デバイスに廃棄フラグを追加してからロックを行わせる
    MutexLock _lock(getDevice()->getGPUMutex()); // GPUアクセス中のロックを得ておく

    if (device->getSurface() == pbufferSurface) {
        device->setSurface(EGL_NULL_SURFACE);
    }

    {
        EGLManager *manager = dynamic_cast<EGLManager*>(device->getEGL().get());
        assert(manager);
        manager->stashEGLCurrents();
    }

// 縦横サイズを再設定
    this->width = width;
    this->height = height;

// 古いWindowが残っていたら、サーフェイスサイズ変更はNative実装に任せる
    if (device->getSurface()) {
        jc_sp<EGLSurfaceManager> surfaceManager = downcast<EGLSurfaceManager>(device->getSurface());
        assert(surfaceManager);
        jclogf("onSurfaceSize Changed(%d x %d)", width, height);
        surfaceManager->onSurfaceResized();
        surfaceManager->setSurfaceSize(width, height);
    } else {
// EGLSurfaceの構築を行う
        EGLSurface eglSurface = NULL;
        eglSurface = (EGLSurface) EGLSupport::eglCreateWindowSurfaceSupport((jint) display, (jint) config, surfaceTexture);

        if (eglSurface == EGL_NO_SURFACE ) {
            jclog("error surface");
            throw create_exception_t(EGLException, EGLException_SurfaceCreateFailed);
        }
        jclogf("window surface = %x", eglSurface);

        // サーフェイスを設定する
        this->device->setSurface(MEGLSurfaceProtocol(new EGLSurfaceManager(display, eglSurface)));
    }

}

/**
 * サーフェイスを休止する
 */
void GLNativeTextureViewContext::onGLSuspend() {

    // デバイスに廃棄フラグを追加してからロックを行わせる
    if (device) {
        jcalertf("onGLSuspend !! (%x)", device->getSurface().get());
        // デバイスに廃棄フラグを追加してからロックを行わせる
        MutexLock _lock(device->getGPUMutex()); // GPUアクセス中のロックを得ておく

        // 古いサーフェイスを削除する
        {
            MEGLSurfaceProtocol oldSurface = device->getSurface();
            oldSurface->dispose();
        }

        // レンダリング側が死なないように、仮サーフェイスをセットする
        {
            device->setSurface(pbufferSurface);
        }
    }

}

/**
 * コンテキストの廃棄作業を行う
 */
void GLNativeTextureViewContext::dispose() {
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

