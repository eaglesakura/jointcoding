/**
 * export from "Joint Coding Project"
 */
#ifndef __NATIVEAPPUTIL_H__
#define __NATIVEAPPUTIL_H__
#include "jointcoding-android.h"

namespace ndk {

class NativeAppUtil: public ::ndk::JniWrapper {
protected:
    NativeAppUtil(jobject obj);
public:
    virtual ~NativeAppUtil(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static jint getAPILevel();
    static void printStackTrace(jobject e);


    static jc_sp<NativeAppUtil> wrap(jobject obj);
    static jc_sp<NativeAppUtil> global(jobject obj);

    static jclass getClass();
};

}

#endif // __NATIVEAPPUTIL_H__
