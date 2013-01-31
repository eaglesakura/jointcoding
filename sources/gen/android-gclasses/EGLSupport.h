/**
 * export from "Joint Coding Project"
 */
#ifndef __EGLSUPPORT_H__
#define __EGLSUPPORT_H__
#include "jointcoding-android.h"

class EGLSupport: public ::ndk::JniWrapper {
protected:
    EGLSupport(jobject obj);
public:
    virtual ~EGLSupport(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static jint getIntFieldNative(jobject class_0, jobject object_1, jstring string_2);
    static jobject newSupportClassNative_unsafe(jstring string_0, jint int_1);
    static void unlockEGLMakeCurrent(jint display, jint draw_surface, jint read_surface, jint context);
    static void unlockEGLMakeCurrentNative(jint int_0, jint int_1, jint int_2, jint int_3);
    static jint eglCreateWindowSurfaceSupport(jint display, jint config, jobject surfaceTexture);


    static jc_sp<EGLSupport> wrap(jobject obj);
    static jc_sp<EGLSupport> global(jobject obj);

    static jclass getClass();
};
#endif // __EGLSUPPORT_H__
