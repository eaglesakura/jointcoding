/**
 * export from "Joint Coding Project"
 */
#ifndef __POINTERLIST_H__
#define __POINTERLIST_H__
#include "jointcoding-android.h"

namespace ndk {

class PointerList: public ::ndk::JniWrapper {
protected:
    PointerList(jobject obj);
public:
    virtual ~PointerList(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void remove(jint key);
    static void remove_(jobject _this, jint key);
    virtual void remove(jstring key);
    static void remove_(jobject _this, jstring key);
    virtual void remove(const ::jc::charactor* key) { CALL_JNIENV();  jstring key_j = ::ndk::c2jstring(key); remove( key_j);  env->DeleteLocalRef(key_j); }


    static jc_sp<PointerList> wrap(jobject obj);
    static jc_sp<PointerList> global(jobject obj);

    static jclass getClass();
};

}

#endif // __POINTERLIST_H__
