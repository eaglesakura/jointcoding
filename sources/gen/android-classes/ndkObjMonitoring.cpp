/**
 * export from "Joint Coding Project"
 */
#include "ndkObjMonitoring.h"

namespace ndk {

const ::jc::charactor* ObjMonitoring::CLASS_SIGNATURE = "com/eaglesakura/jc/android/resource/jni/ObjMonitoring";

static jclass class_ObjMonitoring = NULL;

#define methods_ObjMonitoring_LENGTH 2

#if methods_ObjMonitoring_LENGTH
static jmethodID methods_ObjMonitoring[2];
#endif

static void initialize_ObjMonitoring() {
    // loaded !
    if (class_ObjMonitoring) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_ObjMonitoring = env->FindClass(ObjMonitoring::CLASS_SIGNATURE);
    class_ObjMonitoring = (jclass)::ndk::change_globalref(env, class_ObjMonitoring);
    

    // load methods
    {
        methods_ObjMonitoring[0] = ::ndk::JniWrapper::loadMethod(class_ObjMonitoring, "createInstance", "(Ljava/lang/Object;)Lcom/eaglesakura/jc/android/resource/jni/ObjMonitoring;", true);
        methods_ObjMonitoring[1] = ::ndk::JniWrapper::loadMethod(class_ObjMonitoring, "exist", "()Z", false);

    }
}

ObjMonitoring::ObjMonitoring(jobject obj): ::ndk::JniWrapper(obj){
    initialize_ObjMonitoring();
}

jobject ObjMonitoring::createInstance_unsafe(jobject target) {
    CALL_JNIENV();
    initialize_ObjMonitoring();
    return (jobject) env->CallStaticObjectMethod(class_ObjMonitoring, methods_ObjMonitoring[0], target);
}

jboolean ObjMonitoring::exist() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_ObjMonitoring[1]);
}

jboolean ObjMonitoring::exist_(jobject _this) {
    CALL_JNIENV();
    initialize_ObjMonitoring();
    return (jboolean) env->CallBooleanMethod(_this, methods_ObjMonitoring[1]);
}

jc_sp<ObjMonitoring> ObjMonitoring::wrap(jobject obj) {
    return jc_sp<ObjMonitoring>( new ObjMonitoring(obj));
}

jc_sp<ObjMonitoring> ObjMonitoring::global(jobject obj) {
    return jc_sp<ObjMonitoring>( (ObjMonitoring*)(new ObjMonitoring(obj))->addGlobalRef());
}

jclass ObjMonitoring::getClass() {
    initialize_ObjMonitoring();
    return class_ObjMonitoring;
}

}

