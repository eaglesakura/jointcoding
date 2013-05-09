/**
 * export from "Joint Coding Project"
 */
#include "esBenchmarkRenderer.h"

namespace es {

const ::jc::charactor* BenchmarkRenderer::CLASS_SIGNATURE = "com/eaglesakura/jc/benchmark/app/BenchmarkRenderer";
const ::jc::s32 BenchmarkRenderer::KEY_MAINCONTEXT = 0;

static jclass class_BenchmarkRenderer = NULL;

#define methods_BenchmarkRenderer_LENGTH 11

#if methods_BenchmarkRenderer_LENGTH
static jmethodID methods_BenchmarkRenderer[11];
#endif

static void initialize_BenchmarkRenderer() {
    // loaded !
    if (class_BenchmarkRenderer) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_BenchmarkRenderer = env->FindClass(BenchmarkRenderer::CLASS_SIGNATURE);
    class_BenchmarkRenderer = (jclass)::ndk::change_globalref(env, class_BenchmarkRenderer);
    

    // load methods
    {
        methods_BenchmarkRenderer[0] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "loadTexture", "(Lcom/eaglesakura/jc/android/egl/DeviceManager;)V", false);
        methods_BenchmarkRenderer[1] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "onNativeMainLoop", "()V", false);
        methods_BenchmarkRenderer[2] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "onNativeDestroy", "()V", false);
        methods_BenchmarkRenderer[3] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "createNativeContext", "(Lcom/eaglesakura/jc/android/egl/DeviceManager;)V", false);
        methods_BenchmarkRenderer[4] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "onNativeInitialize", "()V", false);
        methods_BenchmarkRenderer[5] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "onNativeResume", "()V", false);
        methods_BenchmarkRenderer[6] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "onNativePause", "()V", false);
        methods_BenchmarkRenderer[7] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "getNativePointer", "(I)Lcom/eaglesakura/jc/android/resource/jni/Pointer;", false);
        methods_BenchmarkRenderer[8] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "getDeviceManager", "()Lcom/eaglesakura/jc/android/egl/DeviceManager;", false);
        methods_BenchmarkRenderer[9] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "setNativePointer", "(ILcom/eaglesakura/jc/android/resource/jni/Pointer;)V", false);
        methods_BenchmarkRenderer[10] = ::ndk::JniWrapper::loadMethod(class_BenchmarkRenderer, "onNativeSurfaceResized", "(II)V", false);

    }
}

BenchmarkRenderer::BenchmarkRenderer(jobject obj): ::ndk::JniWrapper(obj){
    initialize_BenchmarkRenderer();
}

void BenchmarkRenderer::loadTexture(jobject devicemanager_0) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[0], devicemanager_0);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_loadTexture(JNIEnv *env, jobject _this, jobject devicemanager_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_loadTexture(JNIEnv *env, jobject _this, jobject devicemanager_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_loadTexture");
    
    return;
}
#endif


void BenchmarkRenderer::loadTexture_(jobject _this, jobject devicemanager_0) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[0], devicemanager_0);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_loadTexture(JNIEnv *env, jobject _this, jobject devicemanager_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_loadTexture(JNIEnv *env, jobject _this, jobject devicemanager_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_loadTexture");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeMainLoop() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[1]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeMainLoop(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeMainLoop");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeMainLoop_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[1]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeMainLoop(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeMainLoop");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeDestroy() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[2]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeDestroy(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeDestroy(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeDestroy");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeDestroy_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[2]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeDestroy(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeDestroy(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeDestroy");
    
    return;
}
#endif


void BenchmarkRenderer::createNativeContext(jobject devicemanager_0) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[3], devicemanager_0);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext(JNIEnv *env, jobject _this, jobject devicemanager_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext(JNIEnv *env, jobject _this, jobject devicemanager_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext");
    
    return;
}
#endif


void BenchmarkRenderer::createNativeContext_(jobject _this, jobject devicemanager_0) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[3], devicemanager_0);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext(JNIEnv *env, jobject _this, jobject devicemanager_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext(JNIEnv *env, jobject _this, jobject devicemanager_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_createNativeContext");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeInitialize() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[4]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeInitialize(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeInitialize(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeInitialize");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeInitialize_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[4]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeInitialize(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeInitialize(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeInitialize");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeResume() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[5]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeResume(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeResume(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeResume");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeResume_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[5]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeResume(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeResume(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeResume");
    
    return;
}
#endif


void BenchmarkRenderer::onNativePause() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[6]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativePause(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativePause(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativePause");
    
    return;
}
#endif


void BenchmarkRenderer::onNativePause_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[6]);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativePause(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativePause(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativePause");
    
    return;
}
#endif


jobject BenchmarkRenderer::getNativePointer_unsafe(jint key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_BenchmarkRenderer[7], key);
}

jobject BenchmarkRenderer::getNativePointer_unsafe_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    return (jobject) env->CallObjectMethod(_this, methods_BenchmarkRenderer[7], key);
}

jobject BenchmarkRenderer::getDeviceManager_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_BenchmarkRenderer[8]);
}

jobject BenchmarkRenderer::getDeviceManager_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    return (jobject) env->CallObjectMethod(_this, methods_BenchmarkRenderer[8]);
}

void BenchmarkRenderer::setNativePointer(jint key, jobject ptr) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[9], key, ptr);
}

void BenchmarkRenderer::setNativePointer_(jobject _this, jint key, jobject ptr) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[9], key, ptr);
}

void BenchmarkRenderer::onNativeSurfaceResized(jint int_0, jint int_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_BenchmarkRenderer[10], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeSurfaceResized");
    
    return;
}
#endif


void BenchmarkRenderer::onNativeSurfaceResized_(jobject _this, jint int_0, jint int_1) {
    CALL_JNIENV();
    initialize_BenchmarkRenderer();
    env->CallVoidMethod(_this, methods_BenchmarkRenderer[10], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "esBenchmarkRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_benchmark_app_BenchmarkRenderer_onNativeSurfaceResized");
    
    return;
}
#endif


jc_sp<BenchmarkRenderer> BenchmarkRenderer::wrap(jobject obj) {
    return jc_sp<BenchmarkRenderer>( new BenchmarkRenderer(obj));
}

jc_sp<BenchmarkRenderer> BenchmarkRenderer::global(jobject obj) {
    return jc_sp<BenchmarkRenderer>( (BenchmarkRenderer*)(new BenchmarkRenderer(obj))->addGlobalRef());
}

jclass BenchmarkRenderer::getClass() {
    initialize_BenchmarkRenderer();
    return class_BenchmarkRenderer;
}

}

