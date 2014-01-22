/**
 * export from "Joint Coding Project"
 */
#ifndef __IMAGEDECODER_H__
#define __IMAGEDECODER_H__
#include "jointcoding-android.h"

namespace ndk {

class ImageDecoder: public ::ndk::JniWrapper {
protected:
    ImageDecoder(jobject obj);
public:
    virtual ~ImageDecoder(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jobject getPixels_unsafe();
    static jobject getPixels_unsafe_(jobject _this);
    static jobject decodeBitmapFromNativeUri_unsafe(jstring nativeUri);
    virtual jint getHeight();
    static jint getHeight_(jobject _this);
    static jobject decodeFromStream_unsafe(jobject stream);
    static jobject decodeBitmapFromStream_unsafe(jobject stream);
    virtual jint getWidth();
    static jint getWidth_(jobject _this);
    static jobject decodeFromBitmap_unsafe(jobject image, jboolean recycle);


    static jc_sp<ImageDecoder> wrap(jobject obj);
    static jc_sp<ImageDecoder> global(jobject obj);

    static jclass getClass();
};

}

#endif // __IMAGEDECODER_H__
