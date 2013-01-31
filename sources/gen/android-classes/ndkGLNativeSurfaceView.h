/**
 * export from "Joint Coding Project"
 */
#ifndef __GLNATIVESURFACEVIEW_H__
#define __GLNATIVESURFACEVIEW_H__
#include "jointcoding-android.h"

namespace ndk {

class GLNativeSurfaceView: public ::ndk::JniWrapper {
protected:
    GLNativeSurfaceView(jobject obj);
public:
    virtual ~GLNativeSurfaceView(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void onSurfaceCreated(jobject surfaceholder_0, jobject surface_1);
    static void onSurfaceCreated_(jobject _this, jobject surfaceholder_0, jobject surface_1);
    virtual jint getNativeContextPointer();
    static jint getNativeContextPointer_(jobject _this);
    virtual void onSurfaceChanged(jobject surfaceholder_0, jobject surface_1, jint int_2, jint int_3, jint int_4);
    static void onSurfaceChanged_(jobject _this, jobject surfaceholder_0, jobject surface_1, jint int_2, jint int_3, jint int_4);
    static jint getNativeContextPointer(jobject view);
    virtual void onDestroyNative();
    static void onDestroyNative_(jobject _this);
    virtual jint onInitializeNative(jint int_0);
    static jint onInitializeNative_(jobject _this, jint int_0);
    virtual void onSurfaceDestroyed(jobject surfaceholder_0, jobject surface_1);
    static void onSurfaceDestroyed_(jobject _this, jobject surfaceholder_0, jobject surface_1);

    static const ::jc::s32 EGL_DEPTHBUFFER_ENABLE;
    static const ::jc::s32 EGL_COLORBUFFER_BITS_DEFAULT;

    static jc_sp<GLNativeSurfaceView> wrap(jobject obj);
    static jc_sp<GLNativeSurfaceView> global(jobject obj);

    static jclass getClass();
};

}

#endif // __GLNATIVESURFACEVIEW_H__
