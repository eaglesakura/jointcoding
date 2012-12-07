/*
 * NDK.h
 *
 *  Created on: 2012/07/11
 */

#ifndef NDK_H_
#define NDK_H_

#include "jni.h"

void initJniEnv(JNIEnv *jni);
namespace ndk {

class NDK {
public:
    static JNIEnv *getJniEnv();
    static JavaVM *getJavaVM();
};

#define CALL_JNIENV    JNIEnv  *env = ndk::NDK::getJniEnv

} /* namespace jc */
#endif /* NDK_H_ */
