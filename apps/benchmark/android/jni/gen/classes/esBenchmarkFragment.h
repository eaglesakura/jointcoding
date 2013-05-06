/**
 * export from "Joint Coding Project"
 */
#ifndef __BENCHMARKFRAGMENT_H__
#define __BENCHMARKFRAGMENT_H__
#include "jointcoding-android.h"

namespace es {

class BenchmarkFragment: public ::ndk::JniWrapper {
protected:
    BenchmarkFragment(jobject obj);
public:
    virtual ~BenchmarkFragment(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void onNativeDestroy();
    static void onNativeDestroy_(jobject _this);
    virtual void onNativePause();
    static void onNativePause_(jobject _this);
    virtual void setNativePointer(jint key, jobject ptr);
    static void setNativePointer_(jobject _this, jint key, jobject ptr);
    virtual void onNativeSurfaceResized(jint int_0, jint int_1);
    static void onNativeSurfaceResized_(jobject _this, jint int_0, jint int_1);
    virtual void onNativeMainLoop();
    static void onNativeMainLoop_(jobject _this);
    virtual void createNativeContext(jobject glnativetextureview_0);
    static void createNativeContext_(jobject _this, jobject glnativetextureview_0);
    virtual void onNativeInitialize();
    static void onNativeInitialize_(jobject _this);
    virtual void onNativeResume();
    static void onNativeResume_(jobject _this);
    virtual jobject getNativePointer_unsafe(jint key);
    static jobject getNativePointer_unsafe_(jobject _this, jint key);
    virtual jobject getSurface_unsafe();
    static jobject getSurface_unsafe_(jobject _this);

    static const ::jc::s32 KEY_MAINCONTEXT;

    static jc_sp<BenchmarkFragment> wrap(jobject obj);
    static jc_sp<BenchmarkFragment> global(jobject obj);

    static jclass getClass();
};

}

#endif // __BENCHMARKFRAGMENT_H__
