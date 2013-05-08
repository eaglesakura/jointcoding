/**
 * export from "Joint Coding Project"
 */
#include "ndkEGLSurfaceWrapper.h"

namespace ndk {

const ::jc::charactor* EGLSurfaceWrapper::CLASS_SIGNATURE = "com/eaglesakura/jc/android/egl/EGLSurfaceWrapper";

static jclass class_EGLSurfaceWrapper = NULL;

#define methods_EGLSurfaceWrapper_LENGTH 3

#if methods_EGLSurfaceWrapper_LENGTH
static jmethodID methods_EGLSurfaceWrapper[3];
#endif

static void initialize_EGLSurfaceWrapper() {
    // loaded !
    if (class_EGLSurfaceWrapper) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_EGLSurfaceWrapper = env->FindClass(EGLSurfaceWrapper::CLASS_SIGNATURE);
    class_EGLSurfaceWrapper = (jclass)::ndk::change_globalref(env, class_EGLSurfaceWrapper);
    

    // load methods
    {
        methods_EGLSurfaceWrapper[0] = ::ndk::JniWrapper::loadMethod(class_EGLSurfaceWrapper, "dispose", "()V", false);
        methods_EGLSurfaceWrapper[1] = ::ndk::JniWrapper::loadMethod(class_EGLSurfaceWrapper, "getHeight", "()I", false);
        methods_EGLSurfaceWrapper[2] = ::ndk::JniWrapper::loadMethod(class_EGLSurfaceWrapper, "getWidth", "()I", false);

    }
}

EGLSurfaceWrapper::EGLSurfaceWrapper(jobject obj): ::ndk::JniWrapper(obj){
    initialize_EGLSurfaceWrapper();
}

void EGLSurfaceWrapper::dispose() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_EGLSurfaceWrapper[0]);
}

void EGLSurfaceWrapper::dispose_(jobject _this) {
    CALL_JNIENV();
    initialize_EGLSurfaceWrapper();
    env->CallVoidMethod(_this, methods_EGLSurfaceWrapper[0]);
}

jint EGLSurfaceWrapper::getHeight() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_EGLSurfaceWrapper[1]);
}

jint EGLSurfaceWrapper::getHeight_(jobject _this) {
    CALL_JNIENV();
    initialize_EGLSurfaceWrapper();
    return (jint) env->CallIntMethod(_this, methods_EGLSurfaceWrapper[1]);
}

jint EGLSurfaceWrapper::getWidth() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_EGLSurfaceWrapper[2]);
}

jint EGLSurfaceWrapper::getWidth_(jobject _this) {
    CALL_JNIENV();
    initialize_EGLSurfaceWrapper();
    return (jint) env->CallIntMethod(_this, methods_EGLSurfaceWrapper[2]);
}

jc_sp<EGLSurfaceWrapper> EGLSurfaceWrapper::wrap(jobject obj) {
    return jc_sp<EGLSurfaceWrapper>( new EGLSurfaceWrapper(obj));
}

jc_sp<EGLSurfaceWrapper> EGLSurfaceWrapper::global(jobject obj) {
    return jc_sp<EGLSurfaceWrapper>( (EGLSurfaceWrapper*)(new EGLSurfaceWrapper(obj))->addGlobalRef());
}

jclass EGLSurfaceWrapper::getClass() {
    initialize_EGLSurfaceWrapper();
    return class_EGLSurfaceWrapper;
}

}

