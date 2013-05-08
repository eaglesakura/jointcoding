/*
 * JointApplicationRenderer_impl.cpp
 *
 *  Created on: 2013/05/07
 */

#include "jointcoding-android.h"
#include "android-framework-classes/ndkJointApplicationRenderer.h"

#include    "jc/framework/app/JointApplicationBase.h"
#include    "jcandroid/egl/sdk/SdkDeviceManagerContext.hpp"

using namespace ndk;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize");
    jobject jDeviceManager = JointApplicationRenderer::getDeviceManager_unsafe_(_this);
    assert(jDeviceManager);
    {
        // レンダリングデバイスを得る
        MDevice device = SdkDeviceManagerContext::getDeviceFromSdkDeviceManager(jDeviceManager);
        assert(device);

        // サーフェイスの作成完了を通知する
        joint_context(_this, JointApplicationBase)->dispatchSurfaceCreated(device);
    }
    env->DeleteLocalRef(jDeviceManager);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeResume(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeResume");
    joint_context(_this, JointApplicationBase)->dispatchResume();
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativePause(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativePause");
    joint_context(_this, JointApplicationBase)->dispatchPause();
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint newWidth, jint newHeight) {
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeSurfaceResized");
    joint_context(_this, JointApplicationBase)->dispatchSurfaceResized(newWidth, newHeight);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeDestroy(JNIEnv *env, jobject _this) {
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeDestroy");
    joint_context(_this, JointApplicationBase)->dispatchDestroy();
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeMainLoop(JNIEnv *env, jobject _this) {
    joint_context(_this, JointApplicationBase)->dispatchMainLoop();
}

}

