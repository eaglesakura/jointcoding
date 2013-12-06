/**
 * export from "Joint Coding Project"
 */
#include "ndkDataBank.h"

namespace ndk {

const ::jc::charactor* DataBank::CLASS_SIGNATURE = "com/eaglesakura/jc/util/DataBank";

static jclass class_DataBank = NULL;

#define methods_DataBank_LENGTH 7

#if methods_DataBank_LENGTH
static jmethodID methods_DataBank[7];
#endif

static void initialize_DataBank() {
    // loaded !
    if (class_DataBank) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_DataBank = env->FindClass(DataBank::CLASS_SIGNATURE);
    class_DataBank = (jclass)::ndk::change_globalref(env, class_DataBank);
    

    // load methods
    {
        methods_DataBank[0] = ::ndk::JniWrapper::loadMethod(class_DataBank, "popString", "(J)Ljava/lang/String;", false);
        methods_DataBank[1] = ::ndk::JniWrapper::loadMethod(class_DataBank, "popDouble", "(J)D", false);
        methods_DataBank[2] = ::ndk::JniWrapper::loadMethod(class_DataBank, "popFloat", "(J)F", false);
        methods_DataBank[3] = ::ndk::JniWrapper::loadMethod(class_DataBank, "popLong", "(J)J", false);
        methods_DataBank[4] = ::ndk::JniWrapper::loadMethod(class_DataBank, "popInt", "(J)I", false);
        methods_DataBank[5] = ::ndk::JniWrapper::loadMethod(class_DataBank, "pop", "(J)Ljava/lang/Object;", false);
        methods_DataBank[6] = ::ndk::JniWrapper::loadMethod(class_DataBank, "add", "(Ljava/lang/Object;)J", false);

    }
}

DataBank::DataBank(jobject obj): ::ndk::JniWrapper(obj){
    initialize_DataBank();
}

jstring DataBank::popString(jlong key) {
    CALL_JNIENV();
    return (jstring) env->CallObjectMethod(this->getObject(), methods_DataBank[0], key);
}

jstring DataBank::popString_(jobject _this, jlong key) {
    CALL_JNIENV();
    initialize_DataBank();
    return (jstring) env->CallObjectMethod(_this, methods_DataBank[0], key);
}

jdouble DataBank::popDouble(jlong key) {
    CALL_JNIENV();
    return (jdouble) env->CallDoubleMethod(this->getObject(), methods_DataBank[1], key);
}

jdouble DataBank::popDouble_(jobject _this, jlong key) {
    CALL_JNIENV();
    initialize_DataBank();
    return (jdouble) env->CallDoubleMethod(_this, methods_DataBank[1], key);
}

jfloat DataBank::popFloat(jlong key) {
    CALL_JNIENV();
    return (jfloat) env->CallFloatMethod(this->getObject(), methods_DataBank[2], key);
}

jfloat DataBank::popFloat_(jobject _this, jlong key) {
    CALL_JNIENV();
    initialize_DataBank();
    return (jfloat) env->CallFloatMethod(_this, methods_DataBank[2], key);
}

jlong DataBank::popLong(jlong key) {
    CALL_JNIENV();
    return (jlong) env->CallLongMethod(this->getObject(), methods_DataBank[3], key);
}

jlong DataBank::popLong_(jobject _this, jlong key) {
    CALL_JNIENV();
    initialize_DataBank();
    return (jlong) env->CallLongMethod(_this, methods_DataBank[3], key);
}

jint DataBank::popInt(jlong key) {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_DataBank[4], key);
}

jint DataBank::popInt_(jobject _this, jlong key) {
    CALL_JNIENV();
    initialize_DataBank();
    return (jint) env->CallIntMethod(_this, methods_DataBank[4], key);
}

jobject DataBank::pop_unsafe(jlong key) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_DataBank[5], key);
}

jobject DataBank::pop_unsafe_(jobject _this, jlong key) {
    CALL_JNIENV();
    initialize_DataBank();
    return (jobject) env->CallObjectMethod(_this, methods_DataBank[5], key);
}

jlong DataBank::add(jobject obj) {
    CALL_JNIENV();
    return (jlong) env->CallLongMethod(this->getObject(), methods_DataBank[6], obj);
}

jlong DataBank::add_(jobject _this, jobject obj) {
    CALL_JNIENV();
    initialize_DataBank();
    return (jlong) env->CallLongMethod(_this, methods_DataBank[6], obj);
}

jc_sp<DataBank> DataBank::wrap(jobject obj) {
    return jc_sp<DataBank>( new DataBank(obj));
}

jc_sp<DataBank> DataBank::global(jobject obj) {
    return jc_sp<DataBank>( (DataBank*)(new DataBank(obj))->addGlobalRef());
}

jclass DataBank::getClass() {
    initialize_DataBank();
    return class_DataBank;
}

}

