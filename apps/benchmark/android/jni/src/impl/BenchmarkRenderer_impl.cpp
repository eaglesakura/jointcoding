/*
 * BenchmarkRenderer_impl.cpp
 *
 *  Created on: 2013/05/07
 */


#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

#include    "benchmark.hpp"

using namespace ndk;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext");
    ndk::setJointContextPointer_Object(_this, Jointable::KEY_MAINCONTEXT, (Object*) (new es::BenchmarkApplication()));
}

}



