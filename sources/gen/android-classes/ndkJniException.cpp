/**
 * export from "Joint Coding Project"
 */
#include "ndkJniException.h"

namespace ndk {

const ::jc::charactor* JniException::CLASS_SIGNATURE = "java/lang/Exception";

static jclass class_JniException = NULL;

#define methods_JniException_LENGTH 2

#if methods_JniException_LENGTH
static jmethodID methods_JniException[2];
#endif

static void initialize_JniException() {
    // loaded !
    if (class_JniException) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JniException = env->FindClass(JniException::CLASS_SIGNATURE);
    class_JniException = (jclass)::ndk::change_globalref(env, class_JniException);
    

    // load methods
    {
        methods_JniException[0] = ::ndk::JniWrapper::loadMethod(class_JniException, "printStackTrace", "()V", false);
        methods_JniException[1] = ::ndk::JniWrapper::loadMethod(class_JniException, "getMessage", "()Ljava/lang/String;", false);

    }
}

JniException::JniException(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JniException();
}

void JniException::printStackTrace() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JniException[0]);
}

void JniException::printStackTrace_(jobject _this) {
    CALL_JNIENV();
    initialize_JniException();
    env->CallVoidMethod(_this, methods_JniException[0]);
}

jstring JniException::getMessage() {
    CALL_JNIENV();
    return (jstring) env->CallObjectMethod(this->getObject(), methods_JniException[1]);
}

jstring JniException::getMessage_(jobject _this) {
    CALL_JNIENV();
    initialize_JniException();
    return (jstring) env->CallObjectMethod(_this, methods_JniException[1]);
}

jc_sp<JniException> JniException::wrap(jobject obj) {
    return jc_sp<JniException>( new JniException(obj));
}

jc_sp<JniException> JniException::global(jobject obj) {
    return jc_sp<JniException>( (JniException*)(new JniException(obj))->addGlobalRef());
}

jclass JniException::getClass() {
    initialize_JniException();
    return class_JniException;
}

}

