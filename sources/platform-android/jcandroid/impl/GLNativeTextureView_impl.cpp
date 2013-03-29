/*
 * ndkGLNativeTextureView_impl.cpp
 *
 *  Created on: 2012/12/05
 */
#include "jointcoding-android.h"
#include "android-classes/ndkGLNativeTextureView.h"
#include "jcandroid/view/GLNativeTextureViewContext.h"

// #define RENDERING_CHECK

#ifdef RENDERING_CHECK
#include    "jcglDeviceLock.h"
#include    "jcThreadUtil.h"

using namespace jc;
static u32 g_index = 0;

static void rendering_async_func(ndk::GLNativeTextureViewContext *context) {
    jc::gl::MDevice device = context->getDevice();

    u32 current = (g_index++);

    while (true) {
        // ロックを得る前に、デバイスのロックリクエストを処理する
        device->waitLockRequest(10, NULL);

        jc::gl::DeviceLock lock(device, jcfalse);
        if (lock.isLockCompleted()) {
            if (current == 0) {
                static int color = 0;
                float cf = (float) ((color++) % 256) / 255.0f;
                glClearColor(cf, cf, cf, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            } else {
                glClearColor((float) (rand() % 256) / 255.0f, (float) (rand() % 256) / 255.0f, (float) (rand() % 256) / 255.0f, 1);
                glClear(GL_COLOR_BUFFER_BIT);
            }

            glClearColor(1, 1, 1, 1);
            device->postFrontBuffer();
        } else {
            jclogf("abort thread(%x)", context);
            return;
        }
    }
}

static void rendering_check(jc_sp<ndk::GLNativeTextureViewContext> context) {
    jc::ThreadUtil::asyncFunction((jc::data_runnable_function)rendering_async_func, context, "rendering");
}

#endif

using namespace ndk;

extern "C" {

#define context GLNativeTextureViewContext::getNativeContext(_this)

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext(JNIEnv *env, jobject _this, jint egl_flags) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext");

    GLNativeTextureViewContext *nativeContext = new GLNativeTextureViewContext(egl_flags);
    return (jint) new jc_sp<GLNativeTextureViewContext>(nativeContext);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize");

    context->onGLInitialize(surfacetexture_0);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint width, jint height) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged");

    context->onSurfaceSizeChanged(surfacetexture_0, width, height);

#ifdef  RENDERING_CHECK
    rendering_check(context);
#endif
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint width, jint height) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume");

    context->onSurfaceSizeChanged(surfacetexture_0, width, height);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed(JNIEnv *env, jobject _this, jobject surfacetexture_0) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed");

    context->onGLSuspend();
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative");

    context->dispose();
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_lockEGL(JNIEnv *env, jobject _this) {
    // add code.
//    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_lockEGL");

    if(!context->lockEGL()) {
        jclog("lock failed...");
    }

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_unlockEGL(JNIEnv *env, jobject _this) {
    // add code.
//    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_unlockEGL");
    context->unlockEGL();
    return;
}
}
