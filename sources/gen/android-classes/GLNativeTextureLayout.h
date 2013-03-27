/**
 * export from "Joint Coding Project"
 */
#ifndef __GLNATIVETEXTURELAYOUT_H__
#define __GLNATIVETEXTURELAYOUT_H__
#include "jointcoding-android.h"

class GLNativeTextureLayout: public ::ndk::JniWrapper {
protected:
    GLNativeTextureLayout(jobject obj);
public:
    virtual ~GLNativeTextureLayout(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;


    static jc_sp<GLNativeTextureLayout> wrap(jobject obj);
    static jc_sp<GLNativeTextureLayout> global(jobject obj);

    static jclass getClass();
};
#endif // __GLNATIVETEXTURELAYOUT_H__
