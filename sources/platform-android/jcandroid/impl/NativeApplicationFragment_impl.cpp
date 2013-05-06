/*
 * NativeApplicationFragment_impl.cpp
 *
 *  Created on: 2013/05/04
 */
#include "jointcoding-android.h"
#include "android-framework-classes/ndkNativeApplicationFragment.h"

#include    "jc/thread/Thread.h"
#include    "jc/framework/app/JointApplicationBase.h"
#include    "jcandroid/view/GLNativeTextureViewContext.h"

using namespace jc;
using namespace jc::gl;
using namespace ndk;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeInitialize(JNIEnv *env, jobject _this) {
    jobject jGLNativeTextureView = NativeApplicationFragment::getSurface_unsafe_(_this);
    {
        MDevice device = GLNativeTextureViewContext::getDevice(jGLNativeTextureView);
        assert(device);

        // サーフェイスの作成完了を通知する
        joint_context(_this, JointApplicationBase)->dispatchSurfaceCreated(device);
    }
    env->DeleteLocalRef(jGLNativeTextureView);

}
// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint newWidth, jint newHeight) {
    joint_context(_this, JointApplicationBase)->dispatchSurfaceResized(newWidth, newHeight);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause(JNIEnv *env, jobject _this) {
    joint_context(_this, JointApplicationBase)->dispatchPause();
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume(JNIEnv *env, jobject _this) {
    joint_context(_this, JointApplicationBase)->dispatchResume();
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy(JNIEnv *env, jobject _this) {
    joint_context(_this, JointApplicationBase)->dispatchDestroy();
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop(JNIEnv *env, jobject _this) {
    joint_context(_this, JointApplicationBase)->dispatchMainLoop();
}

}
