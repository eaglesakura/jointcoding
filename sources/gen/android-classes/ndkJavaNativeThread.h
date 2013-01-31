/**
 * export from "Joint Coding Project"
 */
#ifndef __JAVANATIVETHREAD_H__
#define __JAVANATIVETHREAD_H__
#include "jointcoding-android.h"

namespace ndk {

class JavaNativeThread: public ::ndk::JniWrapper {
protected:
    JavaNativeThread(jobject obj);
public:
    virtual ~JavaNativeThread(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void threadJoin();
    static void threadJoin_(jobject _this);
    static jobject createInstance_unsafe(jint nativeThread);
    virtual void setThreadName(jstring name);
    static void setThreadName_(jobject _this, jstring name);
    virtual void setThreadName(const ::jc::charactor* name) { CALL_JNIENV();  jstring name_j = ::ndk::c2jstring(name); setThreadName( name_j);  env->DeleteLocalRef(name_j); }
    virtual void run();
    static void run_(jobject _this);
    static void onBackground(jint int_0);
    virtual void start();
    static void start_(jobject _this);


    static jc_sp<JavaNativeThread> wrap(jobject obj);
    static jc_sp<JavaNativeThread> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JAVANATIVETHREAD_H__
