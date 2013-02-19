/**
 * export from "Joint Coding Project"
 */
#ifndef __APPFONT_H__
#define __APPFONT_H__
#include "jointcoding-android.h"

namespace ndk {

class AppFont: public ::ndk::JniWrapper {
protected:
    AppFont(jobject obj);
public:
    virtual ~AppFont(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static jobject createInstance_unsafe();
    static jobject createRawImage_unsafe(jstring text, jint height);
    virtual void createImage(jstring text, jint heightPixel);
    static void createImage_(jobject _this, jstring text, jint heightPixel);
    virtual void createImage(const ::jc::charactor* text, jint heightPixel) { CALL_JNIENV();  jstring text_j = ::ndk::c2jstring(text); createImage( text_j, heightPixel);  env->DeleteLocalRef(text_j); }
    virtual jobject getImage_unsafe();
    static jobject getImage_unsafe_(jobject _this);


    static jc_sp<AppFont> wrap(jobject obj);
    static jc_sp<AppFont> global(jobject obj);

    static jclass getClass();
};

}

#endif // __APPFONT_H__
