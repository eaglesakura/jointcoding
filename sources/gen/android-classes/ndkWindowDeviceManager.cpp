/**
 * export from "Joint Coding Project"
 */
#include "ndkWindowDeviceManager.h"

namespace ndk {

const ::jc::charactor* WindowDeviceManager::CLASS_SIGNATURE = "com/eaglesakura/jc/egl/WindowDeviceManager";
const ::jc::s32 WindowDeviceManager::KEY_MAINCONTEXT = 0;

static jclass class_WindowDeviceManager = NULL;

#define methods_WindowDeviceManager_LENGTH 9

#if methods_WindowDeviceManager_LENGTH
static jmethodID methods_WindowDeviceManager[9];
#endif

static void initialize_WindowDeviceManager() {
    // loaded !
    if (class_WindowDeviceManager) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_WindowDeviceManager = env->FindClass(WindowDeviceManager::CLASS_SIGNATURE);
    class_WindowDeviceManager = (jclass)::ndk::change_globalref(env, class_WindowDeviceManager);
    

    // load methods
    {
        methods_WindowDeviceManager[0] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "getEGLContextWrapper", "()Lcom/eaglesakura/jc/egl/EGLContextWrapper;", false);
        methods_WindowDeviceManager[1] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "setNativePointer", "(ILcom/eaglesakura/jc/jni/Pointer;)V", false);
        methods_WindowDeviceManager[2] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "getEGLWrapper", "()Lcom/eaglesakura/jc/egl/EGLWrapper;", false);
        methods_WindowDeviceManager[3] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "createSlaveDevice", "()Lcom/eaglesakura/jc/egl/DeviceManager;", false);
        methods_WindowDeviceManager[4] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "getNativePointer", "(I)Lcom/eaglesakura/jc/jni/Pointer;", false);
        methods_WindowDeviceManager[5] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "onNativeEndOperation", "()V", false);
        methods_WindowDeviceManager[6] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "onNativeBeginOperation", "()V", false);
        methods_WindowDeviceManager[7] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "createNative", "()V", false);
        methods_WindowDeviceManager[8] = ::ndk::JniWrapper::loadMethod(class_WindowDeviceManager, "getEGLSurfaceWrapper", "()Lcom/eaglesakura/jc/egl/EGLSurfaceWrapper;", false);

    }
}

WindowDeviceManager::WindowDeviceManager(jobject obj): ::ndk::JniWrapper(obj){
    initialize_WindowDeviceManager();
}

jobject WindowDeviceManager::getEGLContextWrapper_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_WindowDeviceManager[0]);
}

jobject WindowDeviceManager::getEGLContextWrapper_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_WindowDeviceManager[0]);
}

void WindowDeviceManager::setNativePointer(jint key, jobject ptr) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_WindowDeviceManager[1], key, ptr);
}

void WindowDeviceManager::setNativePointer_(jobject _this, jint key, jobject ptr) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    env->CallVoidMethod(_this, methods_WindowDeviceManager[1], key, ptr);
}

jobject WindowDeviceManager::getEGLWrapper_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_WindowDeviceManager[2]);
}

jobject WindowDeviceManager::getEGLWrapper_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_WindowDeviceManager[2]);
}

jobject WindowDeviceManager::createSlaveDevice_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_WindowDeviceManager[3]);
}

jobject WindowDeviceManager::createSlaveDevice_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_WindowDeviceManager[3]);
}

jobject WindowDeviceManager::getNativePointer_unsafe(jint key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_WindowDeviceManager[4], key);
}

jobject WindowDeviceManager::getNativePointer_unsafe_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_WindowDeviceManager[4], key);
}

void WindowDeviceManager::onNativeEndOperation() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_WindowDeviceManager[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkWindowDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeEndOperation(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeEndOperation(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeEndOperation");
    
    return;
}
#endif


void WindowDeviceManager::onNativeEndOperation_(jobject _this) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    env->CallVoidMethod(_this, methods_WindowDeviceManager[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkWindowDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeEndOperation(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeEndOperation(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeEndOperation");
    
    return;
}
#endif


void WindowDeviceManager::onNativeBeginOperation() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_WindowDeviceManager[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkWindowDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeBeginOperation(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeBeginOperation(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeBeginOperation");
    
    return;
}
#endif


void WindowDeviceManager::onNativeBeginOperation_(jobject _this) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    env->CallVoidMethod(_this, methods_WindowDeviceManager[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkWindowDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeBeginOperation(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeBeginOperation(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_WindowDeviceManager_onNativeBeginOperation");
    
    return;
}
#endif


void WindowDeviceManager::createNative() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_WindowDeviceManager[7]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkWindowDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_createNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_createNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_WindowDeviceManager_createNative");
    
    return;
}
#endif


void WindowDeviceManager::createNative_(jobject _this) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    env->CallVoidMethod(_this, methods_WindowDeviceManager[7]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkWindowDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_createNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_WindowDeviceManager_createNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_WindowDeviceManager_createNative");
    
    return;
}
#endif


jobject WindowDeviceManager::getEGLSurfaceWrapper_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_WindowDeviceManager[8]);
}

jobject WindowDeviceManager::getEGLSurfaceWrapper_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_WindowDeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_WindowDeviceManager[8]);
}

jc_sp<WindowDeviceManager> WindowDeviceManager::wrap(jobject obj) {
    return jc_sp<WindowDeviceManager>( new WindowDeviceManager(obj));
}

jc_sp<WindowDeviceManager> WindowDeviceManager::global(jobject obj) {
    return jc_sp<WindowDeviceManager>( (WindowDeviceManager*)(new WindowDeviceManager(obj))->addGlobalRef());
}

jclass WindowDeviceManager::getClass() {
    initialize_WindowDeviceManager();
    return class_WindowDeviceManager;
}

}

