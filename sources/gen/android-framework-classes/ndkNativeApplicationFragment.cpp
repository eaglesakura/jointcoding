/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeApplicationFragment.h"

namespace ndk {

const ::jc::charactor* NativeApplicationFragment::CLASS_SIGNATURE = "com/eaglesakura/jc/framework/app/NativeApplicationFragment";
const ::jc::s32 NativeApplicationFragment::KEY_MAINCONTEXT = 0;

static jclass class_NativeApplicationFragment = NULL;

#define methods_NativeApplicationFragment_LENGTH 8

#if methods_NativeApplicationFragment_LENGTH
static jmethodID methods_NativeApplicationFragment[8];
#endif

static void initialize_NativeApplicationFragment() {
    // loaded !
    if (class_NativeApplicationFragment) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeApplicationFragment = env->FindClass(NativeApplicationFragment::CLASS_SIGNATURE);
    class_NativeApplicationFragment = (jclass)::ndk::change_globalref(env, class_NativeApplicationFragment);
    

    // load methods
    {
        methods_NativeApplicationFragment[0] = ::ndk::JniWrapper::loadMethod(class_NativeApplicationFragment, "onNativeDestroy", "()V", false);
        methods_NativeApplicationFragment[1] = ::ndk::JniWrapper::loadMethod(class_NativeApplicationFragment, "onNativePause", "()V", false);
        methods_NativeApplicationFragment[2] = ::ndk::JniWrapper::loadMethod(class_NativeApplicationFragment, "setNativePointer", "(ILcom/eaglesakura/jc/android/resource/jni/Pointer;)V", false);
        methods_NativeApplicationFragment[3] = ::ndk::JniWrapper::loadMethod(class_NativeApplicationFragment, "onNativeSurfaceResized", "(II)V", false);
        methods_NativeApplicationFragment[4] = ::ndk::JniWrapper::loadMethod(class_NativeApplicationFragment, "onNativeMainLoop", "()V", false);
        methods_NativeApplicationFragment[5] = ::ndk::JniWrapper::loadMethod(class_NativeApplicationFragment, "onNativeResume", "()V", false);
        methods_NativeApplicationFragment[6] = ::ndk::JniWrapper::loadMethod(class_NativeApplicationFragment, "getNativePointer", "(I)Lcom/eaglesakura/jc/android/resource/jni/Pointer;", false);
        methods_NativeApplicationFragment[7] = ::ndk::JniWrapper::loadMethod(class_NativeApplicationFragment, "getSurface", "()Lcom/eaglesakura/jc/android/view/GLNativeTextureView;", false);

    }
}

NativeApplicationFragment::NativeApplicationFragment(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeApplicationFragment();
}

void NativeApplicationFragment::onNativeDestroy() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeApplicationFragment[0]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy");
    
    return;
}
#endif


void NativeApplicationFragment::onNativeDestroy_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeApplicationFragment();
    env->CallVoidMethod(_this, methods_NativeApplicationFragment[0]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeDestroy");
    
    return;
}
#endif


void NativeApplicationFragment::onNativePause() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeApplicationFragment[1]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause");
    
    return;
}
#endif


void NativeApplicationFragment::onNativePause_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeApplicationFragment();
    env->CallVoidMethod(_this, methods_NativeApplicationFragment[1]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativePause");
    
    return;
}
#endif


void NativeApplicationFragment::setNativePointer(jint key, jobject ptr) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeApplicationFragment[2], key, ptr);
}

void NativeApplicationFragment::setNativePointer_(jobject _this, jint key, jobject ptr) {
    CALL_JNIENV();
    initialize_NativeApplicationFragment();
    env->CallVoidMethod(_this, methods_NativeApplicationFragment[2], key, ptr);
}

void NativeApplicationFragment::onNativeSurfaceResized(jint int_0, jint int_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeApplicationFragment[3], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized");
    
    return;
}
#endif


void NativeApplicationFragment::onNativeSurfaceResized_(jobject _this, jint int_0, jint int_1) {
    CALL_JNIENV();
    initialize_NativeApplicationFragment();
    env->CallVoidMethod(_this, methods_NativeApplicationFragment[3], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeSurfaceResized");
    
    return;
}
#endif


void NativeApplicationFragment::onNativeMainLoop() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeApplicationFragment[4]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop");
    
    return;
}
#endif


void NativeApplicationFragment::onNativeMainLoop_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeApplicationFragment();
    env->CallVoidMethod(_this, methods_NativeApplicationFragment[4]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeMainLoop");
    
    return;
}
#endif


void NativeApplicationFragment::onNativeResume() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeApplicationFragment[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume");
    
    return;
}
#endif


void NativeApplicationFragment::onNativeResume_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeApplicationFragment();
    env->CallVoidMethod(_this, methods_NativeApplicationFragment[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeApplicationFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_NativeApplicationFragment_onNativeResume");
    
    return;
}
#endif


jobject NativeApplicationFragment::getNativePointer_unsafe(jint key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_NativeApplicationFragment[6], key);
}

jobject NativeApplicationFragment::getNativePointer_unsafe_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_NativeApplicationFragment();
    return (jobject) env->CallObjectMethod(_this, methods_NativeApplicationFragment[6], key);
}

jobject NativeApplicationFragment::getSurface_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_NativeApplicationFragment[7]);
}

jobject NativeApplicationFragment::getSurface_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeApplicationFragment();
    return (jobject) env->CallObjectMethod(_this, methods_NativeApplicationFragment[7]);
}

jc_sp<NativeApplicationFragment> NativeApplicationFragment::wrap(jobject obj) {
    return jc_sp<NativeApplicationFragment>( new NativeApplicationFragment(obj));
}

jc_sp<NativeApplicationFragment> NativeApplicationFragment::global(jobject obj) {
    return jc_sp<NativeApplicationFragment>( (NativeApplicationFragment*)(new NativeApplicationFragment(obj))->addGlobalRef());
}

jclass NativeApplicationFragment::getClass() {
    initialize_NativeApplicationFragment();
    return class_NativeApplicationFragment;
}

}

