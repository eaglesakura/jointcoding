/*
 * jniUtils.cpp
 *
 *  Created on: 2012/07/06

 */
#include    "jointcoding-android.h"
#include    "jni.h"
#include    "jcandroid/util/utils.h"

namespace ndk {

/**
 * jstringから通常のStringへ変換する
 */
String j2String(jstring jStr, const jcboolean deleteRef) {
    CALL_JNIENV();

    const char* str = env->GetStringUTFChars(jStr, NULL);
    String result = (const charactor*) str;
    env->ReleaseStringUTFChars(jStr, str);

    if(deleteRef) {
        env->DeleteLocalRef(jStr);
    }

    return result;
}
/**
 * charactor*からjstringへ変換する
 */
jstring c2jstring(const charactor *str) {
    CALL_JNIENV();
    return env->NewStringUTF(str);
}

}

