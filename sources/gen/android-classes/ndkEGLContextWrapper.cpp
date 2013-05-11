/**
 * export from "Joint Coding Project"
 */
#include "ndkEGLContextWrapper.h"

namespace ndk {

const ::jc::charactor* EGLContextWrapper::CLASS_SIGNATURE = "com/eaglesakura/jc/egl/EGLContextWrapper";

static jclass class_EGLContextWrapper = NULL;

#define methods_EGLContextWrapper_LENGTH 1

#if methods_EGLContextWrapper_LENGTH
static jmethodID methods_EGLContextWrapper[1];
#endif

static void initialize_EGLContextWrapper() {
    // loaded !
    if (class_EGLContextWrapper) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_EGLContextWrapper = env->FindClass(EGLContextWrapper::CLASS_SIGNATURE);
    class_EGLContextWrapper = (jclass)::ndk::change_globalref(env, class_EGLContextWrapper);
    

    // load methods
    {
        methods_EGLContextWrapper[0] = ::ndk::JniWrapper::loadMethod(class_EGLContextWrapper, "dispose", "()V", false);

    }
}

EGLContextWrapper::EGLContextWrapper(jobject obj): ::ndk::JniWrapper(obj){
    initialize_EGLContextWrapper();
}

void EGLContextWrapper::dispose() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_EGLContextWrapper[0]);
}

void EGLContextWrapper::dispose_(jobject _this) {
    CALL_JNIENV();
    initialize_EGLContextWrapper();
    env->CallVoidMethod(_this, methods_EGLContextWrapper[0]);
}

jc_sp<EGLContextWrapper> EGLContextWrapper::wrap(jobject obj) {
    return jc_sp<EGLContextWrapper>( new EGLContextWrapper(obj));
}

jc_sp<EGLContextWrapper> EGLContextWrapper::global(jobject obj) {
    return jc_sp<EGLContextWrapper>( (EGLContextWrapper*)(new EGLContextWrapper(obj))->addGlobalRef());
}

jclass EGLContextWrapper::getClass() {
    initialize_EGLContextWrapper();
    return class_EGLContextWrapper;
}

}

