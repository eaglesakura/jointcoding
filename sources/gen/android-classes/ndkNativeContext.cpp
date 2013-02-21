/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeContext.h"

namespace ndk {

const ::jc::charactor* NativeContext::CLASS_SIGNATURE = "com/eaglesakura/jc/android/app/NativeContext";

static jclass class_NativeContext = NULL;

#define methods_NativeContext_LENGTH 7

#if methods_NativeContext_LENGTH
static jmethodID methods_NativeContext[7];
#endif

static void initialize_NativeContext() {
    // loaded !
    if (class_NativeContext) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeContext = env->FindClass(NativeContext::CLASS_SIGNATURE);
    class_NativeContext = (jclass)::ndk::change_globalref(env, class_NativeContext);
    

    // load methods
    {
        methods_NativeContext[0] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "getBootingId", "()Ljava/lang/String;", true);
        methods_NativeContext[1] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "isNativeDebuggable", "()Z", true);
        methods_NativeContext[2] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "nativeInitialize", "()V", false);
        methods_NativeContext[3] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "gc", "()V", true);
        methods_NativeContext[4] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "getAppContext", "()Landroid/content/Context;", false);
        methods_NativeContext[5] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "isUIThread", "()Z", true);
        methods_NativeContext[6] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "nativeGC", "()V", true);

    }
}

NativeContext::NativeContext(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeContext();
}

jstring NativeContext::getBootingId() {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jstring) env->CallStaticObjectMethod(class_NativeContext, methods_NativeContext[0]);
}

jboolean NativeContext::isNativeDebuggable() {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jboolean) env->CallStaticBooleanMethod(class_NativeContext, methods_NativeContext[1]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeContext.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_isNativeDebuggable(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_isNativeDebuggable(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_app_NativeContext_isNativeDebuggable");
    
    return (jboolean) 0;
}
#endif


void NativeContext::nativeInitialize() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeContext[2]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeContext.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize");
    
    return;
}
#endif


void NativeContext::nativeInitialize_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeContext();
    env->CallVoidMethod(_this, methods_NativeContext[2]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeContext.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_app_NativeContext_nativeInitialize");
    
    return;
}
#endif


void NativeContext::gc() {
    CALL_JNIENV();
    initialize_NativeContext();
    env->CallStaticVoidMethod(class_NativeContext, methods_NativeContext[3]);
}

jobject NativeContext::getAppContext_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_NativeContext[4]);
}

jobject NativeContext::getAppContext_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jobject) env->CallObjectMethod(_this, methods_NativeContext[4]);
}

jboolean NativeContext::isUIThread() {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jboolean) env->CallStaticBooleanMethod(class_NativeContext, methods_NativeContext[5]);
}

void NativeContext::nativeGC() {
    CALL_JNIENV();
    initialize_NativeContext();
    env->CallStaticVoidMethod(class_NativeContext, methods_NativeContext[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeContext.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_nativeGC(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_nativeGC(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_app_NativeContext_nativeGC");
    
    return;
}
#endif


jc_sp<NativeContext> NativeContext::wrap(jobject obj) {
    return jc_sp<NativeContext>( new NativeContext(obj));
}

jc_sp<NativeContext> NativeContext::global(jobject obj) {
    return jc_sp<NativeContext>( (NativeContext*)(new NativeContext(obj))->addGlobalRef());
}

jclass NativeContext::getClass() {
    initialize_NativeContext();
    return class_NativeContext;
}

}

