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
String j2String(jstring);

/**
 * charactor*からjstringへ変換する
 */
jstring c2jstring(const charactor *str);

}
