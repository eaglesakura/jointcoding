/**
 * export from "Joint Coding Project"
 */
#ifndef __EGLSURFACEWRAPPER_H__
#define __EGLSURFACEWRAPPER_H__
#include "jointcoding-android.h"

namespace ndk {

class EGLSurfaceWrapper: public ::ndk::JniWrapper {
protected:
    EGLSurfaceWrapper(jobject obj);
public:
    virtual ~EGLSurfaceWrapper(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void dispose();
    static void dispose_(jobject _this);
    virtual jint getHeight();
    static jint getHeight_(jobject _this);
    virtual jint getWidth();
    static jint getWidth_(jobject _this);


    static jc_sp<EGLSurfaceWrapper> wrap(jobject obj);
    static jc_sp<EGLSurfaceWrapper> global(jobject obj);

    static jclass getClass();
};

}

#endif // __EGLSURFACEWRAPPER_H__
