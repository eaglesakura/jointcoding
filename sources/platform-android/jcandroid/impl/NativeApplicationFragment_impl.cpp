/*
 * NativeApplicationFragment_impl.cpp
 *
 *  Created on: 2013/05/04
 */
#include "jointcoding-android.h"
#include "android-framework-classes/ndkNativeApplicationFragment.h"

#include    "jc/thread/Thread.h"

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized");
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause");
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume");
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy");
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop");
    jc::Thread::sleep(8);
}

}
