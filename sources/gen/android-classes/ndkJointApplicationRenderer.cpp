/**
 * export from "Joint Coding Project"
 */
#include "ndkJointApplicationRenderer.h"

namespace ndk {

const ::jc::charactor* JointApplicationRenderer::CLASS_SIGNATURE = "com/eaglesakura/jc/framework/app/JointApplicationRenderer";
const ::jc::s32 JointApplicationRenderer::KEY_MAINCONTEXT = 0;

static jclass class_JointApplicationRenderer = NULL;

#define methods_JointApplicationRenderer_LENGTH 13

#if methods_JointApplicationRenderer_LENGTH
static jmethodID methods_JointApplicationRenderer[13];
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
        methods_JointApplicationRenderer[0] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "dispatchTouchEvent", "(Lcom/eaglesakura/jcprotocol/TouchEventProtocol;)V", false);
        methods_JointApplicationRenderer[1] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativeMainLoop", "()V", false);
        methods_JointApplicationRenderer[2] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "queryIntParams", "(II[I)Z", false);
        methods_JointApplicationRenderer[3] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "setNativePointer", "(ILcom/eaglesakura/jc/jni/Pointer;)V", false);
        methods_JointApplicationRenderer[4] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "postStringParams", "(II[Ljava/lang/String;)Z", false);
        methods_JointApplicationRenderer[5] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "createSlaveDevice", "()Lcom/eaglesakura/jc/egl/DeviceManager;", false);
        methods_JointApplicationRenderer[6] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "receiveStringParams", "(II[Ljava/lang/String;)Z", false);
        methods_JointApplicationRenderer[7] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativeNewtask", "(II)V", false);
        methods_JointApplicationRenderer[8] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "onNativeInitialize", "()V", false);
        methods_JointApplicationRenderer[9] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "queryStringParams", "(II[Ljava/lang/String;)Z", false);
        methods_JointApplicationRenderer[10] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "getNativePointer", "(I)Lcom/eaglesakura/jc/jni/Pointer;", false);
        methods_JointApplicationRenderer[11] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "getWindowDevice", "()Lcom/eaglesakura/jc/egl/DeviceManager;", false);
        methods_JointApplicationRenderer[12] = ::ndk::JniWrapper::loadMethod(class_JointApplicationRenderer, "startNewtask", "(II)V", false);

    }
}

JointApplicationRenderer::JointApplicationRenderer(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JointApplicationRenderer();
}

void JointApplicationRenderer::dispatchTouchEvent(jobject toucheventprotocol_0) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[0], toucheventprotocol_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_dispatchTouchEvent(JNIEnv *env, jobject _this, jobject toucheventprotocol_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_dispatchTouchEvent(JNIEnv *env, jobject _this, jobject toucheventprotocol_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_dispatchTouchEvent");
    
    return;
}
#endif


void JointApplicationRenderer::dispatchTouchEvent_(jobject _this, jobject toucheventprotocol_0) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[0], toucheventprotocol_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_dispatchTouchEvent(JNIEnv *env, jobject _this, jobject toucheventprotocol_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_dispatchTouchEvent(JNIEnv *env, jobject _this, jobject toucheventprotocol_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_dispatchTouchEvent");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeMainLoop() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[1]);
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
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[1]);
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


jboolean JointApplicationRenderer::queryIntParams(jint int_0, jint int_1, jintArray int_array_2) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_JointApplicationRenderer[2], int_0, int_1, int_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams");
    
    return (jboolean) 0;
}
#endif


jboolean JointApplicationRenderer::queryIntParams_(jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jboolean) env->CallBooleanMethod(_this, methods_JointApplicationRenderer[2], int_0, int_1, int_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jintArray int_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams");
    
    return (jboolean) 0;
}
#endif


void JointApplicationRenderer::setNativePointer(jint key, jobject ptr) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[3], key, ptr);
}

void JointApplicationRenderer::setNativePointer_(jobject _this, jint key, jobject ptr) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[3], key, ptr);
}

jboolean JointApplicationRenderer::postStringParams(jint int_0, jint int_1, jobjectArray string_array_2) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_JointApplicationRenderer[4], int_0, int_1, string_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postStringParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jobjectArray string_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postStringParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jobjectArray string_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postStringParams");
    
    return (jboolean) 0;
}
#endif


jboolean JointApplicationRenderer::postStringParams_(jobject _this, jint int_0, jint int_1, jobjectArray string_array_2) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jboolean) env->CallBooleanMethod(_this, methods_JointApplicationRenderer[4], int_0, int_1, string_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postStringParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jobjectArray string_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postStringParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jobjectArray string_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postStringParams");
    
    return (jboolean) 0;
}
#endif


jobject JointApplicationRenderer::createSlaveDevice_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointApplicationRenderer[5]);
}

jobject JointApplicationRenderer::createSlaveDevice_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jobject) env->CallObjectMethod(_this, methods_JointApplicationRenderer[5]);
}

jboolean JointApplicationRenderer::receiveStringParams(jint main_key, jint sub_key, jobjectArray params) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_JointApplicationRenderer[6], main_key, sub_key, params);
}

jboolean JointApplicationRenderer::receiveStringParams_(jobject _this, jint main_key, jint sub_key, jobjectArray params) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jboolean) env->CallBooleanMethod(_this, methods_JointApplicationRenderer[6], main_key, sub_key, params);
}

void JointApplicationRenderer::onNativeNewtask(jint int_0, jint int_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[7], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeNewtask(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeNewtask(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeNewtask");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeNewtask_(jobject _this, jint int_0, jint int_1) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[7], int_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeNewtask(JNIEnv *env, jobject _this, jint int_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeNewtask(JNIEnv *env, jobject _this, jint int_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeNewtask");
    
    return;
}
#endif


void JointApplicationRenderer::onNativeInitialize() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[8]);
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
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[8]);
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


jboolean JointApplicationRenderer::queryStringParams(jint int_0, jint int_1, jobjectArray string_array_2) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_JointApplicationRenderer[9], int_0, int_1, string_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryStringParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jobjectArray string_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryStringParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jobjectArray string_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryStringParams");
    
    return (jboolean) 0;
}
#endif


jboolean JointApplicationRenderer::queryStringParams_(jobject _this, jint int_0, jint int_1, jobjectArray string_array_2) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jboolean) env->CallBooleanMethod(_this, methods_JointApplicationRenderer[9], int_0, int_1, string_array_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJointApplicationRenderer.h"

extern "C" {
// prototype
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryStringParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jobjectArray string_array_2);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryStringParams(JNIEnv *env, jobject _this, jint int_0, jint int_1, jobjectArray string_array_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryStringParams");
    
    return (jboolean) 0;
}
#endif


jobject JointApplicationRenderer::getNativePointer_unsafe(jint key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointApplicationRenderer[10], key);
}

jobject JointApplicationRenderer::getNativePointer_unsafe_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jobject) env->CallObjectMethod(_this, methods_JointApplicationRenderer[10], key);
}

jobject JointApplicationRenderer::getWindowDevice_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointApplicationRenderer[11]);
}

jobject JointApplicationRenderer::getWindowDevice_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    return (jobject) env->CallObjectMethod(_this, methods_JointApplicationRenderer[11]);
}

void JointApplicationRenderer::startNewtask(jint taskId, jint userData) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointApplicationRenderer[12], taskId, userData);
}

void JointApplicationRenderer::startNewtask_(jobject _this, jint taskId, jint userData) {
    CALL_JNIENV();
    initialize_JointApplicationRenderer();
    env->CallVoidMethod(_this, methods_JointApplicationRenderer[12], taskId, userData);
}

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

