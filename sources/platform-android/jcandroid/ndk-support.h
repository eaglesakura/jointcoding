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

}/* namespace jc */


/**
 * res/integer
 */
#define NDK_RES_INTEGER(id)     ((s32)::ndk::NativeContext::getInteger(id))

/**
 * res/dimension
 */
#define NDK_RES_DIMENSION(id)   ((float)::ndk::NativeContext::getDimension(id))

/**
 * res/dimension
 */
#define NDK_RES_DIMENSION_VEC2(id_x, id_y)   (::jc::Vector2f(NDK_RES_DIMENSION(id_x), NDK_RES_DIMENSION(id_y)))

/**
 * res/string
 */
#define NDK_RES_STRING(id)      (::ndk::j2String(::ndk::NativeContext::getString(id), jctrue))

/**
 * res/color
 */
#define NDK_RES_COLOR(id)       (::jc::Color::fromRGBAi( ::ndk::NativeContext::getColorRGBA(id) ))



#endif /* NDK_H_ */
