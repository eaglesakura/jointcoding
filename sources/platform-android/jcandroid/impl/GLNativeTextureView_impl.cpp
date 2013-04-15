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

static void rendering_async_func(ndk::GLNativeTextureViewContext *GLNativeTextureView_context) {
    jc::gl::MDevice device = GLNativeTextureView_context->getDevice();

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
            jclogf("abort thread(%x)", GLNativeTextureView_context);
            return;
        }
    }
}

static void rendering_check(jc_sp<ndk::GLNativeTextureViewContext> GLNativeTextureView_context) {
    jc::ThreadUtil::asyncFunction((jc::data_runnable_function)rendering_async_func, GLNativeTextureView_context, "rendering");
}

#endif

using namespace ndk;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext(JNIEnv *env, jobject _this, jint egl_flags) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onCreateNativeContext");

    GLNativeTextureViewContext *nativeContext = new GLNativeTextureViewContext(egl_flags);
    setJointContextPointer_Object(_this, Jointable::KEY_MAINCONTEXT, (Object*)nativeContext);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint int_1, jint int_2) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLInitialize");

    getJointContextPointer<GLNativeTextureViewContext>(_this, Jointable::KEY_MAINCONTEXT)->onGLInitialize(surfacetexture_0);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint width, jint height) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLSurfaceSizeChanged");

    getJointContextPointer<GLNativeTextureViewContext>(_this, Jointable::KEY_MAINCONTEXT)->onSurfaceSizeChanged(surfacetexture_0, width, height);

#ifdef  RENDERING_CHECK
    rendering_check(GLNativeTextureView_context);
#endif
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume(JNIEnv *env, jobject _this, jobject surfacetexture_0, jint width, jint height) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLResume");

    getJointContextPointer<GLNativeTextureViewContext>(_this, Jointable::KEY_MAINCONTEXT)->onSurfaceSizeChanged(surfacetexture_0, width, height);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed(JNIEnv *env, jobject _this, jobject surfacetexture_0) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onNativeGLDestroyed");

    getJointContextPointer<GLNativeTextureViewContext>(_this, Jointable::KEY_MAINCONTEXT)->onGLSuspend();
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_GLNativeTextureView_onDestroyNative");

    getJointContextPointer<GLNativeTextureViewContext>(_this, Jointable::KEY_MAINCONTEXT)->dispose();
}

}
