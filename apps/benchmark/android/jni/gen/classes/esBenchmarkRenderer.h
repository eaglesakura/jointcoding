/**
 * export from "Joint Coding Project"
 */
#ifndef __BENCHMARKRENDERER_H__
#define __BENCHMARKRENDERER_H__
#include "jointcoding-android.h"

namespace es {

class BenchmarkRenderer: public ::ndk::JniWrapper {
protected:
    BenchmarkRenderer(jobject obj);
public:
    virtual ~BenchmarkRenderer(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void onNativeMainLoop();
    static void onNativeMainLoop_(jobject _this);
    virtual void setNativePointer(jint key, jobject ptr);
    static void setNativePointer_(jobject _this, jint key, jobject ptr);
    virtual jobject createSlaveDevice_unsafe();
    static jobject createSlaveDevice_unsafe_(jobject _this);
    virtual void onNativeNewtask(jint int_0, jint int_1);
    static void onNativeNewtask_(jobject _this, jint int_0, jint int_1);
    virtual jboolean queryParams(jint int_0, jint int_1, jintArray int_array_2);
    static jboolean queryParams_(jobject _this, jint int_0, jint int_1, jintArray int_array_2);
    virtual void onNativeInitialize();
    static void onNativeInitialize_(jobject _this);
    virtual void createNativeContext();
    static void createNativeContext_(jobject _this);
    virtual jobject getNativePointer_unsafe(jint key);
    static jobject getNativePointer_unsafe_(jobject _this, jint key);
    virtual jboolean postParams(jint int_0, jint int_1, jintArray int_array_2);
    static jboolean postParams_(jobject _this, jint int_0, jint int_1, jintArray int_array_2);
    virtual jobject getWindowDevice_unsafe();
    static jobject getWindowDevice_unsafe_(jobject _this);
    virtual void startNewtask(jint taskId, jint userData);
    static void startNewtask_(jobject _this, jint taskId, jint userData);

    static const ::jc::s32 KEY_MAINCONTEXT;

    static jc_sp<BenchmarkRenderer> wrap(jobject obj);
    static jc_sp<BenchmarkRenderer> global(jobject obj);

    static jclass getClass();
};

}

#endif // __BENCHMARKRENDERER_H__
