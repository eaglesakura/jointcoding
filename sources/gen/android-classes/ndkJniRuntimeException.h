/**
 * export from "Joint Coding Project"
 */
#ifndef __JNIRUNTIMEEXCEPTION_H__
#define __JNIRUNTIMEEXCEPTION_H__
#include "jointcoding-android.h"

namespace ndk {

class JniRuntimeException: public ::ndk::JniWrapper {
protected:
    JniRuntimeException(jobject obj);
public:
    virtual ~JniRuntimeException(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void printStackTrace();
    static void printStackTrace_(jobject _this);
    virtual jstring getMessage();
    static jstring getMessage_(jobject _this);


    static jc_sp<JniRuntimeException> wrap(jobject obj);
    static jc_sp<JniRuntimeException> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JNIRUNTIMEEXCEPTION_H__
