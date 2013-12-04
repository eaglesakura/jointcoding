/*
 * String.cpp
 *
 *  Created on: 2012/08/20
 */

#include    "jointcoding.h"
#include    "jc/system/StringUtil.h"
#include    <string>

namespace jc {

String::String(const charactor* str) {
    if (str) {
        this->text.reset(mark_new string_t(str, strlen(str)));
    } else {
        this->text.reset(mark_new string_t());
    }
}

/**
 * 内部コピーする
 */
String::String(const String &origin) {
    this->text = origin.text;
}

String::~String() {
}

/**
 * 文字列の長さを取得する
 */
const s32 String::length() const {
    return text->length();
}

/**
 * 空文字だった場合、trueを返す
 */
jcboolean String::empty() const {
    return text->empty();
}

/**
 * C文字列を取得する
 */
charactor const* String::c_str() const {
    return text->c_str();
}

/**
 * s32に変換する
 */
s32 String::toInteger() const {
    return atoi(text->c_str());
}

/**
 * s64に変換する
 */
s64 String::toLong() const {
    return atoll(text->c_str());
}

/**
 * floatに変換する
 */
float String::toFloat() const {
    return (float) atof(text->c_str());
}

/**
 * floatに変換する
 */
double String::toDouble() const {
    return atof(text->c_str());
}

/**
 * 文字列加算
 */
String String::operator+(const String &str) const {
    const string_t &a = this->get();
    const string_t &b = str.get();

    return String((a + b).c_str());
}

/**
 * 文字列加算
 */
String& String::operator+=(const String &str) {
    string_t &a = this->get();
    const string_t &b = str.get();

    a += b;

    return (*this);
}

/**
 * 文字列加算
 */
String& String::operator+=(const charactor* str) {
    string_t &a = this->get();

    a += str;

    return *this;
}

/**
 * 代入を行う
 */
String& String::operator=(const charactor* origin) {
    string_t &a = this->get();

    a = origin;

    return (*this);
}

/**
 * 比較を行う
 */
bool String::operator==(const String &str) const {
    const string_t &a = this->get();
    const string_t &b = str.get();

    return a == b;
}

/**
 * 比較を行う
 */
bool String::operator!=(const String &str) const {
    const string_t &a = this->get();
    const string_t &b = str.get();

    return a != b;
}

/**
 * map比較演算子
 */
bool String::operator<(const String &str) const {
    const string_t &a = this->get();
    const string_t &b = str.get();

    return a < b;
}

/**
 * 文字列のindexOfを求める
 */
s32 String::indexOf(const String &str) const {
    return text->find(str.c_str());
}

/**
 * 文字列のindexOfを求める
 */
s32 String::indexOf(const charactor *str) const {
    return text->find(str);
}

/**
 * 文字列の一部を切り取る
 */
String String::substring(const s32 begin, const s32 end) const {
    const string_t str = text->substr(begin, end);
    return String(str.c_str());
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
    for (s32 i = 0; i < str_length; i++) {
        result = (result * 31 + str[i]);
    }
    return result;
}

/**
 * 文字列をセパレーターに従って分割する
 */
std::vector<String, StlAllocator<String> > String::split(const charactor *sep) const {
    std::vector<String, StlAllocator<String> > result;
    jc::split((*this), String(sep), &result);
    return result;
}

/**
 * 文字列として取得する
 */
String String::format(const charactor *fmt, ...) {
    jc_sa<charactor> temp(mark_new charactor[strlen(fmt) + 256]);
    va_list ap;
    va_start(ap, fmt);
    {
        vsprintf(temp.get(), fmt, ap);
    }
    va_end(ap);
    return String(temp.get());
}
/**
 * 値から生成する
 */
String String::valueOfDoble(const double value) {
    charactor str[32] = "";
    sprintf(str, "%f", value);
    return String(str);
}

/**
 * 値から生成する
 */
String String::valueOfInt(const s32 value) {
    charactor str[32] = "";
    sprintf(str, "%d", value);
    return String(str);
}

/**
 * 値から生成する
 */
String String::valueOfLong(const s64 value) {
    charactor str[32] = "";
    sprintf(str, "%lld", value);
    return String(str);

}

/**
 * 値から生成する
 */
String String::valueOfBoolean(const jcboolean value) {
    return String(value ? "true" : "false");
}

}
