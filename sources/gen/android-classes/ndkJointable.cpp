/**
 * export from "Joint Coding Project"
 */
#include "ndkJointable.h"

namespace ndk {

const ::jc::charactor* Jointable::CLASS_SIGNATURE = "com/eaglesakura/jc/Jointable";
const ::jc::s32 Jointable::KEY_MAINCONTEXT = 0;

static jclass class_Jointable = NULL;

#define methods_Jointable_LENGTH 2

#if methods_Jointable_LENGTH
static jmethodID methods_Jointable[2];
#endif

static void initialize_Jointable() {
    // loaded !
    if (class_Jointable) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_Jointable = env->FindClass(Jointable::CLASS_SIGNATURE);
    class_Jointable = (jclass)::ndk::change_globalref(env, class_Jointable);
    

    // load methods
    {
        methods_Jointable[0] = ::ndk::JniWrapper::loadMethod(class_Jointable, "getNativePointer", "(I)Lcom/eaglesakura/jc/jni/Pointer;", false);
        methods_Jointable[1] = ::ndk::JniWrapper::loadMethod(class_Jointable, "setNativePointer", "(ILcom/eaglesakura/jc/jni/Pointer;)V", false);

    }
}

Jointable::Jointable(jobject obj): ::ndk::JniWrapper(obj){
    initialize_Jointable();
}

jobject Jointable::getNativePointer_unsafe(jint int_0) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_Jointable[0], int_0);
}

jobject Jointable::getNativePointer_unsafe_(jobject _this, jint int_0) {
    CALL_JNIENV();
    initialize_Jointable();
    return (jobject) env->CallObjectMethod(_this, methods_Jointable[0], int_0);
}

void Jointable::setNativePointer(jint int_0, jobject pointer_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_Jointable[1], int_0, pointer_1);
}

void Jointable::setNativePointer_(jobject _this, jint int_0, jobject pointer_1) {
    CALL_JNIENV();
    initialize_Jointable();
    env->CallVoidMethod(_this, methods_Jointable[1], int_0, pointer_1);
}

jc_sp<Jointable> Jointable::wrap(jobject obj) {
    return jc_sp<Jointable>( new Jointable(obj));
}

jc_sp<Jointable> Jointable::global(jobject obj) {
    return jc_sp<Jointable>( (Jointable*)(new Jointable(obj))->addGlobalRef());
}

jclass Jointable::getClass() {
    initialize_Jointable();
    return class_Jointable;
}

}

