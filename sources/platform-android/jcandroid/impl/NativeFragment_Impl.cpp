/*
 * ndkNativeFragmentImpl.cpp
 *
 *  Created on: 2012/09/19
 */

#include "jointcoding-android.h"
#include "android-classes/ndkNativeFragment.h"
#include "jcandroid/fragment/NativeFragmentBase.h"

#define fragment    native_fragment(_this)

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onActivityCreatedCompleteNative(JNIEnv *env, jobject _this, jobject bundle) {
    // call env reset
    initJniEnv(env);
    fragment->onActivityCreatedComplete(_this,bundle);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onResumeCompletedNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    fragment->onResumeCompleted(_this);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDestroyBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    fragment->onDestroyBegin(_this);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onCreateCompletedNative(JNIEnv *env, jobject _this, jobject bundle) {
    // call env reset
    initJniEnv(env);

    fragment->onCreateCompleted(_this,bundle);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onAttachCompletedNative(JNIEnv *env, jobject _this, jobject activity) {
    // call env reset
    initJniEnv(env);

    fragment->onAttachCompleted(_this,activity);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onPauseBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    fragment->onPauseBegin(_this);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDetachBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    fragment->onDetachBegin(_this);
    return;
}

}

