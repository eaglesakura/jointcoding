/*
 * BenchmarkRenderer_impl.cpp
 *
 *  Created on: 2013/05/07
 */


#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"
#include    "jcandroid/egl/sdk/SdkDeviceManagerContext.hpp"

#include    "benchmark.hpp"

using namespace ndk;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext(JNIEnv *env, jobject _this, jobject devicemanager_0) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext");
    ndk::setJointContextPointer_Object(_this, Jointable::KEY_MAINCONTEXT, (Object*) (new es::BenchmarkApplication()));
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_loadTexture(JNIEnv *env, jobject _this, jobject jDeviceManager) {
    MDevice device = SdkDeviceManagerContext::getDeviceFromSdkDeviceManager(jDeviceManager);
    assert(device);

    // サーフェイスの作成完了を通知する
    joint_context(_this, es::BenchmarkApplication)->loadTexture(device);
}

}



