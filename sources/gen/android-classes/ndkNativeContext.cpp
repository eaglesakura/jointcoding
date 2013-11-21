/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeContext.h"

namespace ndk {

const ::jc::charactor* NativeContext::CLASS_SIGNATURE = "com/eaglesakura/jc/android/app/NativeContext";

static jclass class_NativeContext = NULL;

#define methods_NativeContext_LENGTH 13

#if methods_NativeContext_LENGTH
static jmethodID methods_NativeContext[13];
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
        methods_NativeContext[0] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "isNativeDebuggable", "()Z", true);
        methods_NativeContext[1] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "getColorRGBA", "(I)I", true);
        methods_NativeContext[2] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "nativeInitialize", "()V", false);
        methods_NativeContext[3] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "getDimension", "(I)F", true);
        methods_NativeContext[4] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "showToast", "(Ljava/lang/String;Z)V", true);
        methods_NativeContext[5] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "gc", "()V", true);
        methods_NativeContext[6] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "getAppContext", "()Landroid/content/Context;", false);
        methods_NativeContext[7] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "isUIThread", "()Z", true);
        methods_NativeContext[8] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "nativeGC", "()V", true);
        methods_NativeContext[9] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "getBootingId", "()Ljava/lang/String;", true);
        methods_NativeContext[10] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "getInteger", "(I)I", true);
        methods_NativeContext[11] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "getString", "(I)Ljava/lang/String;", true);
        methods_NativeContext[12] = ::ndk::JniWrapper::loadMethod(class_NativeContext, "isNativeLogOutput", "()Z", true);

    }
}

NativeContext::NativeContext(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeContext();
}

jboolean NativeContext::isNativeDebuggable() {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jboolean) env->CallStaticBooleanMethod(class_NativeContext, methods_NativeContext[0]);
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


jint NativeContext::getColorRGBA(jint color_id) {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jint) env->CallStaticIntMethod(class_NativeContext, methods_NativeContext[1], color_id);
}

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


jfloat NativeContext::getDimension(jint id) {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jfloat) env->CallStaticFloatMethod(class_NativeContext, methods_NativeContext[3], id);
}

void NativeContext::showToast(jstring message, jboolean longTime) {
    CALL_JNIENV();
    initialize_NativeContext();
    env->CallStaticVoidMethod(class_NativeContext, methods_NativeContext[4], message, longTime);
}

void NativeContext::gc() {
    CALL_JNIENV();
    initialize_NativeContext();
    env->CallStaticVoidMethod(class_NativeContext, methods_NativeContext[5]);
}

jobject NativeContext::getAppContext_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_NativeContext[6]);
}

jobject NativeContext::getAppContext_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jobject) env->CallObjectMethod(_this, methods_NativeContext[6]);
}

jboolean NativeContext::isUIThread() {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jboolean) env->CallStaticBooleanMethod(class_NativeContext, methods_NativeContext[7]);
}

void NativeContext::nativeGC() {
    CALL_JNIENV();
    initialize_NativeContext();
    env->CallStaticVoidMethod(class_NativeContext, methods_NativeContext[8]);
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


jstring NativeContext::getBootingId() {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jstring) env->CallStaticObjectMethod(class_NativeContext, methods_NativeContext[9]);
}

jint NativeContext::getInteger(jint integer_id) {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jint) env->CallStaticIntMethod(class_NativeContext, methods_NativeContext[10], integer_id);
}

jstring NativeContext::getString(jint id) {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jstring) env->CallStaticObjectMethod(class_NativeContext, methods_NativeContext[11], id);
}

jboolean NativeContext::isNativeLogOutput() {
    CALL_JNIENV();
    initialize_NativeContext();
    return (jboolean) env->CallStaticBooleanMethod(class_NativeContext, methods_NativeContext[12]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeContext.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_isNativeLogOutput(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_android_app_NativeContext_isNativeLogOutput(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_app_NativeContext_isNativeLogOutput");
    
    return (jboolean) 0;
}
#endif


jc_sp<NativeContext> NativeContext::wrap(jobject obj) {
    return jc_sp<NativeContext>( mark_new NativeContext(obj));
}

jc_sp<NativeContext> NativeContext::global(jobject obj) {
    return jc_sp<NativeContext>( (NativeContext*)(mark_new NativeContext(obj))->addGlobalRef());
}

jclass NativeContext::getClass() {
    initialize_NativeContext();
    return class_NativeContext;
}

}

