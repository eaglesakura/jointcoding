/*
 * Jointable.cpp
 *
 *  Created on: 2013/04/13
 */

#include    "android-classes/ndkJointable.h"
#include    "android-classes/ndkPointer.h"
#include    "jcandroid/jni/Jointable.h"

namespace ndk {

void* getJointContextPointer_void(const jobject jJointable, const s32 key) {

    assert(jJointable != NULL);

    jobject jPointer = ndk::Jointable::getNativePointer_unsafe_(jJointable, key);
    if (!jPointer) {
        return NULL;
    }

    void* result = (void*) (ndk::Pointer::getObjectPointer_(jPointer));

    {
        // 参照廃棄
        CALL_JNIENV();
        env->DeleteLocalRef(jPointer);
    }

    return result;
}
}

