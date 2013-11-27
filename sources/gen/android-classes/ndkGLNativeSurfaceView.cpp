/**
 * export from "Joint Coding Project"
 */
#include "ndkGLNativeSurfaceView.h"

namespace ndk {

const ::jc::charactor* GLNativeSurfaceView::CLASS_SIGNATURE = "com/eaglesakura/jc/android/view/GLNativeSurfaceView";
const ::jc::s32 GLNativeSurfaceView::KEY_MAINCONTEXT = 0;
const ::jc::s32 GLNativeSurfaceView::EGL_DEPTHBUFFER_ENABLE = 1;
const ::jc::s32 GLNativeSurfaceView::EGL_COLORBUFFER_BITS_DEFAULT = 0;

static jclass class_GLNativeSurfaceView = NULL;

#define methods_GLNativeSurfaceView_LENGTH 9

#if methods_GLNativeSurfaceView_LENGTH
static jmethodID methods_GLNativeSurfaceView[9];
#endif

static void initialize_GLNativeSurfaceView() {
    // loaded !
    if (class_GLNativeSurfaceView) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_GLNativeSurfaceView = env->FindClass(GLNativeSurfaceView::CLASS_SIGNATURE);
    class_GLNativeSurfaceView = (jclass)::ndk::change_globalref(env, class_GLNativeSurfaceView);
    

    // load methods
    {
        methods_GLNativeSurfaceView[0] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "onSurfaceCreated", "(Landroid/view/SurfaceHolder;Landroid/view/Surface;)V", false);
        methods_GLNativeSurfaceView[1] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "getNativeContextPointer", "()I", false);
        methods_GLNativeSurfaceView[2] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "onSurfaceChanged", "(Landroid/view/SurfaceHolder;Landroid/view/Surface;III)V", false);
        methods_GLNativeSurfaceView[3] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "getNativeContextPointer", "(Lcom/eaglesakura/jc/android/view/GLNativeSurfaceView;)I", true);
        methods_GLNativeSurfaceView[4] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "getNativePointer", "(I)Lcom/eaglesakura/jc/android/resource/jni/Pointer;", false);
        methods_GLNativeSurfaceView[5] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "onDestroyNative", "()V", false);
        methods_GLNativeSurfaceView[6] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "onInitializeNative", "(I)I", false);
        methods_GLNativeSurfaceView[7] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "setNativePointer", "(ILcom/eaglesakura/jc/android/resource/jni/Pointer;)V", false);
        methods_GLNativeSurfaceView[8] = ::ndk::JniWrapper::loadMethod(class_GLNativeSurfaceView, "onSurfaceDestroyed", "(Landroid/view/SurfaceHolder;Landroid/view/Surface;)V", false);

    }
}

GLNativeSurfaceView::GLNativeSurfaceView(jobject obj): ::ndk::JniWrapper(obj){
    initialize_GLNativeSurfaceView();
}

void GLNativeSurfaceView::onSurfaceCreated(jobject surfaceholder_0, jobject surface_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeSurfaceView[0], surfaceholder_0, surface_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceCreated(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceCreated(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceCreated");
    
    return;
}
#endif


void GLNativeSurfaceView::onSurfaceCreated_(jobject _this, jobject surfaceholder_0, jobject surface_1) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    env->CallVoidMethod(_this, methods_GLNativeSurfaceView[0], surfaceholder_0, surface_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceCreated(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceCreated(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceCreated");
    
    return;
}
#endif


jint GLNativeSurfaceView::getNativeContextPointer() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_GLNativeSurfaceView[1]);
}

jint GLNativeSurfaceView::getNativeContextPointer_(jobject _this) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    return (jint) env->CallIntMethod(_this, methods_GLNativeSurfaceView[1]);
}

void GLNativeSurfaceView::onSurfaceChanged(jobject surfaceholder_0, jobject surface_1, jint int_2, jint int_3, jint int_4) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeSurfaceView[2], surfaceholder_0, surface_1, int_2, int_3, int_4);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceChanged(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1, jint int_2, jint int_3, jint int_4);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceChanged(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1, jint int_2, jint int_3, jint int_4) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceChanged");
    
    return;
}
#endif


