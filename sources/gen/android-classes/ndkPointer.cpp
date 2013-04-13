/**
 * export from "Joint Coding Project"
 */
#include "ndkPointer.h"

namespace ndk {

const ::jc::charactor* Pointer::CLASS_SIGNATURE = "com/eaglesakura/jc/android/resource/jni/Pointer";

static jclass class_Pointer = NULL;

#define methods_Pointer_LENGTH 6

#if methods_Pointer_LENGTH
static jmethodID methods_Pointer[6];
#endif

static void initialize_Pointer() {
    // loaded !
    if (class_Pointer) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_Pointer = env->FindClass(Pointer::CLASS_SIGNATURE);
    class_Pointer = (jclass)::ndk::change_globalref(env, class_Pointer);
    

    // load methods
    {
        methods_Pointer[0] = ::ndk::JniWrapper::loadMethod(class_Pointer, "deleteSharedObjectPtr", "(I)V", true);
        methods_Pointer[1] = ::ndk::JniWrapper::loadMethod(class_Pointer, "dispose", "()V", false);
        methods_Pointer[2] = ::ndk::JniWrapper::loadMethod(class_Pointer, "getSharedObjectPtr", "(I)I", true);
        methods_Pointer[3] = ::ndk::JniWrapper::loadMethod(class_Pointer, "deleteObjectPtr", "(I)V", true);
        methods_Pointer[4] = ::ndk::JniWrapper::loadMethod(class_Pointer, "deleteVoidPtr", "(I)V", true);
        methods_Pointer[5] = ::ndk::JniWrapper::loadMethod(class_Pointer, "getObjectPointer", "()I", false);

    }
}

Pointer::Pointer(jobject obj): ::ndk::JniWrapper(obj){
    initialize_Pointer();
}

void Pointer::deleteSharedObjectPtr(jint int_0) {
    CALL_JNIENV();
    initialize_Pointer();
    env->CallStaticVoidMethod(class_Pointer, methods_Pointer[0], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkPointer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteSharedObjectPtr(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteSharedObjectPtr(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteSharedObjectPtr");
    
    return;
}
#endif


void Pointer::dispose() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_Pointer[1]);
}

void Pointer::dispose_(jobject _this) {
    CALL_JNIENV();
    initialize_Pointer();
    env->CallVoidMethod(_this, methods_Pointer[1]);
}

jint Pointer::getSharedObjectPtr(jint int_0) {
    CALL_JNIENV();
    initialize_Pointer();
    return (jint) env->CallStaticIntMethod(class_Pointer, methods_Pointer[2], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkPointer.h"

extern "C" {
// prototype
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_getSharedObjectPtr(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_getSharedObjectPtr(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_resource_jni_Pointer_getSharedObjectPtr");
    
    return (jint) 0;
}
#endif


void Pointer::deleteObjectPtr(jint int_0) {
    CALL_JNIENV();
    initialize_Pointer();
    env->CallStaticVoidMethod(class_Pointer, methods_Pointer[3], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkPointer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteObjectPtr(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteObjectPtr(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteObjectPtr");
    
    return;
}
#endif


void Pointer::deleteVoidPtr(jint int_0) {
    CALL_JNIENV();
    initialize_Pointer();
    env->CallStaticVoidMethod(class_Pointer, methods_Pointer[4], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkPointer.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteVoidPtr(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteVoidPtr(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteVoidPtr");
    
    return;
}
#endif


jint Pointer::getObjectPointer() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_Pointer[5]);
}

jint Pointer::getObjectPointer_(jobject _this) {
    CALL_JNIENV();
    initialize_Pointer();
    return (jint) env->CallIntMethod(_this, methods_Pointer[5]);
}

jc_sp<Pointer> Pointer::wrap(jobject obj) {
    return jc_sp<Pointer>( new Pointer(obj));
}

jc_sp<Pointer> Pointer::global(jobject obj) {
    return jc_sp<Pointer>( (Pointer*)(new Pointer(obj))->addGlobalRef());
}

jclass Pointer::getClass() {
    initialize_Pointer();
    return class_Pointer;
}

}

