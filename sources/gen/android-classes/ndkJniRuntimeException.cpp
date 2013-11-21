/**
 * export from "Joint Coding Project"
 */
#include "ndkJniRuntimeException.h"

namespace ndk {

const ::jc::charactor* JniRuntimeException::CLASS_SIGNATURE = "java/lang/RuntimeException";

static jclass class_JniRuntimeException = NULL;

#define methods_JniRuntimeException_LENGTH 2

#if methods_JniRuntimeException_LENGTH
static jmethodID methods_JniRuntimeException[2];
#endif

static void initialize_JniRuntimeException() {
    // loaded !
    if (class_JniRuntimeException) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JniRuntimeException = env->FindClass(JniRuntimeException::CLASS_SIGNATURE);
    class_JniRuntimeException = (jclass)::ndk::change_globalref(env, class_JniRuntimeException);
    

    // load methods
    {
        methods_JniRuntimeException[0] = ::ndk::JniWrapper::loadMethod(class_JniRuntimeException, "printStackTrace", "()V", false);
        methods_JniRuntimeException[1] = ::ndk::JniWrapper::loadMethod(class_JniRuntimeException, "getMessage", "()Ljava/lang/String;", false);

    }
}

JniRuntimeException::JniRuntimeException(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JniRuntimeException();
}

void JniRuntimeException::printStackTrace() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JniRuntimeException[0]);
}

void JniRuntimeException::printStackTrace_(jobject _this) {
    CALL_JNIENV();
    initialize_JniRuntimeException();
    env->CallVoidMethod(_this, methods_JniRuntimeException[0]);
}

jstring JniRuntimeException::getMessage() {
    CALL_JNIENV();
    return (jstring) env->CallObjectMethod(this->getObject(), methods_JniRuntimeException[1]);
}

jstring JniRuntimeException::getMessage_(jobject _this) {
    CALL_JNIENV();
    initialize_JniRuntimeException();
    return (jstring) env->CallObjectMethod(_this, methods_JniRuntimeException[1]);
}

jc_sp<JniRuntimeException> JniRuntimeException::wrap(jobject obj) {
    return jc_sp<JniRuntimeException>( mark_new JniRuntimeException(obj));
}

jc_sp<JniRuntimeException> JniRuntimeException::global(jobject obj) {
    return jc_sp<JniRuntimeException>( (JniRuntimeException*)(mark_new JniRuntimeException(obj))->addGlobalRef());
}

jclass JniRuntimeException::getClass() {
    initialize_JniRuntimeException();
    return class_JniRuntimeException;
}

}

