/**
 * export from "Joint Coding Project"
 */
#include "ndkJniInputStream.h"

namespace ndk {

const ::jc::charactor* JniInputStream::CLASS_SIGNATURE = "java/io/InputStream";

static jclass class_JniInputStream = NULL;

#define methods_JniInputStream_LENGTH 6

#if methods_JniInputStream_LENGTH
static jmethodID methods_JniInputStream[6];
#endif

static void initialize_JniInputStream() {
    // loaded !
    if (class_JniInputStream) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JniInputStream = env->FindClass(JniInputStream::CLASS_SIGNATURE);
    class_JniInputStream = (jclass)::ndk::change_globalref(env, class_JniInputStream);
    

    // load methods
    {
        methods_JniInputStream[0] = ::ndk::JniWrapper::loadMethod(class_JniInputStream, "close", "()V", false);
        methods_JniInputStream[1] = ::ndk::JniWrapper::loadMethod(class_JniInputStream, "read", "()I", false);
        methods_JniInputStream[2] = ::ndk::JniWrapper::loadMethod(class_JniInputStream, "available", "()I", false);
        methods_JniInputStream[3] = ::ndk::JniWrapper::loadMethod(class_JniInputStream, "read", "([B)I", false);
        methods_JniInputStream[4] = ::ndk::JniWrapper::loadMethod(class_JniInputStream, "read", "([BII)I", false);
        methods_JniInputStream[5] = ::ndk::JniWrapper::loadMethod(class_JniInputStream, "skip", "(J)J", false);

    }
}

JniInputStream::JniInputStream(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JniInputStream();
}

void JniInputStream::close() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JniInputStream[0]);
}

void JniInputStream::close_(jobject _this) {
    CALL_JNIENV();
    initialize_JniInputStream();
    env->CallVoidMethod(_this, methods_JniInputStream[0]);
}

jint JniInputStream::read() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_JniInputStream[1]);
}

jint JniInputStream::read_(jobject _this) {
    CALL_JNIENV();
    initialize_JniInputStream();
    return (jint) env->CallIntMethod(_this, methods_JniInputStream[1]);
}

jint JniInputStream::available() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_JniInputStream[2]);
}

jint JniInputStream::available_(jobject _this) {
    CALL_JNIENV();
    initialize_JniInputStream();
    return (jint) env->CallIntMethod(_this, methods_JniInputStream[2]);
}

jint JniInputStream::read(jbyteArray buffer) {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_JniInputStream[3], buffer);
}

jint JniInputStream::read_(jobject _this, jbyteArray buffer) {
    CALL_JNIENV();
    initialize_JniInputStream();
    return (jint) env->CallIntMethod(_this, methods_JniInputStream[3], buffer);
}

jint JniInputStream::read(jbyteArray buffer, jint offset, jint length) {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_JniInputStream[4], buffer, offset, length);
}

jint JniInputStream::read_(jobject _this, jbyteArray buffer, jint offset, jint length) {
    CALL_JNIENV();
    initialize_JniInputStream();
    return (jint) env->CallIntMethod(_this, methods_JniInputStream[4], buffer, offset, length);
}

jlong JniInputStream::skip(jlong byteCount) {
    CALL_JNIENV();
    return (jlong) env->CallLongMethod(this->getObject(), methods_JniInputStream[5], byteCount);
}

jlong JniInputStream::skip_(jobject _this, jlong byteCount) {
    CALL_JNIENV();
    initialize_JniInputStream();
    return (jlong) env->CallLongMethod(_this, methods_JniInputStream[5], byteCount);
}

jc_sp<JniInputStream> JniInputStream::wrap(jobject obj) {
    return jc_sp<JniInputStream>( mark_new JniInputStream(obj));
}

jc_sp<JniInputStream> JniInputStream::global(jobject obj) {
    return jc_sp<JniInputStream>( (JniInputStream*)(mark_new JniInputStream(obj))->addGlobalRef());
}

jclass JniInputStream::getClass() {
    initialize_JniInputStream();
    return class_JniInputStream;
}

}

