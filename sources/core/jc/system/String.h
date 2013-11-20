/*
 * jcString.h
 *
 *  Created on: 2012/08/20
 */

#ifndef JCSTRING_H_
#define JCSTRING_H_

#include    "jc/system/Macro.h"
#include    <vector>
#include    <string>

namespace jc {

class String {

    /**
     * ネイティブ実装の文字列
     */
    jc_sp<std::string> text;
public:
    String(const charactor* str = NULL);

    /**
     * 内部コピーする
     */
    String(const String &origin);

    ~String();

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
        text = cpy.text;
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
     * map比較演算子
     */
    bool operator<(const String &b) const;

    /**
     * 比較を行う
     */
    bool operator==(const String &r) const;

    /**
     * 比較を行う
     */
    bool operator!=(const String &r) const;

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
     * 空文字だった場合、trueを返す
     */
    jcboolean empty() const;

    /**
     * 文字列をセパレーターに従って分割する
     */
    std::vector<String> split(const charactor *sep) const;

    /**
     * ネイティブクラスに変換して取得する
     * 変換は必ずJC_NATIVE_STRINGを利用しなければならない。
     */
    std::string& get() {
        return *text.get();
    }

    /**
     * ネイティブクラスに変換して取得する
     */
    const std::string& get() const {
        return *text.get();
    }

    /**
     * フォーマットを指定してStringを作成する
     */
    static String format(const charactor *fmt, ...);
};
}

#endif /* JCSTRING_H_ */
