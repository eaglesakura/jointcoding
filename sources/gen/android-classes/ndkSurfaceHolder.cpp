/**
 * export from "Joint Coding Project"
 */
#include "ndkSurfaceHolder.h"

namespace ndk {

const ::jc::charactor* SurfaceHolder::CLASS_SIGNATURE = "android/view/SurfaceHolder";

static jclass class_SurfaceHolder = NULL;

#define methods_SurfaceHolder_LENGTH 1

#if methods_SurfaceHolder_LENGTH
static jmethodID methods_SurfaceHolder[1];
#endif

static void initialize_SurfaceHolder() {
    // loaded !
    if (class_SurfaceHolder) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_SurfaceHolder = env->FindClass(SurfaceHolder::CLASS_SIGNATURE);
    class_SurfaceHolder = (jclass)::ndk::change_globalref(env, class_SurfaceHolder);
    

    // load methods
    {
        methods_SurfaceHolder[0] = ::ndk::JniWrapper::loadMethod(class_SurfaceHolder, "getSurface", "()Landroid/view/Surface;", false);

    }
}

SurfaceHolder::SurfaceHolder(jobject obj): ::ndk::JniWrapper(obj){
    initialize_SurfaceHolder();
}

jobject SurfaceHolder::getSurface_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_SurfaceHolder[0]);
}

jobject SurfaceHolder::getSurface_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_SurfaceHolder();
    return (jobject) env->CallObjectMethod(_this, methods_SurfaceHolder[0]);
}

jc_sp<SurfaceHolder> SurfaceHolder::wrap(jobject obj) {
    return jc_sp<SurfaceHolder>( new SurfaceHolder(obj));
}

jc_sp<SurfaceHolder> SurfaceHolder::global(jobject obj) {
    return jc_sp<SurfaceHolder>( (SurfaceHolder*)(new SurfaceHolder(obj))->addGlobalRef());
}

jclass SurfaceHolder::getClass() {
    initialize_SurfaceHolder();
    return class_SurfaceHolder;
}

}

