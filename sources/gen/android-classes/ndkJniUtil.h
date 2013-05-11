/**
 * export from "Joint Coding Project"
 */
#ifndef __JNIUTIL_H__
#define __JNIUTIL_H__
#include "jointcoding-android.h"

namespace ndk {

class JniUtil: public ::ndk::JniWrapper {
protected:
    JniUtil(jobject obj);
public:
    virtual ~JniUtil(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static jint getAPILevel();
    static void printStackTrace(jobject e);


    static jc_sp<JniUtil> wrap(jobject obj);
    static jc_sp<JniUtil> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JNIUTIL_H__
