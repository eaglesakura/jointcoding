/**
 * export from "Joint Coding Project"
 */
#include "ndkGLNativeTextureView.h"

namespace ndk {

const ::jc::charactor* GLNativeTextureView::CLASS_SIGNATURE = "com/eaglesakura/jc/android/view/GLNativeTextureView";
const ::jc::s32 GLNativeTextureView::EGL_DEPTHBUFFER_ENABLE = 1;
const ::jc::s32 GLNativeTextureView::EGL_COLORBUFFER_BITS_DEFAULT = 0;

static jclass class_GLNativeTextureView = NULL;

#define methods_GLNativeTextureView_LENGTH 7

#if methods_GLNativeTextureView_LENGTH
static jmethodID methods_GLNativeTextureView[7];
#endif

static void initialize_GLNativeTextureView() {
    // loaded !
    if (class_GLNativeTextureView) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_GLNativeTextureView = env->FindClass(GLNativeTextureView::CLASS_SIGNATURE);
    class_GLNativeTextureView = (jclass)::ndk::change_globalref(env, class_GLNativeTextureView);
    

    // load methods
    {
        methods_GLNativeTextureView[0] = ::ndk::JniWrapper::loadMethod(class_GLNativeTextureView, "onNativeGLSurfaceSizeChanged", "(Landroid/graphics/SurfaceTexture;II)V", false);
        methods_GLNativeTextureView[1] = ::ndk::JniWrapper::loadMethod(class_GLNativeTextureView, "onCreateNativeContext", "(I)I", false);
        methods_GLNativeTextureView[2] = ::ndk::JniWrapper::loadMethod(class_GLNativeTextureView, "getNativeContextPointer", "()I", false);
        methods_GLNativeTextureView[3] = ::ndk::JniWrapper::loadMethod(class_GLNativeTextureView, "onNativeGLResume", "(Landroid/graphics/SurfaceTexture;II)V", false);
        methods_GLNativeTextureView[4] = ::ndk::JniWrapper::loadMethod(class_GLNativeTextureView, "onNativeGLInitialize", "(Landroid/graphics/SurfaceTexture;II)V", false);
        methods_GLNativeTextureView[5] = ::ndk::JniWrapper::loadMethod(class_GLNativeTextureView, "onDestroyNative", "()V", false);
        methods_GLNativeTextureView[6] = ::ndk::JniWrapper::loadMethod(class_GLNativeTextureView, "onNativeGLDestroyed", "(Landroid/graphics/SurfaceTexture;)V", false);

    }
}

GLNativeTextureView::GLNativeTextureView(jobject obj): ::ndk::JniWrapper(obj){
    initialize_GLNativeTextureView();
}

void GLNativeTextureView::onNativeGLSurfaceSizeChanged(jobject surfacetexture_0, jint int_1, jint int_2) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeTextureView[0], surfacetexture_0, int_1, int_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged");
    
    return;
}
#endif


void GLNativeTextureView::onNativeGLSurfaceSizeChanged_(jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    CALL_JNIENV();
    initialize_GLNativeTextureView();
    env->CallVoidMethod(_this, methods_GLNativeTextureView[0], surfacetexture_0, int_1, int_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged");
    
    return;
}
#endif


jint GLNativeTextureView::onCreateNativeContext(jint int_0) {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_GLNativeTextureView[1], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext");
    
    return (jint) 0;
}
#endif


jint GLNativeTextureView::onCreateNativeContext_(jobject _this, jint int_0) {
    CALL_JNIENV();
    initialize_GLNativeTextureView();
    return (jint) env->CallIntMethod(_this, methods_GLNativeTextureView[1], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext");
    
    return (jint) 0;
}
#endif


jint GLNativeTextureView::getNativeContextPointer() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_GLNativeTextureView[2]);
}

jint GLNativeTextureView::getNativeContextPointer_(jobject _this) {
    CALL_JNIENV();
    initialize_GLNativeTextureView();
    return (jint) env->CallIntMethod(_this, methods_GLNativeTextureView[2]);
}

void GLNativeTextureView::onNativeGLResume(jobject surfacetexture_0, jint int_1, jint int_2) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeTextureView[3], surfacetexture_0, int_1, int_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume");
    
    return;
}
#endif


void GLNativeTextureView::onNativeGLResume_(jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    CALL_JNIENV();
    initialize_GLNativeTextureView();
    env->CallVoidMethod(_this, methods_GLNativeTextureView[3], surfacetexture_0, int_1, int_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume");
    
    return;
}
#endif


void GLNativeTextureView::onNativeGLInitialize(jobject surfacetexture_0, jint int_1, jint int_2) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeTextureView[4], surfacetexture_0, int_1, int_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize");
    
    return;
}
#endif


void GLNativeTextureView::onNativeGLInitialize_(jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    CALL_JNIENV();
    initialize_GLNativeTextureView();
    env->CallVoidMethod(_this, methods_GLNativeTextureView[4], surfacetexture_0, int_1, int_2);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize");
    
    return;
}
#endif


void GLNativeTextureView::onDestroyNative() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeTextureView[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative");
    
    return;
}
#endif


void GLNativeTextureView::onDestroyNative_(jobject _this) {
    CALL_JNIENV();
    initialize_GLNativeTextureView();
    env->CallVoidMethod(_this, methods_GLNativeTextureView[5]);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative(JNIEnv *env, jobject _this);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative");
    
    return;
}
#endif


void GLNativeTextureView::onNativeGLDestroyed(jobject surfacetexture_0) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_GLNativeTextureView[6], surfacetexture_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed(JNIEnv *env, jobject _this, jobject surfacetexture_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed(JNIEnv *env, jobject _this, jobject surfacetexture_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed");
    
    return;
}
#endif


void GLNativeTextureView::onNativeGLDestroyed_(jobject _this, jobject surfacetexture_0) {
    CALL_JNIENV();
    initialize_GLNativeTextureView();
    env->CallVoidMethod(_this, methods_GLNativeTextureView[6], surfacetexture_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkGLNativeTextureView.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed(JNIEnv *env, jobject _this, jobject surfacetexture_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed(JNIEnv *env, jobject _this, jobject surfacetexture_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed");
    
    return;
}
#endif


jc_sp<GLNativeTextureView> GLNativeTextureView::wrap(jobject obj) {
    return jc_sp<GLNativeTextureView>( new GLNativeTextureView(obj));
}

jc_sp<GLNativeTextureView> GLNativeTextureView::global(jobject obj) {
    return jc_sp<GLNativeTextureView>( (GLNativeTextureView*)(new GLNativeTextureView(obj))->addGlobalRef());
}

jclass GLNativeTextureView::getClass() {
    initialize_GLNativeTextureView();
    return class_GLNativeTextureView;
}

}

