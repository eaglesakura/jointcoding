/**
 * export from "Joint Coding Project"
 */
#ifndef __EGLWRAPPER_H__
#define __EGLWRAPPER_H__
#include "jointcoding-android.h"

namespace ndk {

class EGLWrapper: public ::ndk::JniWrapper {
protected:
    EGLWrapper(jobject obj);
public:
    virtual ~EGLWrapper(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void dispose();
    static void dispose_(jobject _this);
    virtual jboolean current(jobject context, jobject surface);
    static jboolean current_(jobject _this, jobject context, jobject surface);
    virtual jboolean postFrontBuffer(jobject surface);
    static jboolean postFrontBuffer_(jobject _this, jobject surface);


    static jc_sp<EGLWrapper> wrap(jobject obj);
    static jc_sp<EGLWrapper> global(jobject obj);

    static jclass getClass();
};

}

#endif // __EGLWRAPPER_H__
