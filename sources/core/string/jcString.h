/*
 * jcString.h
 *
 *  Created on: 2012/08/20
 */

#ifndef JCSTRING_H_
#define JCSTRING_H_

#include    "jcMacro.h"

namespace jc {

class String {
    /**
     * ネイティブ実装の文字列
     */
    ImplCapsule native_string;
public:
    String(const charactor* str = NULL);

    /**
     * 内部コピーする
     */
    String(const String &origin) {
        this->native_string = origin.native_string;
    }

    /**
     * 文字列の長さを取得する
     */
    const s32 length() const;

    /**
     * C文字列を取得する
     */
    charactor const* c_str() const;

    /**
     * 代入を行う
     */
    String& operator=(const String &cpy) {
        native_string = cpy.native_string;
        return *this;
    }

    /**
     * 文字列加算
     */
    String operator+(const String &str) const;

    /**
     * 文字列加算
     */
    String& operator+=(const String &str);

    /**
     * 文字列加算
     */
    String& operator+=(const charactor* str);

    /**
     * 代入を行う
     */
    String& operator=(const charactor* origin);

    /**
     * 比較を行う
     */
    bool operator==(const String &r) const;

    /**
     * 文字列のindexOfを求める
     */
    s32 indexOf(const String &str) const;

    /**
     * 文字列のindexOfを求める
     */
    s32 indexOf(const charactor *str) const;

    /**
     * 文字列の一部を切り取る
     */
    String substring(const s32 begin, const s32 end) const;

    /**
     * ネイティブクラスに変換して取得する
     * 変換は必ずJC_NATIVE_STRINGを利用しなければならない。
     */
    template<typename T>
    T& get_native() {
        return native_string.as<T>();
    }

    /**
     * ネイティブクラスに変換して取得する
     */
    template<typename T>
    const T& get_native() const {
        return native_string.as<T>();
    }
};
}

#endif /* JCSTRING_H_ */
