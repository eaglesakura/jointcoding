/**
 * export from "Joint Coding Project"
 */
#ifndef __NATIVEIOUTIL_H__
#define __NATIVEIOUTIL_H__
#include "jointcoding-android.h"

namespace ndk {

class NativeIOUtil: public ::ndk::JniWrapper {
protected:
    NativeIOUtil(jobject obj);
public:
    virtual ~NativeIOUtil(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static jobject openFromAssets_unsafe(jstring fileName, jobject appContext);
    static jobject openFromLocalStrage_unsafe(jstring path, jobject appContext);
    static jstring localStoragePath2NativePath(jstring path);
    static jobject openFromExternalStrage_unsafe(jstring path, jobject appContext);


    static jc_sp<NativeIOUtil> wrap(jobject obj);
    static jc_sp<NativeIOUtil> global(jobject obj);

    static jclass getClass();
};

}

#endif // __NATIVEIOUTIL_H__
