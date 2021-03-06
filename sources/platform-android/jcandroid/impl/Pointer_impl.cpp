/*
 * Pointer_jni.cpp
 *
 *  Created on: 2012/06/21
 *      Author: Takeshi
 */

#include "jointcoding-android.h"
#include "android-classes/ndkPointer.h"

using namespace jc;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteSharedObjectPtr(JNIEnv *env, jobject _this, jint pointer) {
    // call env reset
    initJniEnv(env);

    jc_sp<Object> *ptr = ( jc_sp<Object>*)pointer;
    jclogf("delete shared_ptr<Object>* = %x", ptr);
    SAFE_DELETE(ptr);

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteObjectPtr(JNIEnv *env, jobject _this, jint pointer) {
    // call env reset
    initJniEnv(env);

    Object* ptr = (Object*) pointer;
    jclogf("delete Object* = %x", ptr);
    SAFE_DELETE(ptr);

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_deleteVoidPtr(JNIEnv *env, jobject _this, jint pointer) {
    // call env reset
    initJniEnv(env);

    u8* ptr = (u8*) pointer;
    jclogf("delete void* = %x", ptr);
    SAFE_DELETE(ptr);

    return;
}

/**
 * ナマのポインタを取得する
 */JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_resource_jni_Pointer_getSharedObjectPtr(JNIEnv *env, jobject _this, jint pointer) {
    // call env reset
    initJniEnv(env);

    jc_sp<Object> *ptr = ( jc_sp<Object>*)pointer;

    return (jint) ptr->get();
}

}
