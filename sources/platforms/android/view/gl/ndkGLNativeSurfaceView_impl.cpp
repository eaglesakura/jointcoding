/*
 * ndkGLNativeSurfaceView.cpp
 *
 *  Created on: 2012/09/20
 */

#include    "jointcoding-android.h"
#include    "ndkGLNativeSurfaceView.h"
#include    "ndkGLNativeSurfaceViewContext.h"

// #define RENDERING_CHECK

#ifdef RENDERING_CHECK
#include    "jcglDeviceLock.h"
#include    "jcThreadUtil.h"

using namespace jc;
static u32 g_index = 0;

static void rendering_async_func(ndk::GLNativeSurfaceViewContext *context) {
    jc::gl::MDevice device = context->getDevice();

    u32 current = (g_index++);

    while (true) {
        // ロックを得る前に、デバイスのロックリクエストを処理する
        device->waitLockRequest(10, NULL);

        jc::gl::DeviceLock lock(device);
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

static void rendering_check(jc_sp<ndk::GLNativeSurfaceViewContext> context) {
    jc::ThreadUtil::asyncFunction((jc::data_runnable_function)rendering_async_func, context, "rendering");
}

#endif

using namespace ndk;

extern "C" {

#define context GLNativeSurfaceViewContext::getNativeContext(_this)

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceCreated(JNIEnv *env, jobject _this, jobject holder, jobject surface) {
    // call env reset
    initJniEnv(env);
    context->onSurfaceCreated(surface);
    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceChanged(JNIEnv *env, jobject _this, jobject holder, jobject surface, jint pixFormat, jint width, jint height) {
    // call env reset
    initJniEnv(env);

    context->onSurfaceChanged(surface, pixFormat, width, height);
#ifdef RENDERING_CHECK
    {
        rendering_check(context);
    }
#endif

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onSurfaceDestroyed(JNIEnv *env, jobject _this, jobject holder, jobject surface) {
    // call env reset
    initJniEnv(env);

    context->onSurfaceDestroyed(surface);
    return;
}

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onInitializeNative(JNIEnv *env, jobject _this, jint egl_flags) {
    // call env reset
    initJniEnv(env);

    GLNativeSurfaceViewContext *viewContext = new GLNativeSurfaceViewContext(egl_flags);
    return (jint) new jc_sp<GLNativeSurfaceViewContext>(viewContext);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_GLNativeSurfaceView_onDestroyNative(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    context->dispose();

#ifdef  RENDERING_CHECK
    g_index = 0;
#endif

    return;
}
}

