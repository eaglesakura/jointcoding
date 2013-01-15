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
String j2String(jstring jStr) {
    CALL_JNIENV();

    const char* str = env->GetStringUTFChars(jStr, NULL);
    String result = (const charactor*) str;
    env->ReleaseStringUTFChars(jStr, str);

    return result;
}

}

