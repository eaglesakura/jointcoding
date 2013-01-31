/**
 * export from "Joint Coding Project"
 */
#include "EGLSupport.h"

const ::jc::charactor* EGLSupport::CLASS_SIGNATURE = "com/google/android/gles_jni/EGLSupport";

static jclass class_EGLSupport = NULL;

#define methods_EGLSupport_LENGTH 5

#if methods_EGLSupport_LENGTH
static jmethodID methods_EGLSupport[5];
#endif

static void initialize_EGLSupport() {
    // loaded !
    if (class_EGLSupport) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_EGLSupport = env->FindClass(EGLSupport::CLASS_SIGNATURE);
    class_EGLSupport = (jclass)::ndk::change_globalref(env, class_EGLSupport);
    

    // load methods
    {
        methods_EGLSupport[0] = ::ndk::JniWrapper::loadMethod(class_EGLSupport, "getIntFieldNative", "(Ljava/lang/Class;Ljava/lang/Object;Ljava/lang/String;)I", true);
        methods_EGLSupport[1] = ::ndk::JniWrapper::loadMethod(class_EGLSupport, "newSupportClassNative", "(Ljava/lang/String;I)Ljava/lang/Object;", true);
        methods_EGLSupport[2] = ::ndk::JniWrapper::loadMethod(class_EGLSupport, "unlockEGLMakeCurrent", "(IIII)V", true);
        methods_EGLSupport[3] = ::ndk::JniWrapper::loadMethod(class_EGLSupport, "unlockEGLMakeCurrentNative", "(IIII)V", true);
        methods_EGLSupport[4] = ::ndk::JniWrapper::loadMethod(class_EGLSupport, "eglCreateWindowSurfaceSupport", "(IILandroid/graphics/SurfaceTexture;)I", true);

    }
}

EGLSupport::EGLSupport(jobject obj): ::ndk::JniWrapper(obj){
    initialize_EGLSupport();
}

jint EGLSupport::getIntFieldNative(jobject class_0, jobject object_1, jstring string_2) {
    CALL_JNIENV();
    initialize_EGLSupport();
    return (jint) env->CallStaticIntMethod(class_EGLSupport, methods_EGLSupport[0], class_0, object_1, string_2);
}
#if 0
#include "jointcoding-android.h"
#include "EGLSupport.h"

extern "C" {
// prototype
JNIEXPORT jint JNICALL Java_com_google_android_gles_jni_EGLSupport_getIntFieldNative(JNIEnv *env, jobject _this, jobject class_0, jobject object_1, jstring string_2);
}

// main
JNIEXPORT jint JNICALL Java_com_google_android_gles_jni_EGLSupport_getIntFieldNative(JNIEnv *env, jobject _this, jobject class_0, jobject object_1, jstring string_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_google_android_gles_jni_EGLSupport_getIntFieldNative");
    
    return (jint) 0;
}
#endif


jobject EGLSupport::newSupportClassNative_unsafe(jstring string_0, jint int_1) {
    CALL_JNIENV();
    initialize_EGLSupport();
    return (jobject) env->CallStaticObjectMethod(class_EGLSupport, methods_EGLSupport[1], string_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "EGLSupport.h"

extern "C" {
// prototype
JNIEXPORT jobject JNICALL Java_com_google_android_gles_jni_EGLSupport_newSupportClassNative_unsafe(JNIEnv *env, jobject _this, jstring string_0, jint int_1);
}

// main
JNIEXPORT jobject JNICALL Java_com_google_android_gles_jni_EGLSupport_newSupportClassNative_unsafe(JNIEnv *env, jobject _this, jstring string_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_google_android_gles_jni_EGLSupport_newSupportClassNative_unsafe");
    
    return (jobject) 0;
}
#endif


void EGLSupport::unlockEGLMakeCurrent(jint display, jint draw_surface, jint read_surface, jint context) {
    CALL_JNIENV();
    initialize_EGLSupport();
    env->CallStaticVoidMethod(class_EGLSupport, methods_EGLSupport[2], display, draw_surface, read_surface, context);
}

void EGLSupport::unlockEGLMakeCurrentNative(jint int_0, jint int_1, jint int_2, jint int_3) {
    CALL_JNIENV();
    initialize_EGLSupport();
    env->CallStaticVoidMethod(class_EGLSupport, methods_EGLSupport[3], int_0, int_1, int_2, int_3);
}
#if 0
#include "jointcoding-android.h"
#include "EGLSupport.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_google_android_gles_jni_EGLSupport_unlockEGLMakeCurrentNative(JNIEnv *env, jobject _this, jint int_0, jint int_1, jint int_2, jint int_3);
}

// main
JNIEXPORT void JNICALL Java_com_google_android_gles_jni_EGLSupport_unlockEGLMakeCurrentNative(JNIEnv *env, jobject _this, jint int_0, jint int_1, jint int_2, jint int_3) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_google_android_gles_jni_EGLSupport_unlockEGLMakeCurrentNative");
    
    return;
}
#endif


jint EGLSupport::eglCreateWindowSurfaceSupport(jint display, jint config, jobject surfaceTexture) {
    CALL_JNIENV();
    initialize_EGLSupport();
    return (jint) env->CallStaticIntMethod(class_EGLSupport, methods_EGLSupport[4], display, config, surfaceTexture);
}

jc_sp<EGLSupport> EGLSupport::wrap(jobject obj) {
    return jc_sp<EGLSupport>( new EGLSupport(obj));
}

jc_sp<EGLSupport> EGLSupport::global(jobject obj) {
    return jc_sp<EGLSupport>( (EGLSupport*)(new EGLSupport(obj))->addGlobalRef());
}

jclass EGLSupport::getClass() {
    initialize_EGLSupport();
    return class_EGLSupport;
}
