/**
 * export from "Joint Coding Project"
 */
#ifndef __JOINTABLE_H__
#define __JOINTABLE_H__
#include "jointcoding-android.h"

namespace ndk {

class Jointable: public ::ndk::JniWrapper {
protected:
    Jointable(jobject obj);
public:
    virtual ~Jointable(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jobject getNativePointer_unsafe(jint int_0);
    static jobject getNativePointer_unsafe_(jobject _this, jint int_0);
    virtual void setNativePointer(jint int_0, jobject pointer_1);
    static void setNativePointer_(jobject _this, jint int_0, jobject pointer_1);


    static jc_sp<Jointable> wrap(jobject obj);
    static jc_sp<Jointable> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JOINTABLE_H__
