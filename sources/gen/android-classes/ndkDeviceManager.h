/**
 * export from "Joint Coding Project"
 */
#ifndef __DEVICEMANAGER_H__
#define __DEVICEMANAGER_H__
#include "jointcoding-android.h"

namespace ndk {

class DeviceManager: public ::ndk::JniWrapper {
protected:
    DeviceManager(jobject obj);
public:
    virtual ~DeviceManager(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jobject getEGLContextWrapper_unsafe();
    static jobject getEGLContextWrapper_unsafe_(jobject _this);
    virtual jobject getNativePointer_unsafe(jint key);
    static jobject getNativePointer_unsafe_(jobject _this, jint key);
    virtual jobject getEGLWrapper_unsafe();
    static jobject getEGLWrapper_unsafe_(jobject _this);
    virtual void setNativePointer(jint key, jobject ptr);
    static void setNativePointer_(jobject _this, jint key, jobject ptr);
    virtual void createNative();
    static void createNative_(jobject _this);
    virtual void preDestroyNative();
    static void preDestroyNative_(jobject _this);
    virtual jobject getEGLSurfaceWrapper_unsafe();
    static jobject getEGLSurfaceWrapper_unsafe_(jobject _this);

    static const ::jc::s32 KEY_MAINCONTEXT;

    static jc_sp<DeviceManager> wrap(jobject obj);
    static jc_sp<DeviceManager> global(jobject obj);

    static jclass getClass();
};

}

#endif // __DEVICEMANAGER_H__
