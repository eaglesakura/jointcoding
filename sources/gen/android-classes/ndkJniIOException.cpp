/**
 * export from "Joint Coding Project"
 */
#include "ndkJniIOException.h"

namespace ndk {

const ::jc::charactor* JniIOException::CLASS_SIGNATURE = "java/lang/IOException";

static jclass class_JniIOException = NULL;

#define methods_JniIOException_LENGTH 2

#if methods_JniIOException_LENGTH
static jmethodID methods_JniIOException[2];
#endif

static void initialize_JniIOException() {
    // loaded !
    if (class_JniIOException) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JniIOException = env->FindClass(JniIOException::CLASS_SIGNATURE);
    class_JniIOException = (jclass)::ndk::change_globalref(env, class_JniIOException);
    

    // load methods
    {
        methods_JniIOException[0] = ::ndk::JniWrapper::loadMethod(class_JniIOException, "printStackTrace", "()V", false);
        methods_JniIOException[1] = ::ndk::JniWrapper::loadMethod(class_JniIOException, "getMessage", "()Ljava/lang/String;", false);

    }
}

JniIOException::JniIOException(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JniIOException();
}

void JniIOException::printStackTrace() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JniIOException[0]);
}

void JniIOException::printStackTrace_(jobject _this) {
    CALL_JNIENV();
    initialize_JniIOException();
    env->CallVoidMethod(_this, methods_JniIOException[0]);
}

jstring JniIOException::getMessage() {
    CALL_JNIENV();
    return (jstring) env->CallObjectMethod(this->getObject(), methods_JniIOException[1]);
}

jstring JniIOException::getMessage_(jobject _this) {
    CALL_JNIENV();
    initialize_JniIOException();
    return (jstring) env->CallObjectMethod(_this, methods_JniIOException[1]);
}

jc_sp<JniIOException> JniIOException::wrap(jobject obj) {
    return jc_sp<JniIOException>( mark_new JniIOException(obj));
}

jc_sp<JniIOException> JniIOException::global(jobject obj) {
    return jc_sp<JniIOException>( (JniIOException*)(mark_new JniIOException(obj))->addGlobalRef());
}

jclass JniIOException::getClass() {
    initialize_JniIOException();
    return class_JniIOException;
}

}

