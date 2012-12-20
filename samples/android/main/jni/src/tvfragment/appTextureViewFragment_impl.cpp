/*
 * appTextureViewFragment_impl.cpp
 *
 *  Created on: 2012/12/05
 */

#include "jointcoding-android.h"
#include "appTextureViewFragment.h"

#include    "jcandroid/fragment/NativeFragmentBase.h"

using namespace ndk;
class TextureViewFragmentNative: public NativeFragmentBase {
public:
};

#define fragment    native_fragment_ptr(AppFragmentNative, _this)

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLSurfaceSizeChanged(JNIEnv *env, jobject _this, jobject glnativetextureview_0, jint int_1, jint int_2) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLSurfaceSizeChanged");

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLPauseCompleted(JNIEnv *env, jobject _this, jobject glnativetextureview_0) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLPauseCompleted");

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLResumeCompleted(JNIEnv *env, jobject _this, jobject glnativetextureview_0) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLResumeCompleted");

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLPauseBegin(JNIEnv *env, jobject _this, jobject glnativetextureview_0) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLPauseBegin");

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLInitializeCompleted(JNIEnv *env, jobject _this, jobject glnativetextureview_0) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_onEGLInitializeCompleted");

    return;
}

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_newState(JNIEnv *env, jobject _this) {
    // call env reset
    initJniEnv(env);

    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_sample_hello_gl_TextureViewFragment_newState");

    TextureViewFragmentNative *state = new TextureViewFragmentNative();
    return (jint) new jc_sp<jc::Object>(state);
}

}

