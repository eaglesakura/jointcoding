/*
 * ndkNativeContext_impl.cpp
 *
 *  Created on: 2012/07/22

 */
#include "jointcoding-android.h"
#include "jcandroid/platform/PlatformImpl.h"
#include "jcandroid/jni/SystemMemory.h"

namespace ndk {
extern void initPlatform(MNativeContext context);
}

using namespace ndk;
extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    jclog("Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize()");
    jc_sp<NativeContext> context = NativeContext::global(_this);
    jclog("Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize() finished");
    initPlatform(context);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_nativeGC(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    SystemMemory::gc();
    return;
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_isNativeDebuggable(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

#ifdef  DEBUG
    return (jboolean) JNI_TRUE;
#else
    return (jboolean)JNI_FALSE;
#endif
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_isNativeLogOutput(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

#if  defined(DEBUG) || defined(OUTPUT_LOG)
    return (jboolean)JNI_TRUE;
#else
    return (jboolean) JNI_FALSE;
#endif
}
}

