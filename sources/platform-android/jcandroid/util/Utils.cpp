/*
 * jniUtils.cpp
 *
 *  Created on: 2012/07/06

 */
#include    "jointcoding-android.h"
#include    "jni.h"
#include    "jcandroid/util/Utils.h"

namespace ndk {

/**
 * jstringから通常のStringへ変換する
 */
String j2String(jstring jStr, const jcboolean deleteRef) {
    assert(jStr != NULL);

    CALL_JNIENV();

    const char* str = env->GetStringUTFChars(jStr, NULL);
    String result = (const charactor*) str;
    env->ReleaseStringUTFChars(jStr, str);

    if (deleteRef) {
        env->DeleteLocalRef(jStr);
    }

    return result;
}
/**
 * charactor*からjstringへ変換する
 */
jstring c2jstring(const charactor *str) {
    assert(str != NULL);

    CALL_JNIENV();
    return env->NewStringUTF(str);
}

/**
 * 複数のパラメータを一括で書き込む
 */
void c2stringArray(jobjectArray array, const String* values, const s32 length) {
    CALL_JNIENV();
    for( int i = 0; i < length; ++i) {
        jclogf("call(%d)", i);
        jobject jStr = (jobject)c2jstring(values[i].c_str());
        env->SetObjectArrayElement(array, i, jStr);
        env->DeleteLocalRef(jStr);
    }
}

/**
 * 一括でStringへ直す
 */
string_vector j2stringArray(jobjectArray array) {
    assert(array);

    CALL_JNIENV();
    const s32 length = env->GetArrayLength(array);

    string_vector result;

    for (int i = 0; i < length; ++i) {
        jstring jStr = (jstring) env->GetObjectArrayElement(array, i);

        if (jStr) {
            result.push_back(j2String(jStr, jcfalse));
            env->DeleteLocalRef(jStr);
        }else {
            result.push_back(String());
        }

    }

    return result;

}

/**
 * 一括でStringへ直す
 */
void j2stringArray(jobjectArray array, unsafe_array<String> *result) {
    assert(array);

    CALL_JNIENV();

    const s32 length = env->GetArrayLength(array);
    assert(length <= result->length);
    for (int i = 0; i < length; ++i) {
        jstring jStr = (jstring) env->GetObjectArrayElement(array, i);

        if (jStr) {
            (*result)[i] = j2String(jStr, jcfalse);
            env->DeleteLocalRef(jStr);
        }
    }

}

}

