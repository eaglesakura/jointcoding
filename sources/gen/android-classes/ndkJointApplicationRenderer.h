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
    virtual void dispatchTouchEvent(jobject toucheventprotocol_0);
    static void dispatchTouchEvent_(jobject _this, jobject toucheventprotocol_0);
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
    virtual jboolean postParams(jint int_0, jint int_1, jintArray int_array_2);
    static jboolean postParams_(jobject _this, jint int_0, jint int_1, jintArray int_array_2);
    virtual jobject getNativePointer_unsafe(jint key);
    static jobject getNativePointer_unsafe_(jobject _this, jint key);
    virtual jobject getWindowDevice_unsafe();
    static jobject getWindowDevice_unsafe_(jobject _this);
    virtual void startNewtask(jint taskId, jint userData);
    static void startNewtask_(jobject _this, jint taskId, jint userData);

    static const ::jc::s32 KEY_MAINCONTEXT;

    static jc_sp<JointApplicationRenderer> wrap(jobject obj);
    static jc_sp<JointApplicationRenderer> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JOINTAPPLICATIONRENDERER_H__
