/*
 * String.cpp
 *
 *  Created on: 2012/08/20
 */

#include    "jointcoding.h"
#include    <string>

namespace jc {




/**
 * ネイティブの文字列型。
 * 基本的にはstd::string。対応してなければ、適宜書き換える。
 */
typedef std::string string_t;

#define _native     (native_string.get<string_t>())

String::String(const charactor* str) {
    if (str) {
        // 文字列が設定されているなら、それに従う
        native_string = ImplCapsule(new string_t(str), JC_CAPSULE_RELEAE(string_t) );
    } else {
        // それ以外は共有にnilオブジェクトを利用する
        native_string = ImplCapsule(new string_t(""), JC_CAPSULE_RELEAE(string_t) );
    }
}

/**
 * 文字列の長さを取得する
 */
const s32 String::length() const {
    return _native->length();
}

/**
 * C文字列を取得する
 */
charactor const* String::c_str() const {
    return _native->c_str();
}

/**
 * 文字列加算
 */
String String::operator+(const String &str) const {
    const string_t &a = this->native_string.as<string_t>();
    const string_t &b = str.native_string.as<string_t>();

    return String( (a+b).c_str());
}

    /**
     * 文字列加算
     */
String& String::operator+=(const String &str) {
    string_t &a = this->native_string.as<string_t>();
    const string_t &b = str.native_string.as<string_t>();

    a += b;

    return (*this);
}

/**
 * 文字列加算
 */
String& String::operator+=(const charactor* str) {
    string_t &a = this->native_string.as<string_t>();

    a += str;

    return *this;
}

/**
 * 代入を行う
 */
String& String::operator=(const charactor* origin) {
    string_t &a = this->native_string.as<string_t>();

    a = origin;

    return (*this);
}

/**
 * 比較を行う
 */
bool String::operator==(const String &str) const {
    const string_t&a = this->native_string.as<string_t>();
    const string_t &b = str.native_string.as<string_t>();

    return a == b;
}

/**
 * 文字列のindexOfを求める
 */
s32 String::indexOf(const String &str) const {
    return _native->find(str.c_str());
}

/**
 * 文字列のindexOfを求める
 */
s32 String::indexOf(const charactor *str) const {
    return _native->find(str);
}

/**
 * 文字列の一部を切り取る
 */
String String::substring(const s32 begin, const s32 end) const {
    string_t str = _native->substr(begin, end);
    return String(str.c_str());
}

}
