/**
 * export from "Joint Coding Project"
 */
#include "esBenchmarkFragment.h"

namespace es {

const ::jc::charactor* BenchmarkFragment::CLASS_SIGNATURE = "com/eaglesakura/jc/benchmark/app/BenchmarkFragment";
const ::jc::s32 BenchmarkFragment::KEY_MAINCONTEXT = 0;

static jclass class_BenchmarkFragment = NULL;

#define methods_BenchmarkFragment_LENGTH 9

#if methods_BenchmarkFragment_LENGTH
static jmethodID methods_BenchmarkFragment[9];
#endif

static void initialize_BenchmarkFragment() {
    // loaded !
    if (class_BenchmarkFragment) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_BenchmarkFragment = env->FindClass(BenchmarkFragment::CLASS_SIGNATURE);
    class_BenchmarkFragment = (jclass)::ndk::change_globalref(env, class_BenchmarkFragment);
    

    // load methods
    {
        methods_BenchmarkFragment[0] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "onNativeDestroy", "()V", false);
        methods_BenchmarkFragment[1] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "onNativePause", "()V", false);
        methods_BenchmarkFragment[2] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "setNativePointer", "(ILcom/eaglesakura/jc/android/resource/jni/Pointer;)V", false);
        methods_BenchmarkFragment[3] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "onNativeSurfaceResized", "(II)V", false);
        methods_BenchmarkFragment[4] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "onNativeMainLoop", "()V", false);
        methods_BenchmarkFragment[5] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "createNativeContext", "(Lcom/eaglesakura/jc/android/view/GLNativeTextureView;)V", false);
        methods_BenchmarkFragment[6] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "onNativeResume", "()V", false);
        methods_BenchmarkFragment[7] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "getNativePointer", "(I)Lcom/eaglesakura/jc/android/resource/jni/Pointer;", false);
        methods_BenchmarkFragment[8] = ::ndk::JniWrapper::loadMethod(class_BenchmarkFragment, "getSurface", "()Lcom/eaglesakura/jc/android/view/GLNativeTextureView;", false);

    }
}

BenchmarkFragment::BenchmarkFragment(jobject obj): ::ndk::JniWrapper(obj){
    initialize_BenchmarkFragment();
}

void BenchmarkFragment::onNativeDestroy() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkFragment[0]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeDestroy(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeDestroy(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeDestroy");
    
    return;
}
#endif


void BenchmarkFragment::onNativeDestroy_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    env->CallVoidMethod(_this, methods_BenchmarkFragment[0]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeDestroy(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeDestroy(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeDestroy");
    
    return;
}
#endif


void BenchmarkFragment::onNativePause() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkFragment[1]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativePause(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativePause(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativePause");
    
    return;
}
#endif


void BenchmarkFragment::onNativePause_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    env->CallVoidMethod(_this, methods_BenchmarkFragment[1]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativePause(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativePause(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativePause");
    
    return;
}
#endif


void BenchmarkFragment::setNativePointer(jint key, jobject ptr) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkFragment[2], key, ptr);
}

void BenchmarkFragment::setNativePointer_(jobject _this, jint key, jobject ptr) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    env->CallVoidMethod(_this, methods_BenchmarkFragment[2], key, ptr);
}

void BenchmarkFragment::onNativeSurfaceResized(jint int_0, jint int_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkFragment[3], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeSurfaceResized");
    
    return;
}
#endif


void BenchmarkFragment::onNativeSurfaceResized_(jobject _this, jint int_0, jint int_1) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    env->CallVoidMethod(_this, methods_BenchmarkFragment[3], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeSurfaceResized");
    
    return;
}
#endif


void BenchmarkFragment::onNativeMainLoop() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkFragment[4]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeMainLoop(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeMainLoop");
    
    return;
}
#endif


void BenchmarkFragment::onNativeMainLoop_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    env->CallVoidMethod(_this, methods_BenchmarkFragment[4]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeMainLoop(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeMainLoop");
    
    return;
}
#endif


void BenchmarkFragment::createNativeContext(jobject glnativetextureview_0) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkFragment[5], glnativetextureview_0);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_createNativeContext(JNIEnv *env, jobject _this, jobject glnativetextureview_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_createNativeContext(JNIEnv *env, jobject _this, jobject glnativetextureview_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_createNativeContext");
    
    return;
}
#endif


void BenchmarkFragment::createNativeContext_(jobject _this, jobject glnativetextureview_0) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    env->CallVoidMethod(_this, methods_BenchmarkFragment[5], glnativetextureview_0);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_createNativeContext(JNIEnv *env, jobject _this, jobject glnativetextureview_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_createNativeContext(JNIEnv *env, jobject _this, jobject glnativetextureview_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_createNativeContext");
    
    return;
}
#endif


void BenchmarkFragment::onNativeResume() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkFragment[6]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeResume(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeResume(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeResume");
    
    return;
}
#endif


void BenchmarkFragment::onNativeResume_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    env->CallVoidMethod(_this, methods_BenchmarkFragment[6]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeResume(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeResume(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkFragment_onNativeResume");
    
    return;
}
#endif


jobject BenchmarkFragment::getNativePointer_unsafe(jint key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_BenchmarkFragment[7], key);
}

jobject BenchmarkFragment::getNativePointer_unsafe_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    return (jobject) env->CallObjectMethod(_this, methods_BenchmarkFragment[7], key);
}

jobject BenchmarkFragment::getSurface_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_BenchmarkFragment[8]);
}

jobject BenchmarkFragment::getSurface_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkFragment();
    return (jobject) env->CallObjectMethod(_this, methods_BenchmarkFragment[8]);
}

jc_sp<BenchmarkFragment> BenchmarkFragment::wrap(jobject obj) {
    return jc_sp<BenchmarkFragment>( new BenchmarkFragment(obj));
}

jc_sp<BenchmarkFragment> BenchmarkFragment::global(jobject obj) {
    return jc_sp<BenchmarkFragment>( (BenchmarkFragment*)(new BenchmarkFragment(obj))->addGlobalRef());
}

jclass BenchmarkFragment::getClass() {
    initialize_BenchmarkFragment();
    return class_BenchmarkFragment;
}

}

