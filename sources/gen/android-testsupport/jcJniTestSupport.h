/**
 * export from "Joint Coding Project"
 */
#ifndef __JNITESTSUPPORT_H__
#define __JNITESTSUPPORT_H__
#include "jointcoding-android.h"

namespace jc {

class JniTestSupport: public ::ndk::JniWrapper {
protected:
    JniTestSupport(jobject obj);
public:
    virtual ~JniTestSupport(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    static void callJniTest(jstring string_0, jstring string_1, jstring string_2);


    static jc_sp<JniTestSupport> wrap(jobject obj);
    static jc_sp<JniTestSupport> global(jobject obj);

    static jclass getClass();
};

}

#endif // __JNITESTSUPPORT_H__
