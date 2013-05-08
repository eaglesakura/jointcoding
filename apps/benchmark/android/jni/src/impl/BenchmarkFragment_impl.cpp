/*
 * BenchmarkFragment_impl.cpp
 *
 *  Created on: 2013/05/06
 */
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

#include    "benchmark.hpp"

using namespace ndk;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_createNativeContext(JNIEnv *env, jobject _this, jobject glnativetextureview_0) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_createNativeContext");

    ndk::setJointContextPointer_Object(_this, Jointable::KEY_MAINCONTEXT, (Object*) (new es::BenchmarkApplication()));
}

}
#endif
