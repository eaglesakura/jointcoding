/*
 * ndkNativeContext_impl.cpp
 *
 *  Created on: 2012/07/22

 */
#include "jointcoding-android.h"
#include "ndkPlatformImpl.h"
#include "ndkSystemMemory.h"

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
}

