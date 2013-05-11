/**
 * export from "Joint Coding Project"
 */
#ifndef __OBJMONITORING_H__
#define __OBJMONITORING_H__
#include "jointcoding-android.h"

namespace ndk {

class ObjMonitoring: public ::ndk::JniWrapper {
protected:
    ObjMonitoring(jobject obj);
public:
    virtual ~ObjMonitoring(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static jobject createInstance_unsafe(jobject target);
    virtual jboolean exist();
    static jboolean exist_(jobject _this);
    virtual jobject get_unsafe();
    static jobject get_unsafe_(jobject _this);


    static jc_sp<ObjMonitoring> wrap(jobject obj);
    static jc_sp<ObjMonitoring> global(jobject obj);

    static jclass getClass();
};

}

#endif // __OBJMONITORING_H__
