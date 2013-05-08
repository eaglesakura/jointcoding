/**
 * export from "Joint Coding Project"
 */
#ifndef __EGLCONTEXTWRAPPER_H__
#define __EGLCONTEXTWRAPPER_H__
#include "jointcoding-android.h"

namespace ndk {

class EGLContextWrapper: public ::ndk::JniWrapper {
protected:
    EGLContextWrapper(jobject obj);
public:
    virtual ~EGLContextWrapper(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void dispose();
    static void dispose_(jobject _this);


    static jc_sp<EGLContextWrapper> wrap(jobject obj);
    static jc_sp<EGLContextWrapper> global(jobject obj);

    static jclass getClass();
};

}

#endif // __EGLCONTEXTWRAPPER_H__
