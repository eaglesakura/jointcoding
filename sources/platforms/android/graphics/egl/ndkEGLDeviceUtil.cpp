/*
 * ndkEGLDeviceUtil.cpp
 *
 *  Created on: 2012/09/11
 */

#include    "ndkEGLDeviceUtil.h"
#include    "ndkEGLImpl.h"
#include    "ndkGLNativeSurfaceViewContext.h"

namespace ndk {

/**
 * 新たにEGLデバイスを作成する。
 * この時点でデバイスと画面は関連付けられていない。
 */
MDevice EGLDeviceUtil::createInstance() {
    MDevice result(new Device());
    result->setEGL(getEGLProtocol());
    return result;
}

/**
 * EGL管理クラスを取得する。
 * このインスタンスはシングルトン管理される。
 * EGL資源はAndroidアプリ内で1つしか利用できない（必要がない）ため。
 */
MEGLProtocol EGLDeviceUtil::getEGLProtocol() {
    static jc_sp<EGLManager> g_eglManager( new EGLManager() );
    return g_eglManager;
}

/**
 * glNativeSurfaceViewからデバイスインスタンスを取得する
 */
MDevice EGLDeviceUtil::getDevice(jobject glNativeSurfaceView) {
    jc_sp<ndk::GLNativeSurfaceViewContext>& context = GLNativeSurfaceViewContext::getNativeContext(glNativeSurfaceView);
    return context->getDevice();
}

}

