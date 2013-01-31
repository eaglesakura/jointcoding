/**
 * export from "Joint Coding Project"
 */
#ifndef __NATIVEFRAGMENT_H__
#define __NATIVEFRAGMENT_H__
#include "jointcoding-android.h"

namespace ndk {

class NativeFragment: public ::ndk::JniWrapper {
protected:
    NativeFragment(jobject obj);
public:
    virtual ~NativeFragment(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void onActivityCreatedCompleteNative(jobject bundle_0);
    static void onActivityCreatedCompleteNative_(jobject _this, jobject bundle_0);
    virtual void onResumeCompletedNative();
    static void onResumeCompletedNative_(jobject _this);
    virtual void onDestroyBeginNative();
    static void onDestroyBeginNative_(jobject _this);
    virtual void onCreateCompletedNative(jobject bundle_0);
    static void onCreateCompletedNative_(jobject _this, jobject bundle_0);
    virtual void onAttachCompletedNative(jobject activity_0);
    static void onAttachCompletedNative_(jobject _this, jobject activity_0);
    virtual jint getNativeStatePointer();
    static jint getNativeStatePointer_(jobject _this);
    virtual void onPauseBeginNative();
    static void onPauseBeginNative_(jobject _this);
    virtual void onDetachBeginNative();
    static void onDetachBeginNative_(jobject _this);


    static jc_sp<NativeFragment> wrap(jobject obj);
    static jc_sp<NativeFragment> global(jobject obj);

    static jclass getClass();
};

}

#endif // __NATIVEFRAGMENT_H__
