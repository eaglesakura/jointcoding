/*
 * String.cpp
 *
 *  Created on: 2012/08/20
 */

#include    "jointcoding.h"
#include    "jc/system/StringUtil.h"
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
 * 空文字だった場合、trueを返す
 */
jcboolean String::empty() const {
    return _native->empty();
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

    return String((a + b).c_str());
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
    const string_t &a = this->native_string.as<string_t>();
    const string_t &b = str.native_string.as<string_t>();

    return a == b;
}

/**
 * 比較を行う
 */
bool String::operator!=(const String &str) const {
    const string_t &a = this->native_string.as<string_t>();
    const string_t &b = str.native_string.as<string_t>();

    return a != b;
}

/**
 * map比較演算子
 */
bool String::operator<(const String &str) const {
    const string_t &a = this->native_string.as<string_t>();
    const string_t &b = str.native_string.as<string_t>();

    return a < b;
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
/**
 * 文字列をセパレーターに従って分割する
 */std::vector<String> String::split(const charactor *sep) const {
    std::vector<String> result;
    jc::split((*this), String(sep), &result);
    return result;
}
/**
 * 文字列のハッシュコードを求める
 */
s32 String::hashCode() const {
    if (empty()) {
        return 0;
    }

    const charactor *str = c_str();

    s32 result = 0;
    const s32 str_length = length();
    for (int i = 0; i < str_length; i++) {
        result = (result * 31 + str[i]);
    }
    return result;
}

/**
 * 文字列として取得する
 */
String String::format(const charactor *fmt, ...) {
    jc_sa<charactor> temp(new charactor[strlen(fmt) + 256]);
    va_list ap;
    va_start(ap, fmt);
    {
        vsprintf(temp.get(), fmt, ap);
    }
    va_end(ap);
    return String(temp.get());
}

}
