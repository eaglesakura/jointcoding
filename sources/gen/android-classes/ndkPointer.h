/**
 * export from "Joint Coding Project"
 */
#ifndef __POINTER_H__
#define __POINTER_H__
#include "jointcoding-android.h"

namespace ndk {

class Pointer: public ::ndk::JniWrapper {
protected:
    Pointer(jobject obj);
public:
    virtual ~Pointer(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static jobject createSharedObjectInstance_unsafe(jint pointer);
    static void deleteSharedObjectPtr(jint int_0);
    virtual void dispose();
    static void dispose_(jobject _this);
    static jint getSharedObjectPtr(jint int_0);
    static jobject createObjectPointerInstance_unsafe(jint pointer);
    static void deleteObjectPtr(jint int_0);
    static jobject createVoidPointerInstance_unsafe(jint pointer);
    static void deleteVoidPtr(jint int_0);
    virtual jint getObjectPointer();
    static jint getObjectPointer_(jobject _this);


    static jc_sp<Pointer> wrap(jobject obj);
    static jc_sp<Pointer> global(jobject obj);

    static jclass getClass();
};

}

#endif // __POINTER_H__
