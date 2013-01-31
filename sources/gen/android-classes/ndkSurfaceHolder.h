/**
 * export from "Joint Coding Project"
 */
#ifndef __SURFACEHOLDER_H__
#define __SURFACEHOLDER_H__
#include "jointcoding-android.h"

namespace ndk {

class SurfaceHolder: public ::ndk::JniWrapper {
protected:
    SurfaceHolder(jobject obj);
public:
    virtual ~SurfaceHolder(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jobject getSurface_unsafe();
    static jobject getSurface_unsafe_(jobject _this);


    static jc_sp<SurfaceHolder> wrap(jobject obj);
    static jc_sp<SurfaceHolder> global(jobject obj);

    static jclass getClass();
};

}

#endif // __SURFACEHOLDER_H__
