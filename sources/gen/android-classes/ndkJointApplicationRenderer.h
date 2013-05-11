/**
 * export from "Joint Coding Project"
 */
#ifndef __JOINTAPPLICATIONRENDERER_H__
#define __JOINTAPPLICATIONRENDERER_H__
#include "jointcoding-android.h"

namespace ndk {

class JointApplicationRenderer: public ::ndk::JniWrapper {
protected:
    JointApplicationRenderer(jobject obj);
public:
    virtual ~JointApplicationRenderer(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void onNativeMainLoop();
    static void onNativeMainLoop_(jobject _this);
    virtual void setNativePointer(jint key, jobject ptr);
    static void setNativePointer_(jobject _this, jint key, jobject ptr);
    virtual void onNativeDestroy();
    static void onNativeDestroy_(jobject _this);
    virtual void onNativeInitialize();
    static void onNativeInitialize_(jobject _this);
    virtual void onNativeResume();
    static void onNativeResume_(jobject _this);
    virtual void onNativePause();
    static void onNativePause_(jobject _this);
    virtual jobject getNativePointer_unsafe(jint key);
    static jobject getNativePointer_unsafe_(jobject _this, jint key);
    virtual jobject getDeviceManager_unsafe();
    static jobject getDeviceManager_unsafe_(jobject _this);
    virtual void onNativeSurfaceResized(jint int_0, jint int_1);
    static void onNativeSurfaceResized_(jobject _this, jint int_0, jint int_1);

    static const ::jc::s32 KEY_MAINCONTEXT;

    static jc_sp<JointApplicationRenderer> wrap(jobject obj);
    static jc_sp<JointApplicationRenderer> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JOINTAPPLICATIONRENDERER_H__
