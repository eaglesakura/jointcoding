/*
 * JavaJointThread.cpp
 *
 *  Created on: 2012/07/22

 */

#include    "ndkJavaJointThread.h"

namespace ndk {

JavaJointThread::JavaJointThread(MRunnable runnable) :
        Thread(runnable) {
    nativeThread = JavaNativeThread::global(JavaNativeThread::createInstance((jint) this));
    this->setName(getName());
}

JavaJointThread::~JavaJointThread() {
    jclogf("thread delete = %x", this);
}

/**
 * スレッド名を指定する
 */
void JavaJointThread::setName(String name) {
    Thread::setName(name);

    CALL_JNIENV();
    jstring jName = env->NewStringUTF(name.c_str());
    {
        nativeThread->setThreadName(jName);
    }
    env->DeleteLocalRef(jName);
}

void JavaJointThread::start() {
    nativeThread->start();
}

void JavaJointThread::join() {
    nativeThread->threadJoin();
}

}

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_util_JavaNativeThread_onBackground(JNIEnv *env, jobject _this, jint pointer) {
    // call env reset
    initJniEnv(env);

    ndk::JavaJointThread *thread = (ndk::JavaJointThread *) pointer;
    if (thread) {
        thread->getRunnable()->run();
        ndk::JavaJointThread::removeAutorelease(thread);
    }

    return;
}

}
