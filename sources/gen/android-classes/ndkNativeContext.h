/**
 * export from "Joint Coding Project"
 */
#ifndef __NATIVECONTEXT_H__
#define __NATIVECONTEXT_H__
#include "jointcoding-android.h"

namespace ndk {

class NativeContext: public ::ndk::JniWrapper {
protected:
    NativeContext(jobject obj);
public:
    virtual ~NativeContext(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static jstring getBootingId();
    static jboolean isNativeDebuggable();
    virtual void nativeInitialize();
    static void nativeInitialize_(jobject _this);
    static void gc();
    virtual jobject getAppContext_unsafe();
    static jobject getAppContext_unsafe_(jobject _this);
    static jboolean isUIThread();
    static void nativeGC();


    static jc_sp<NativeContext> wrap(jobject obj);
    static jc_sp<NativeContext> global(jobject obj);

    static jclass getClass();
};

}

#endif // __NATIVECONTEXT_H__
