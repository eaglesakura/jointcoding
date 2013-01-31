/**
 * export from "Joint Coding Project"
 */
#ifndef __ASSERTERROR_H__
#define __ASSERTERROR_H__
#include "jointcoding-android.h"

namespace jc {

class AssertError: public ::ndk::JniWrapper {
protected:
    AssertError(jobject obj);
public:
    virtual ~AssertError(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;


    static jc_sp<AssertError> wrap(jobject obj);
    static jc_sp<AssertError> global(jobject obj);

    static jclass getClass();
};

}

#endif // __ASSERTERROR_H__
