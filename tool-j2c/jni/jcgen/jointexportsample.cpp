/**
 * export from "Joint Coding Project"
 */
#include "jointexportsample.h"

const charactor* JointExportSample::CLASS_SIGNATURE = "com.eaglesakura.export.test.JointExportSample";
static jclass class_JointExportSample = NULL;
static jmethodID methods_JointExportSample[12] = { NULL };

static void initialize_JointExportSample() {
    // loaded !
    if (class_JointExportSample) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JointExportSample = env->FindClass(JointExportSample::CLASS_SIGNATURE);
    class_JointExportSample = (jclass)env->NewGlobalRef(class_JointExportSample);
    

    // load methods
    {
        methods_JointExportSample[0] = JCJniWrapper::loadMethod(class_JointExportSample, "setHoge", "(I)V", true);
        methods_JointExportSample[1] = JCJniWrapper::loadMethod(class_JointExportSample, "setHoge", "(ILjava.lang.String;)Ljava.lang.String;", false);
        methods_JointExportSample[2] = JCJniWrapper::loadMethod(class_JointExportSample, "nativeCall", "(ILjava.lang.String;)V", false);
        methods_JointExportSample[3] = JCJniWrapper::loadMethod(class_JointExportSample, "doIntArray", "([I)[I", false);
        methods_JointExportSample[4] = JCJniWrapper::loadMethod(class_JointExportSample, "doHogeArray", "([Lcom.eaglesakura.lib.jc.sample.SampleExportClass;)[Lcom.eaglesakura.lib.jc.sample.SampleExportClass;", false);
        methods_JointExportSample[5] = JCJniWrapper::loadMethod(class_JointExportSample, "obj", "(Ljava.lang.Object;)Ljava.lang.Object;", false);
        methods_JointExportSample[6] = JCJniWrapper::loadMethod(class_JointExportSample, "equals", "(Ljava.lang.Object;)Z", false);
        methods_JointExportSample[7] = JCJniWrapper::loadMethod(class_JointExportSample, "doObjectArray", "([Ljava.lang.Object;)[Ljava.lang.Object;", false);
        methods_JointExportSample[8] = JCJniWrapper::loadMethod(class_JointExportSample, "getHoge", "(I)Lcom.eaglesakura.lib.jc.sample.SampleExportClass;", false);
        methods_JointExportSample[9] = JCJniWrapper::loadMethod(class_JointExportSample, "doHoge", "(Lcom.eaglesakura.lib.jc.sample.SampleExportClass;)Lcom.eaglesakura.lib.jc.sample.SampleExportClass;", false);
        methods_JointExportSample[10] = JCJniWrapper::loadMethod(class_JointExportSample, "read", "([BII)V", false);
        methods_JointExportSample[11] = JCJniWrapper::loadMethod(class_JointExportSample, "setHoge", "()V", false);

    }
}

JointExportSample::JointExportSample(jobject obj): JCJniWrapper(obj){
    initialize_JointExportSample();
}

void JointExportSample::setHoge(jint value) {
    CALL_JNIENV();
    initialize_JointExportSample();
    env->CallStaticVoidMethod(class_JointExportSample, methods_JointExportSample[0], value);
}

jstring JointExportSample::setHoge(jint value, jstring str) {
    CALL_JNIENV();
    return (jstring) env->CallObjectMethod(this->getObject(), methods_JointExportSample[1], value, str);
}

void JointExportSample::nativeCall(jint arg0, jstring arg1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointExportSample[2], arg0, arg1);
}
#if 0
#include <jni.h>
extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_export_test_JointExportSample_nativeCall(JNIEnv *env, jobject _this, jint, arg0, jstring, arg1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_export_test_JointExportSample_nativeCall(JNIEnv *env, jobject _this, jint, arg0, jstring, arg1) {
    logf("call method!! :: %s", "Java_com_eaglesakura_export_test_JointExportSample_nativeCall");
    // TODO Let's Coding!!
    
    return;
}
#endif


jintArray JointExportSample::doIntArray(jintArray array) {
    CALL_JNIENV();
    return (jintArray) env->CallObjectMethod(this->getObject(), methods_JointExportSample[3], array);
}

jobjectArray JointExportSample::doHogeArray(jobjectArray clz) {
    CALL_JNIENV();
    return (jobjectArray) env->CallObjectMethod(this->getObject(), methods_JointExportSample[4], clz);
}

jobject JointExportSample::obj(jobject obj) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointExportSample[5], obj);
}

jobject JointExportSample::equals(jobject obj) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointExportSample[6], obj);
}

jobjectArray JointExportSample::doObjectArray(jobjectArray array) {
    CALL_JNIENV();
    return (jobjectArray) env->CallObjectMethod(this->getObject(), methods_JointExportSample[7], array);
}

jobject JointExportSample::getHoge(jint value) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointExportSample[8], value);
}

jobject JointExportSample::doHoge(jobject clz) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_JointExportSample[9], clz);
}

void JointExportSample::read(jbyteArray buffer, jint index, jint length) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointExportSample[10], buffer, index, length);
}

void JointExportSample::setHoge() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JointExportSample[11]);
}

JointExportSample* JointExportSample::wrap(jobject obj) {
    return new JointExportSample(obj);
}

JointExportSample* JointExportSample::global(jobject obj) {
    return (JointExportSample*) (new JointExportSample(obj))->addGlobalRef();
}
