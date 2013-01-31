/**
 * export from "Joint Coding Project"
 */
#ifndef __ANDROIDUTIL_H__
#define __ANDROIDUTIL_H__
#include "jointcoding-android.h"

namespace ndk {

class AndroidUtil: public ::ndk::JniWrapper {
protected:
    AndroidUtil(jobject obj);
public:
    virtual ~AndroidUtil(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static void _log(jstring string_0);
    static jobject getSurface_unsafe(jobject holder);


    static jc_sp<AndroidUtil> wrap(jobject obj);
    static jc_sp<AndroidUtil> global(jobject obj);

    static jclass getClass();
};

}

#endif // __ANDROIDUTIL_H__
