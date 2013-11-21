/**
 * export from "Joint Coding Project"
 */
#include "ndkJavaNativeThread.h"

namespace ndk {

const ::jc::charactor* JavaNativeThread::CLASS_SIGNATURE = "com/eaglesakura/jc/android/app/util/JavaNativeThread";

static jclass class_JavaNativeThread = NULL;

#define methods_JavaNativeThread_LENGTH 6

#if methods_JavaNativeThread_LENGTH
static jmethodID methods_JavaNativeThread[6];
#endif

static void initialize_JavaNativeThread() {
    // loaded !
    if (class_JavaNativeThread) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JavaNativeThread = env->FindClass(JavaNativeThread::CLASS_SIGNATURE);
    class_JavaNativeThread = (jclass)::ndk::change_globalref(env, class_JavaNativeThread);
    

    // load methods
    {
        methods_JavaNativeThread[0] = ::ndk::JniWrapper::loadMethod(class_JavaNativeThread, "threadJoin", "()V", false);
        methods_JavaNativeThread[1] = ::ndk::JniWrapper::loadMethod(class_JavaNativeThread, "createInstance", "(I)Lcom/eaglesakura/jc/android/app/util/JavaNativeThread;", true);
        methods_JavaNativeThread[2] = ::ndk::JniWrapper::loadMethod(class_JavaNativeThread, "setThreadName", "(Ljava/lang/String;)V", false);
        methods_JavaNativeThread[3] = ::ndk::JniWrapper::loadMethod(class_JavaNativeThread, "run", "()V", false);
        methods_JavaNativeThread[4] = ::ndk::JniWrapper::loadMethod(class_JavaNativeThread, "onBackground", "(I)V", true);
        methods_JavaNativeThread[5] = ::ndk::JniWrapper::loadMethod(class_JavaNativeThread, "start", "()V", false);

    }
}

JavaNativeThread::JavaNativeThread(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JavaNativeThread();
}

void JavaNativeThread::threadJoin() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JavaNativeThread[0]);
}

void JavaNativeThread::threadJoin_(jobject _this) {
    CALL_JNIENV();
    initialize_JavaNativeThread();
    env->CallVoidMethod(_this, methods_JavaNativeThread[0]);
}

jobject JavaNativeThread::createInstance_unsafe(jint nativeThread) {
    CALL_JNIENV();
    initialize_JavaNativeThread();
    return (jobject) env->CallStaticObjectMethod(class_JavaNativeThread, methods_JavaNativeThread[1], nativeThread);
}

void JavaNativeThread::setThreadName(jstring name) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JavaNativeThread[2], name);
}

void JavaNativeThread::setThreadName_(jobject _this, jstring name) {
    CALL_JNIENV();
    initialize_JavaNativeThread();
    env->CallVoidMethod(_this, methods_JavaNativeThread[2], name);
}

void JavaNativeThread::run() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JavaNativeThread[3]);
}

void JavaNativeThread::run_(jobject _this) {
    CALL_JNIENV();
    initialize_JavaNativeThread();
    env->CallVoidMethod(_this, methods_JavaNativeThread[3]);
}

void JavaNativeThread::onBackground(jint int_0) {
    CALL_JNIENV();
    initialize_JavaNativeThread();
    env->CallStaticVoidMethod(class_JavaNativeThread, methods_JavaNativeThread[4], int_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkJavaNativeThread.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_util_JavaNativeThread_onBackground(JNIEnv *env, jobject _this, jint int_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_util_JavaNativeThread_onBackground(JNIEnv *env, jobject _this, jint int_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_app_util_JavaNativeThread_onBackground");
    
    return;
}
#endif


void JavaNativeThread::start() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_JavaNativeThread[5]);
}

void JavaNativeThread::start_(jobject _this) {
    CALL_JNIENV();
    initialize_JavaNativeThread();
    env->CallVoidMethod(_this, methods_JavaNativeThread[5]);
}

jc_sp<JavaNativeThread> JavaNativeThread::wrap(jobject obj) {
    return jc_sp<JavaNativeThread>( mark_new JavaNativeThread(obj));
}

jc_sp<JavaNativeThread> JavaNativeThread::global(jobject obj) {
    return jc_sp<JavaNativeThread>( (JavaNativeThread*)(mark_new JavaNativeThread(obj))->addGlobalRef());
}

jclass JavaNativeThread::getClass() {
    initialize_JavaNativeThread();
    return class_JavaNativeThread;
}

}

