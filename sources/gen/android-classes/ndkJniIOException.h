/**
 * export from "Joint Coding Project"
 */
#ifndef __JNIIOEXCEPTION_H__
#define __JNIIOEXCEPTION_H__
#include "jointcoding-android.h"

namespace ndk {

class JniIOException: public ::ndk::JniWrapper {
protected:
    JniIOException(jobject obj);
public:
    virtual ~JniIOException(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void printStackTrace();
    static void printStackTrace_(jobject _this);
    virtual jstring getMessage();
    static jstring getMessage_(jobject _this);


    static jc_sp<JniIOException> wrap(jobject obj);
    static jc_sp<JniIOException> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JNIIOEXCEPTION_H__
