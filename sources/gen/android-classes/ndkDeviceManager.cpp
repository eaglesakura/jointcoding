/**
 * export from "Joint Coding Project"
 */
#include "ndkDeviceManager.h"

namespace ndk {

const ::jc::charactor* DeviceManager::CLASS_SIGNATURE = "com/eaglesakura/jc/egl/DeviceManager";
const ::jc::s32 DeviceManager::KEY_MAINCONTEXT = 0;

static jclass class_DeviceManager = NULL;

#define methods_DeviceManager_LENGTH 8

#if methods_DeviceManager_LENGTH
static jmethodID methods_DeviceManager[8];
#endif

static void initialize_DeviceManager() {
    // loaded !
    if (class_DeviceManager) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_DeviceManager = env->FindClass(DeviceManager::CLASS_SIGNATURE);
    class_DeviceManager = (jclass)::ndk::change_globalref(env, class_DeviceManager);
    

    // load methods
    {
        methods_DeviceManager[0] = ::ndk::JniWrapper::loadMethod(class_DeviceManager, "getEGLContextWrapper", "()Lcom/eaglesakura/jc/egl/EGLContextWrapper;", false);
        methods_DeviceManager[1] = ::ndk::JniWrapper::loadMethod(class_DeviceManager, "setNativePointer", "(ILcom/eaglesakura/jc/jni/Pointer;)V", false);
        methods_DeviceManager[2] = ::ndk::JniWrapper::loadMethod(class_DeviceManager, "getNativePointer", "(I)Lcom/eaglesakura/jc/jni/Pointer;", false);
        methods_DeviceManager[3] = ::ndk::JniWrapper::loadMethod(class_DeviceManager, "getEGLWrapper", "()Lcom/eaglesakura/jc/egl/EGLWrapper;", false);
        methods_DeviceManager[4] = ::ndk::JniWrapper::loadMethod(class_DeviceManager, "onNativeEndOperation", "()V", false);
        methods_DeviceManager[5] = ::ndk::JniWrapper::loadMethod(class_DeviceManager, "onNativeBeginOperation", "()V", false);
        methods_DeviceManager[6] = ::ndk::JniWrapper::loadMethod(class_DeviceManager, "createNative", "()V", false);
        methods_DeviceManager[7] = ::ndk::JniWrapper::loadMethod(class_DeviceManager, "getEGLSurfaceWrapper", "()Lcom/eaglesakura/jc/egl/EGLSurfaceWrapper;", false);

    }
}

DeviceManager::DeviceManager(jobject obj): ::ndk::JniWrapper(obj){
    initialize_DeviceManager();
}

jobject DeviceManager::getEGLContextWrapper_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_DeviceManager[0]);
}

jobject DeviceManager::getEGLContextWrapper_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_DeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_DeviceManager[0]);
}

void DeviceManager::setNativePointer(jint key, jobject ptr) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_DeviceManager[1], key, ptr);
}

void DeviceManager::setNativePointer_(jobject _this, jint key, jobject ptr) {
    CALL_JNIENV();
    initialize_DeviceManager();
    env->CallVoidMethod(_this, methods_DeviceManager[1], key, ptr);
}

jobject DeviceManager::getNativePointer_unsafe(jint key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_DeviceManager[2], key);
}

jobject DeviceManager::getNativePointer_unsafe_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_DeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_DeviceManager[2], key);
}

jobject DeviceManager::getEGLWrapper_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_DeviceManager[3]);
}

jobject DeviceManager::getEGLWrapper_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_DeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_DeviceManager[3]);
}

void DeviceManager::onNativeEndOperation() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_DeviceManager[4]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_onNativeEndOperation(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_onNativeEndOperation(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_DeviceManager_onNativeEndOperation");
    
    return;
}
#endif


void DeviceManager::onNativeEndOperation_(jobject _this) {
    CALL_JNIENV();
    initialize_DeviceManager();
    env->CallVoidMethod(_this, methods_DeviceManager[4]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_onNativeEndOperation(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_onNativeEndOperation(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_DeviceManager_onNativeEndOperation");
    
    return;
}
#endif


void DeviceManager::onNativeBeginOperation() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_DeviceManager[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_onNativeBeginOperation(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_onNativeBeginOperation(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_DeviceManager_onNativeBeginOperation");
    
    return;
}
#endif


void DeviceManager::onNativeBeginOperation_(jobject _this) {
    CALL_JNIENV();
    initialize_DeviceManager();
    env->CallVoidMethod(_this, methods_DeviceManager[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_onNativeBeginOperation(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_onNativeBeginOperation(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_DeviceManager_onNativeBeginOperation");
    
    return;
}
#endif


void DeviceManager::createNative() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_DeviceManager[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_createNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_createNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_DeviceManager_createNative");
    
    return;
}
#endif


void DeviceManager::createNative_(jobject _this) {
    CALL_JNIENV();
    initialize_DeviceManager();
    env->CallVoidMethod(_this, methods_DeviceManager[6]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkDeviceManager.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_createNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_egl_DeviceManager_createNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_egl_DeviceManager_createNative");
    
    return;
}
#endif


jobject DeviceManager::getEGLSurfaceWrapper_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_DeviceManager[7]);
}

jobject DeviceManager::getEGLSurfaceWrapper_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_DeviceManager();
    return (jobject) env->CallObjectMethod(_this, methods_DeviceManager[7]);
}

jc_sp<DeviceManager> DeviceManager::wrap(jobject obj) {
    return jc_sp<DeviceManager>( new DeviceManager(obj));
}

jc_sp<DeviceManager> DeviceManager::global(jobject obj) {
    return jc_sp<DeviceManager>( (DeviceManager*)(new DeviceManager(obj))->addGlobalRef());
}

jclass DeviceManager::getClass() {
    initialize_DeviceManager();
    return class_DeviceManager;
}

}

