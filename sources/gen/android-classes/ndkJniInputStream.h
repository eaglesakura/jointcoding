/**
 * export from "Joint Coding Project"
 */
#ifndef __JNIINPUTSTREAM_H__
#define __JNIINPUTSTREAM_H__
#include "jointcoding-android.h"

namespace ndk {

class JniInputStream: public ::ndk::JniWrapper {
protected:
    JniInputStream(jobject obj);
public:
    virtual ~JniInputStream(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual void close();
    static void close_(jobject _this);
    virtual jint read();
    static jint read_(jobject _this);
    virtual jint available();
    static jint available_(jobject _this);
    virtual jint read(jbyteArray buffer);
    static jint read_(jobject _this, jbyteArray buffer);
    virtual jint read(jbyteArray buffer, jint offset, jint length);
    static jint read_(jobject _this, jbyteArray buffer, jint offset, jint length);
    virtual jlong skip(jlong byteCount);
    static jlong skip_(jobject _this, jlong byteCount);


    static jc_sp<JniInputStream> wrap(jobject obj);
    static jc_sp<JniInputStream> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JNIINPUTSTREAM_H__
