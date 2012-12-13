/*
 * ndkNativeFragment.h
 *
 *  Created on: 2012/09/05
 */

#ifndef NDKNATIVEFRAGMENT_H_
#define NDKNATIVEFRAGMENT_H_

#include    "ndkNativeFragment.h"

namespace ndk {

using namespace jc;

class NativeFragmentBase: public Object {
protected:
    NativeFragmentBase() {
    }

public:
    virtual ~NativeFragmentBase() {
    }

    virtual void onCreateCompleted(jobject fragment, jobject bundle) {
        jclog("onCreateCompleted");
    }

    virtual void onActivityCreatedComplete(jobject fragment, jobject bundle) {
        jclog("onActivityCreatedComplete");
    }

    virtual void onAttachCompleted(jobject fragment, jobject activity) {
        jclog("onAttachCompleted");
    }

    virtual void onResumeCompleted(jobject fragment) {
        jclog("onResumeCompleted");
    }

    virtual void onPauseBegin(jobject fragment) {
        jclog("onPauseBegin");
    }

    virtual void onDetachBegin(jobject fragment) {
        jclog("onDetachBegin");
    }

    virtual void onDestroyBegin(jobject fragment) {
        jclog("onDestroyBegin");
    }

    static jc_sp<NativeFragmentBase>& getFragmentPointer(jobject nativeFragment) {
        return *((jc_sp<NativeFragmentBase>*) NativeFragment::getNativeStatePointer_(nativeFragment));
    }
};

/**
 * ndk::NativeFragmentBase*を取得する
 */
#define     native_fragment_ptr(clazz, _this)  ((clazz*)::ndk::NativeFragmentBase::getFragmentPointer(_this).get())

#define     native_fragment(_this)      (::ndk::NativeFragmentBase::getFragmentPointer(_this))


}

#endif /* NDKNATIVEFRAGMENT_H_ */
