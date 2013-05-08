/**
 * export from "Joint Coding Project"
 */
#include "ndkEGLWrapper.h"

namespace ndk {

const ::jc::charactor* EGLWrapper::CLASS_SIGNATURE = "com/eaglesakura/jc/android/egl/EGLWrapper";

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
        methods_EGLWrapper[1] = ::ndk::JniWrapper::loadMethod(class_EGLWrapper, "postFrontBuffer", "(Lcom/eaglesakura/jc/android/egl/EGLSurfaceWrapper;)Z", false);
        methods_EGLWrapper[2] = ::ndk::JniWrapper::loadMethod(class_EGLWrapper, "current", "(Lcom/eaglesakura/jc/android/egl/EGLContextWrapper;Lcom/eaglesakura/jc/android/egl/EGLSurfaceWrapper;)Z", false);

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

jboolean EGLWrapper::postFrontBuffer(jobject surface) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_EGLWrapper[1], surface);
}

jboolean EGLWrapper::postFrontBuffer_(jobject _this, jobject surface) {
    CALL_JNIENV();
    initialize_EGLWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_EGLWrapper[1], surface);
}

jboolean EGLWrapper::current(jobject context, jobject surface) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_EGLWrapper[2], context, surface);
}

jboolean EGLWrapper::current_(jobject _this, jobject context, jobject surface) {
    CALL_JNIENV();
    initialize_EGLWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_EGLWrapper[2], context, surface);
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

