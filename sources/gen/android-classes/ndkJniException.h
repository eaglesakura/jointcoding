/**
 * export from "Joint Coding Project"
 */
#ifndef __JNIEXCEPTION_H__
#define __JNIEXCEPTION_H__
#include "jointcoding-android.h"

namespace ndk {

class JniException: public ::ndk::JniWrapper {
protected:
    JniException(jobject obj);
public:
    virtual ~JniException(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void printStackTrace();
    static void printStackTrace_(jobject _this);
    virtual jstring getMessage();
    static jstring getMessage_(jobject _this);


    static jc_sp<JniException> wrap(jobject obj);
    static jc_sp<JniException> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JNIEXCEPTION_H__
