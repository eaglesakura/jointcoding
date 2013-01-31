/**
 * export from "Joint Coding Project"
 */
#ifndef __NATIVEONCLICKLISTENER_H__
#define __NATIVEONCLICKLISTENER_H__
#include "jointcoding-android.h"

namespace ndk {

class NativeOnClickListener: public ::ndk::JniWrapper {
protected:
    NativeOnClickListener(jobject obj);
public:
    virtual ~NativeOnClickListener(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void onNativeClick(jobject view_0, jint int_1);
    static void onNativeClick_(jobject _this, jobject view_0, jint int_1);
    static jobject setOnClickListener_unsafe(jobject view, jint nativeListenerPointer);


    static jc_sp<NativeOnClickListener> wrap(jobject obj);
    static jc_sp<NativeOnClickListener> global(jobject obj);

    static jclass getClass();
};

}

#endif // __NATIVEONCLICKLISTENER_H__
