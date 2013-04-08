/*
 * NDK.cpp
 *
 *  Created on: 2012/07/11
 */
#include "jointcoding.h"
#include "jointcoding-android.h"
#include "jcandroid/ndk-support.h"

using namespace jc;
using namespace ndk;

namespace ndk {

namespace {

/**
 * JavaVM
 */
static JavaVM *g_javavm = NULL;

}

JavaVM* NDK::getJavaVM() {
    return g_javavm;
}

/**
 * 最新のJNIEnvを取得する
 */
JNIEnv* NDK::getJniEnv() {
    JNIEnv *env = NULL;
    g_javavm->GetEnv((void**) &env, JNI_VERSION_1_6);
    return env;
}

} /* namespace jc */

/**
 * VMを初期化する
 */
void initJniEnv(JNIEnv *env) {

    if (!g_javavm) {
        if (!ndk::g_javavm) {
            env->GetJavaVM(&g_javavm);
        }
    }
}