void GLNativeSurfaceView::onSurfaceChanged_(jobject _this, jobject surfaceholder_0, jobject surface_1, jint int_2, jint int_3, jint int_4) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    env->CallVoidMethod(_this, methods_GLNativeSurfaceView[2], surfaceholder_0, surface_1, int_2, int_3, int_4);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceChanged(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1, jint int_2, jint int_3, jint int_4);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceChanged(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1, jint int_2, jint int_3, jint int_4) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceChanged");
    
    return;
}
#endif


jint GLNativeSurfaceView::getNativeContextPointer(jobject view) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    return (jint) env->CallStaticIntMethod(class_GLNativeSurfaceView, methods_GLNativeSurfaceView[3], view);
}

jobject GLNativeSurfaceView::getNativePointer_unsafe(jint key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_GLNativeSurfaceView[4], key);
}

jobject GLNativeSurfaceView::getNativePointer_unsafe_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    return (jobject) env->CallObjectMethod(_this, methods_GLNativeSurfaceView[4], key);
}

void GLNativeSurfaceView::onDestroyNative() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeSurfaceView[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onDestroyNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onDestroyNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onDestroyNative");
    
    return;
}
#endif


void GLNativeSurfaceView::onDestroyNative_(jobject _this) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    env->CallVoidMethod(_this, methods_GLNativeSurfaceView[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onDestroyNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onDestroyNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onDestroyNative");
    
    return;
}
#endif


jint GLNativeSurfaceView::onInitializeNative(jint int_0) {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_GLNativeSurfaceView[6], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onInitializeNative(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onInitializeNative(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onInitializeNative");
    
    return (jint) 0;
}
#endif


jint GLNativeSurfaceView::onInitializeNative_(jobject _this, jint int_0) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    return (jint) env->CallIntMethod(_this, methods_GLNativeSurfaceView[6], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onInitializeNative(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onInitializeNative(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onInitializeNative");
    
    return (jint) 0;
}
#endif


void GLNativeSurfaceView::setNativePointer(jint key, jobject ptr) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeSurfaceView[7], key, ptr);
}

void GLNativeSurfaceView::setNativePointer_(jobject _this, jint key, jobject ptr) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    env->CallVoidMethod(_this, methods_GLNativeSurfaceView[7], key, ptr);
}

void GLNativeSurfaceView::onSurfaceDestroyed(jobject surfaceholder_0, jobject surface_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeSurfaceView[8], surfaceholder_0, surface_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceDestroyed(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceDestroyed(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceDestroyed");
    
    return;
}
#endif


void GLNativeSurfaceView::onSurfaceDestroyed_(jobject _this, jobject surfaceholder_0, jobject surface_1) {
    CALL_JNIENV();
    initialize_GLNativeSurfaceView();
    env->CallVoidMethod(_this, methods_GLNativeSurfaceView[8], surfaceholder_0, surface_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeSurfaceView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceDestroyed(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceDestroyed(JNIEnv *env, jobject _this, jobject surfaceholder_0, jobject surface_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceDestroyed");
    
    return;
}
#endif


jc_sp<GLNativeSurfaceView> GLNativeSurfaceView::wrap(jobject obj) {
    return jc_sp<GLNativeSurfaceView>( mark_new GLNativeSurfaceView(obj));
}

jc_sp<GLNativeSurfaceView> GLNativeSurfaceView::global(jobject obj) {
    return jc_sp<GLNativeSurfaceView>( (GLNativeSurfaceView*)(mark_new GLNativeSurfaceView(obj))->addGlobalRef());
}

jclass GLNativeSurfaceView::getClass() {
    initialize_GLNativeSurfaceView();
    return class_GLNativeSurfaceView;
}

}

