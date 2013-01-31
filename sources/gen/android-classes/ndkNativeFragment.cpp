/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeFragment.h"

namespace ndk {

const ::jc::charactor* NativeFragment::CLASS_SIGNATURE = "com/eaglesakura/jc/android/fragment/NativeFragment";

static jclass class_NativeFragment = NULL;

#define methods_NativeFragment_LENGTH 8

#if methods_NativeFragment_LENGTH
static jmethodID methods_NativeFragment[8];
#endif

static void initialize_NativeFragment() {
    // loaded !
    if (class_NativeFragment) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeFragment = env->FindClass(NativeFragment::CLASS_SIGNATURE);
    class_NativeFragment = (jclass)::ndk::change_globalref(env, class_NativeFragment);
    

    // load methods
    {
        methods_NativeFragment[0] = ::ndk::JniWrapper::loadMethod(class_NativeFragment, "onActivityCreatedCompleteNative", "(Landroid/os/Bundle;)V", false);
        methods_NativeFragment[1] = ::ndk::JniWrapper::loadMethod(class_NativeFragment, "onResumeCompletedNative", "()V", false);
        methods_NativeFragment[2] = ::ndk::JniWrapper::loadMethod(class_NativeFragment, "onDestroyBeginNative", "()V", false);
        methods_NativeFragment[3] = ::ndk::JniWrapper::loadMethod(class_NativeFragment, "onCreateCompletedNative", "(Landroid/os/Bundle;)V", false);
        methods_NativeFragment[4] = ::ndk::JniWrapper::loadMethod(class_NativeFragment, "onAttachCompletedNative", "(Landroid/app/Activity;)V", false);
        methods_NativeFragment[5] = ::ndk::JniWrapper::loadMethod(class_NativeFragment, "getNativeStatePointer", "()I", false);
        methods_NativeFragment[6] = ::ndk::JniWrapper::loadMethod(class_NativeFragment, "onPauseBeginNative", "()V", false);
        methods_NativeFragment[7] = ::ndk::JniWrapper::loadMethod(class_NativeFragment, "onDetachBeginNative", "()V", false);

    }
}

NativeFragment::NativeFragment(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeFragment();
}

void NativeFragment::onActivityCreatedCompleteNative(jobject bundle_0) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeFragment[0], bundle_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onActivityCreatedCompleteNative(JNIEnv *env, jobject _this, jobject bundle_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onActivityCreatedCompleteNative(JNIEnv *env, jobject _this, jobject bundle_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onActivityCreatedCompleteNative");
    
    return;
}
#endif


void NativeFragment::onActivityCreatedCompleteNative_(jobject _this, jobject bundle_0) {
    CALL_JNIENV();
    initialize_NativeFragment();
    env->CallVoidMethod(_this, methods_NativeFragment[0], bundle_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onActivityCreatedCompleteNative(JNIEnv *env, jobject _this, jobject bundle_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onActivityCreatedCompleteNative(JNIEnv *env, jobject _this, jobject bundle_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onActivityCreatedCompleteNative");
    
    return;
}
#endif


void NativeFragment::onResumeCompletedNative() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeFragment[1]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onResumeCompletedNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onResumeCompletedNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onResumeCompletedNative");
    
    return;
}
#endif


void NativeFragment::onResumeCompletedNative_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeFragment();
    env->CallVoidMethod(_this, methods_NativeFragment[1]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onResumeCompletedNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onResumeCompletedNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onResumeCompletedNative");
    
    return;
}
#endif


void NativeFragment::onDestroyBeginNative() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeFragment[2]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDestroyBeginNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDestroyBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDestroyBeginNative");
    
    return;
}
#endif


void NativeFragment::onDestroyBeginNative_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeFragment();
    env->CallVoidMethod(_this, methods_NativeFragment[2]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDestroyBeginNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDestroyBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDestroyBeginNative");
    
    return;
}
#endif


void NativeFragment::onCreateCompletedNative(jobject bundle_0) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeFragment[3], bundle_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onCreateCompletedNative(JNIEnv *env, jobject _this, jobject bundle_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onCreateCompletedNative(JNIEnv *env, jobject _this, jobject bundle_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onCreateCompletedNative");
    
    return;
}
#endif


void NativeFragment::onCreateCompletedNative_(jobject _this, jobject bundle_0) {
    CALL_JNIENV();
    initialize_NativeFragment();
    env->CallVoidMethod(_this, methods_NativeFragment[3], bundle_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onCreateCompletedNative(JNIEnv *env, jobject _this, jobject bundle_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onCreateCompletedNative(JNIEnv *env, jobject _this, jobject bundle_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onCreateCompletedNative");
    
    return;
}
#endif


void NativeFragment::onAttachCompletedNative(jobject activity_0) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeFragment[4], activity_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onAttachCompletedNative(JNIEnv *env, jobject _this, jobject activity_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onAttachCompletedNative(JNIEnv *env, jobject _this, jobject activity_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onAttachCompletedNative");
    
    return;
}
#endif


void NativeFragment::onAttachCompletedNative_(jobject _this, jobject activity_0) {
    CALL_JNIENV();
    initialize_NativeFragment();
    env->CallVoidMethod(_this, methods_NativeFragment[4], activity_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onAttachCompletedNative(JNIEnv *env, jobject _this, jobject activity_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onAttachCompletedNative(JNIEnv *env, jobject _this, jobject activity_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onAttachCompletedNative");
    
    return;
}
#endif


jint NativeFragment::getNativeStatePointer() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_NativeFragment[5]);
}

jint NativeFragment::getNativeStatePointer_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeFragment();
    return (jint) env->CallIntMethod(_this, methods_NativeFragment[5]);
}

void NativeFragment::onPauseBeginNative() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeFragment[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onPauseBeginNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onPauseBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onPauseBeginNative");
    
    return;
}
#endif


void NativeFragment::onPauseBeginNative_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeFragment();
    env->CallVoidMethod(_this, methods_NativeFragment[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onPauseBeginNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onPauseBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onPauseBeginNative");
    
    return;
}
#endif


void NativeFragment::onDetachBeginNative() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeFragment[7]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDetachBeginNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDetachBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDetachBeginNative");
    
    return;
}
#endif


void NativeFragment::onDetachBeginNative_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeFragment();
    env->CallVoidMethod(_this, methods_NativeFragment[7]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeFragment.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDetachBeginNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDetachBeginNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_fragment_NativeFragment_onDetachBeginNative");
    
    return;
}
#endif


jc_sp<NativeFragment> NativeFragment::wrap(jobject obj) {
    return jc_sp<NativeFragment>( new NativeFragment(obj));
}

jc_sp<NativeFragment> NativeFragment::global(jobject obj) {
    return jc_sp<NativeFragment>( (NativeFragment*)(new NativeFragment(obj))->addGlobalRef());
}

jclass NativeFragment::getClass() {
    initialize_NativeFragment();
    return class_NativeFragment;
}

}

