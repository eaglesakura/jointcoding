/**
 * export from "Joint Coding Project"
 */
#ifndef __WINDOWDEVICEMANAGER_H__
#define __WINDOWDEVICEMANAGER_H__
#include "jointcoding-android.h"

namespace ndk {

class WindowDeviceManager: public ::ndk::JniWrapper {
protected:
    WindowDeviceManager(jobject obj);
public:
    virtual ~WindowDeviceManager(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jobject getEGLContextWrapper_unsafe();
    static jobject getEGLContextWrapper_unsafe_(jobject _this);
    virtual void setNativePointer(jint key, jobject ptr);
    static void setNativePointer_(jobject _this, jint key, jobject ptr);
    virtual jobject getEGLWrapper_unsafe();
    static jobject getEGLWrapper_unsafe_(jobject _this);
    virtual jobject createSlaveDevice_unsafe();
    static jobject createSlaveDevice_unsafe_(jobject _this);
    virtual jobject getNativePointer_unsafe(jint key);
    static jobject getNativePointer_unsafe_(jobject _this, jint key);
    virtual void onNativeEndOperation();
    static void onNativeEndOperation_(jobject _this);
    virtual void onNativeBeginOperation();
    static void onNativeBeginOperation_(jobject _this);
    virtual void createNative();
    static void createNative_(jobject _this);
    virtual jobject getEGLSurfaceWrapper_unsafe();
    static jobject getEGLSurfaceWrapper_unsafe_(jobject _this);

    static const ::jc::s32 KEY_MAINCONTEXT;

    static jc_sp<WindowDeviceManager> wrap(jobject obj);
    static jc_sp<WindowDeviceManager> global(jobject obj);

    static jclass getClass();
};

}

#endif // __WINDOWDEVICEMANAGER_H__
