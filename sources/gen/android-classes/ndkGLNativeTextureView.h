/**
 * export from "Joint Coding Project"
 */
#ifndef __GLNATIVETEXTUREVIEW_H__
#define __GLNATIVETEXTUREVIEW_H__
#include "jointcoding-android.h"

namespace ndk {

class GLNativeTextureView: public ::ndk::JniWrapper {
protected:
    GLNativeTextureView(jobject obj);
public:
    virtual ~GLNativeTextureView(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void onNativeGLSurfaceSizeChanged(jobject surfacetexture_0, jint int_1, jint int_2);
    static void onNativeGLSurfaceSizeChanged_(jobject _this, jobject surfacetexture_0, jint int_1, jint int_2);
    virtual jint onCreateNativeContext(jint int_0);
    static jint onCreateNativeContext_(jobject _this, jint int_0);
    virtual void unlockEGL();
    static void unlockEGL_(jobject _this);
    virtual void lockEGL();
    static void lockEGL_(jobject _this);
    virtual jint getNativeContextPointer();
    static jint getNativeContextPointer_(jobject _this);
    virtual void onNativeGLInitialize(jobject surfacetexture_0, jint int_1, jint int_2);
    static void onNativeGLInitialize_(jobject _this, jobject surfacetexture_0, jint int_1, jint int_2);
    virtual void onDestroyNative();
    static void onDestroyNative_(jobject _this);
    virtual void onNativeGLDestroyed(jobject surfacetexture_0);
    static void onNativeGLDestroyed_(jobject _this, jobject surfacetexture_0);

    static const ::jc::s32 EGL_DEPTHBUFFER_ENABLE;
    static const ::jc::s32 EGL_COLORBUFFER_BITS_DEFAULT;

    static jc_sp<GLNativeTextureView> wrap(jobject obj);
    static jc_sp<GLNativeTextureView> global(jobject obj);

    static jclass getClass();
};

}

#endif // __GLNATIVETEXTUREVIEW_H__
