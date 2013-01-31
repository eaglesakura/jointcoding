/**
 * export from "Joint Coding Project"
 */
#ifndef __NATIVEONTOUCHLISTENER_H__
#define __NATIVEONTOUCHLISTENER_H__
#include "jointcoding-android.h"

namespace ndk {

class NativeOnTouchListener: public ::ndk::JniWrapper {
protected:
    NativeOnTouchListener(jobject obj);
public:
    virtual ~NativeOnTouchListener(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void onNativeTouchEvent(jobject nativetouchevent_0, jint int_1, jint int_2, jint int_3);
    static void onNativeTouchEvent_(jobject _this, jobject nativetouchevent_0, jint int_1, jint int_2, jint int_3);
    static jobject setOnTouchListener_unsafe(jobject view, jint nativeListenerPointer);


    static jc_sp<NativeOnTouchListener> wrap(jobject obj);
    static jc_sp<NativeOnTouchListener> global(jobject obj);

    static jclass getClass();
};

}

#endif // __NATIVEONTOUCHLISTENER_H__
