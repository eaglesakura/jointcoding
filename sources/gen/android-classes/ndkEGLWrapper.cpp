/**
 * export from "Joint Coding Project"
 */
#include "ndkEGLWrapper.h"

namespace ndk {

const ::jc::charactor* EGLWrapper::CLASS_SIGNATURE = "com/eaglesakura/jc/egl/EGLWrapper";

static jclass class_EGLWrapper = NULL;

#define methods_EGLWrapper_LENGTH 3

#if methods_EGLWrapper_LENGTH
static jmethodID methods_EGLWrapper[3];
#endif

static void initialize_EGLWrapper() {
    // loaded !
    if (class_EGLWrapper) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_EGLWrapper = env->FindClass(EGLWrapper::CLASS_SIGNATURE);
    class_EGLWrapper = (jclass)::ndk::change_globalref(env, class_EGLWrapper);
    

    // load methods
    {
        methods_EGLWrapper[0] = ::ndk::JniWrapper::loadMethod(class_EGLWrapper, "dispose", "()V", false);
        methods_EGLWrapper[1] = ::ndk::JniWrapper::loadMethod(class_EGLWrapper, "current", "(Lcom/eaglesakura/jc/egl/EGLContextWrapper;Lcom/eaglesakura/jc/egl/EGLSurfaceWrapper;)Z", false);
        methods_EGLWrapper[2] = ::ndk::JniWrapper::loadMethod(class_EGLWrapper, "postFrontBuffer", "(Lcom/eaglesakura/jc/egl/EGLSurfaceWrapper;)Z", false);

    }
}

EGLWrapper::EGLWrapper(jobject obj): ::ndk::JniWrapper(obj){
    initialize_EGLWrapper();
}

void EGLWrapper::dispose() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_EGLWrapper[0]);
}

void EGLWrapper::dispose_(jobject _this) {
    CALL_JNIENV();
    initialize_EGLWrapper();
    env->CallVoidMethod(_this, methods_EGLWrapper[0]);
}

jboolean EGLWrapper::current(jobject eglSurface, jobject result) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_EGLWrapper[1], eglSurface, result);
}

jboolean EGLWrapper::current_(jobject _this, jobject eglSurface, jobject result) {
    CALL_JNIENV();
    initialize_EGLWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_EGLWrapper[1], eglSurface, result);
}

jboolean EGLWrapper::postFrontBuffer(jobject currentSurface) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_EGLWrapper[2], currentSurface);
}

jboolean EGLWrapper::postFrontBuffer_(jobject _this, jobject currentSurface) {
    CALL_JNIENV();
    initialize_EGLWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_EGLWrapper[2], currentSurface);
}

jc_sp<EGLWrapper> EGLWrapper::wrap(jobject obj) {
    return jc_sp<EGLWrapper>( new EGLWrapper(obj));
}

jc_sp<EGLWrapper> EGLWrapper::global(jobject obj) {
    return jc_sp<EGLWrapper>( (EGLWrapper*)(new EGLWrapper(obj))->addGlobalRef());
}

jclass EGLWrapper::getClass() {
    initialize_EGLWrapper();
    return class_EGLWrapper;
}

}

