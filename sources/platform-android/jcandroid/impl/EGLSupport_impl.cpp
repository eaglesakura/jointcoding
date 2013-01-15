/*
 * EGLSupport_impl.cpp
 *
 *  Created on: 2012/12/06
 */

#include "jointcoding-android.h"
#include "android-gclasses/EGLSupport.h"

extern "C" {

// main
JNIEXPORT jobject JNICALL Java_com_google_android_gles_1jni_EGLSupport_newSupportClassNative(JNIEnv *env, jobject _this, jstring eglClassName, jint state) {
    // call env reset
    initJniEnv(env);

    char const *pEglClassName = env->GetStringUTFChars(eglClassName, NULL);
    jclogf("EGLClassName(%s)", pEglClassName);

    // create instance
    {
        jclass clazz = env->FindClass(pEglClassName);
        env->ReleaseStringUTFChars(eglClassName, pEglClassName);
        if (!clazz) {
            jclog("class not found!");
            return NULL;
        }

        jmethodID method = env->GetMethodID(clazz, "<init>", "(I)V");
        if (!method) {
            jclog("method not found!!");
            return NULL;
        }

        return env->NewObject(clazz, method, state);
    }
    // native not found...
    return (jobject) 0;
}

// main
JNIEXPORT jint JNICALL Java_com_google_android_gles_1jni_EGLSupport_getIntFieldNative(JNIEnv *env, jobject _this, jclass clazz, jobject obj, jstring fieldName) {
    // call env reset
    initJniEnv(env);

    char const *pFieldName = env->GetStringUTFChars(fieldName, NULL);
    jfieldID field = env->GetFieldID(clazz, pFieldName, "I");
    env->ReleaseStringUTFChars(fieldName, pFieldName);

    if( !field ) {
        jclog("field not found !!");
    }

    return env->GetIntField(obj, field);
}

}

