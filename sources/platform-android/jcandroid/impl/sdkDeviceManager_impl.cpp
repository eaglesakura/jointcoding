/*
 * SDKDeviceManager_impl.cpp
 *
 *  Created on: 2013/05/07
 */

#include "jointcoding-android.h"
#include "android-classes/ndkDeviceManager.h"

#include    "jcandroid/egl/sdk/EGLWrapper.h"
#include    "jcandroid/egl/sdk/EGLSurfaceWrapper.hpp"
#include    "jcandroid/egl/sdk/EGLContextWrapper.hpp"

#include    "jcandroid/egl/sdk/SdkDeviceManagerContext.hpp"

using namespace ndk;
extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_createNative(JNIEnv *env, jobject _this) {
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_egl_DeviceManager_createNative");

    // SDK側のコンテキストを取り出す
    jobject jEGLWrapper = DeviceManager::getEGLWrapper_unsafe_(_this);
    jobject jEGLSurfaceWrapper = DeviceManager::getEGLSurfaceWrapper_unsafe_(_this);
    jobject jEGLContextWrapper = DeviceManager::getEGLContextWrapper_unsafe_(_this);

    assert(jEGLWrapper);
    assert(jEGLSurfaceWrapper);
    assert(jEGLContextWrapper);

    // マネージクラスを作成する
    jc_sp<SdkEGLWrapper> eglWrapper( new SdkEGLWrapper( EGLWrapper::global(jEGLWrapper) ));
    jc_sp<SdkEGLSurfaceWrapper> eglSurfaceWrapper( new SdkEGLSurfaceWrapper( EGLSurfaceWrapper::global(jEGLSurfaceWrapper) ));
    jc_sp<SdkEGLContextWrapper> eglContextWrapper( new SdkEGLContextWrapper( EGLContextWrapper::global(jEGLContextWrapper) ));

    MDevice device(new Device());
    device->setEGL(eglWrapper);
    device->setSurface(eglSurfaceWrapper);
    device->setContext(eglContextWrapper);

    setJointContextPointer_Object(_this, Jointable::KEY_MAINCONTEXT, new SdkDeviceManagerContext(device));
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_preDestroyNative(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_egl_DeviceManager_preDestroyNative");

    // デバイスに廃棄フラグを追加する
    MDevice device = joint_context(_this, SdkDeviceManagerContext)->getDevice();
    device->addFlags(DeviceFlag_RequestDestroy);

    // デバイスの同期待を行う
    MutexLock lock(device->getGPUMutex());
}

}

