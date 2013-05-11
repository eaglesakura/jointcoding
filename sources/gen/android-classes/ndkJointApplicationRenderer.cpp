/**
 * export from "Joint Coding Project"
 */
#include "ndkJointApplicationRenderer.h"

namespace ndk {

const ::jc::charactor* JointApplicationRenderer::CLASS_SIGNATURE = "com/eaglesakura/jc/framework/app/JointApplicationRenderer";
const ::jc::s32 JointApplicationRenderer::KEY_MAINCONTEXT = 0;

static jclass class_JointApplicationRenderer = NULL;

#define methods_JointApplicationRenderer_LENGTH 11

#if methods_JointApplicationRenderer_LENGTH
static jmethodID methods_JointApplicationRenderer[11];
#endif

static void initialize_JointApplicationRenderer() {
    // loaded !
    if (class_JointApplicationRenderer) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JointApplicationRenderer = env->FindClass(JointApplicationRenderer::CLASS_SIGNATURE);
    class_JointApplicationRenderer = (jclass)::ndk::change_globalref(env, class_JointApplicationRenderer);
    

    // load methods
    {
        methods_JointApplicationRenderer[0] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativeMainLoop", "()V", false);
        methods_JointApplicationRenderer[1] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "setNativePointer", "(ILcom/eaglesakura/jc/jni/Pointer;)V", false);
        methods_JointApplicationRenderer[2] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativeDestroy", "()V", false);
        methods_JointApplicationRenderer[3] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "queryParams", "(II[I)Z", false);
        methods_JointApplicationRenderer[4] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativeInitialize", "()V", false);
        methods_JointApplicationRenderer[5] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativeResume", "()V", false);
        methods_JointApplicationRenderer[6] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativePause", "()V", false);
        methods_JointApplicationRenderer[7] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "postParams", "(II[I)Z", false);
        methods_JointApplicationRenderer[8] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "getNativePointer", "(I)Lcom/eaglesakura/jc/jni/Pointer;", false);
        methods_JointApplicationRenderer[9] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "getDeviceManager", "()Lcom/eaglesakura/jc/egl/DeviceManager;", false);
        methods_JointApplicationRenderer[10] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativeSurfaceResized", "(II)V", false);

    }
}

JointApplicationRenderer::JointApplicationRenderer(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JointApplicationRenderer();
}

void JointApplicationRenderer::onNativeMainLoop() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[0]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeMainLoop(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeMainLoop");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeMainLoop_(jobject _this) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[0]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeMainLoop(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeMainLoop");
    
    return;
}
#endif


void JointApplicationRenderer::setNativePointer(jint key, jobject ptr) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[1], key, ptr);
}

void JointApplicationRenderer::setNativePointer_(jobject _this, jint key, jobject ptr) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[1], key, ptr);
}

void JointApplicationRenderer::onNativeDestroy() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[2]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeDestroy(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeDestroy(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeDestroy");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeDestroy_(jobject _this) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[2]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeDestroy(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeDestroy(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeDestroy");
    
    return;
}
#endif


jboolean JointApplicationRenderer::queryParams(jint int_0, jint int_1, jintArray int_array_2) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_JointApplicationRenderer[3], int_0, int_1, int_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryParams");
    
    return (jboolean) 0;
}
#endif


jboolean JointApplicationRenderer::queryParams_(jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jboolean) env->CallBooleanMethod(_this, methods_JointApplicationRenderer[3], int_0, int_1, int_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryParams");
    
    return (jboolean) 0;
}
#endif


void JointApplicationRenderer::onNativeInitialize() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[4]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeInitialize_(jobject _this) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[4]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeResume() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeResume(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeResume(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeResume");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeResume_(jobject _this) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeResume(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeResume(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeResume");
    
    return;
}
#endif


void JointApplicationRenderer::onNativePause() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativePause(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativePause(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativePause");
    
    return;
}
#endif


void JointApplicationRenderer::onNativePause_(jobject _this) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativePause(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativePause(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativePause");
    
    return;
}
#endif


jboolean JointApplicationRenderer::postParams(jint int_0, jint int_1, jintArray int_array_2) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_JointApplicationRenderer[7], int_0, int_1, int_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams");
    
    return (jboolean) 0;
}
#endif


jboolean JointApplicationRenderer::postParams_(jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jboolean) env->CallBooleanMethod(_this, methods_JointApplicationRenderer[7], int_0, int_1, int_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams");
    
    return (jboolean) 0;
}
#endif


jobject JointApplicationRenderer::getNativePointer_unsafe(jint key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointApplicationRenderer[8], key);
}

jobject JointApplicationRenderer::getNativePointer_unsafe_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jobject) env->CallObjectMethod(_this, methods_JointApplicationRenderer[8], key);
}

jobject JointApplicationRenderer::getDeviceManager_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointApplicationRenderer[9]);
}

jobject JointApplicationRenderer::getDeviceManager_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jobject) env->CallObjectMethod(_this, methods_JointApplicationRenderer[9]);
}

void JointApplicationRenderer::onNativeSurfaceResized(jint int_0, jint int_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[10], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeSurfaceResized");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeSurfaceResized_(jobject _this, jint int_0, jint int_1) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[10], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeSurfaceResized");
    
    return;
}
#endif


jc_sp<JointApplicationRenderer> JointApplicationRenderer::wrap(jobject obj) {
    return jc_sp<JointApplicationRenderer>( new JointApplicationRenderer(obj));
}

jc_sp<JointApplicationRenderer> JointApplicationRenderer::global(jobject obj) {
    return jc_sp<JointApplicationRenderer>( (JointApplicationRenderer*)(new JointApplicationRenderer(obj))->addGlobalRef());
}

jclass JointApplicationRenderer::getClass() {
    initialize_JointApplicationRenderer();
    return class_JointApplicationRenderer;
}

}

