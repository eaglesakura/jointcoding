/**
 * export from "Joint Coding Project"
 */
#ifndef __IMAGEENCODER_H__
#define __IMAGEENCODER_H__
#include "jointcoding-android.h"

namespace ndk {

class ImageEncoder: public ::ndk::JniWrapper {
protected:
    ImageEncoder(jobject obj);
public:
    virtual ~ImageEncoder(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jobject alloc_unsafe(jint bufferSize);
    static jobject alloc_unsafe_(jobject _this, jint bufferSize);
    virtual jboolean encodeRGBA();
    static jboolean encodeRGBA_(jobject _this);
    static jobject createInstance_unsafe();
    virtual void setImageSize(jint width, jint height);
    static void setImageSize_(jobject _this, jint width, jint height);


    static jc_sp<ImageEncoder> wrap(jobject obj);
    static jc_sp<ImageEncoder> global(jobject obj);

    static jclass getClass();
};

}

#endif // __IMAGEENCODER_H__
