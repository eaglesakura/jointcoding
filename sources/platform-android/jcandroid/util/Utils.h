/*
 * jniUtils.h
 *
 *  Created on: 2012/07/06

 */

#include    "jointcoding-android.h"

namespace ndk {

/**
 * jstringから通常のStringへ変換する
 */
String j2String(jstring str, const jcboolean deleteRef);

/**
 * charactor*からjstringへ変換する
 */
jstring c2jstring(const charactor *str);

/**
 * 複数のパラメータを一括で書き込む
 */
void c2stringArray(jobjectArray array, const String* values, const s32 length);

/**
 * 一括でStringへ直す
 */
string_vector j2stringArray(jobjectArray array);

/**
 * 一括でStringへ直す
 */
void j2stringArray(jobjectArray array, unsafe_array<String> *result);

}
