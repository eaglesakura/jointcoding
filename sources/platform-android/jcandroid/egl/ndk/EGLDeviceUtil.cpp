/*
 * ndkEGLDeviceUtil.cpp
 *
 *  Created on: 2012/09/11
 */

#include    "jcandroid/egl/ndk/EGLDeviceUtil.h"
#include    "jcandroid/egl/ndk/EGLImpl.h"

namespace ndk {

/**
 * 新たにEGLデバイスを作成する。
 * この時点でデバイスと画面は関連付けられていない。
 */
MDevice EGLDeviceUtil::createInstance() {
    MDevice result(mark_new Device());
    result->setEGL(getEGLProtocol());
    return result;
}

/**
 * EGL管理クラスを取得する。
 * このインスタンスはシングルトン管理される。
 * EGL資源はAndroidアプリ内で1つしか利用できない（必要がない）ため。
 */
MEGLProtocol EGLDeviceUtil::getEGLProtocol() {
    static jc_sp<EGLManager> g_eglManager( mark_new EGLManager() );
    return g_eglManager;
}

}

