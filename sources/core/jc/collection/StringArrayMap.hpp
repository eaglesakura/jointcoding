/*
 * StringArrayMap.hpp
 *
 *  Created on: 2013/12/15
 */
#ifndef STRINGARRAYMAP_HPP_
#define STRINGARRAYMAP_HPP_

#include    "jointcoding.h"
#include    "jc/collection/SafeArray.hpp"
#include    <vector>

namespace jc {

/**
 * 文字配列をKey-Valueマップと見せかけて値の取得・設定を行う。
 */
class StringArrayMap {

    string_vector datas;

    jcboolean readOnly;

    unsafe_array<String> pDatas;

    /**
     * データ格納用の配列Indexを取得する。
     * keyが見つからなかったら自動で拡張する
     * @param key
     * @return
     */
    s32 getDataIndex(const String key) {
        for (int i = 0; i < pDatas.length; i += 2) {
            if (pDatas[i] == key) {
                return i + 1;
            }
        }

        if (readOnly) {
            return -1;
        }

        datas.push_back(key);
        datas.push_back("");

        pDatas.ptr = &(datas[0]);
        pDatas.length = datas.size();

        return datas.size() - 1;
    }

public:
    StringArrayMap() {
        readOnly = jcfalse;
    }

    /**
     * 配列から構築を行う
     */
    StringArrayMap(const unsafe_array<String> origin) {
        pDatas = origin;
        readOnly = jctrue;
    }

    void put(const String key, const String value) {
        const s32 index = getDataIndex(key);
        if (index < 0) {
            jclogf("drop key(%s)", key.c_str());
            return;
        }

        pDatas[index] = value;
    }

    void put(const s64 key, const String value) {
        put(String::valueOfLong(key), value);
    }

    void putLong(const String key, const s64 value) {
        put(key, String::valueOfLong(value));
    }
    void putInteger(const String key, const s32 value) {
        put(key, String::valueOfLong(value));
    }
    void putBoolean(const String key, const jcboolean value) {
        put(key, String::valueOfBoolean(value));
    }

    String get(const String key) {
        const s32 index = getDataIndex(key);
        if (index < 0) {
            return "";
        }
        return pDatas[index];
    }

    /**
     * 整数値を取得する
     */
    long getLong(const String key) {
        return get(key).toLong();
    }

    /**
     * 整数値を取得する
     */
    int getInteger(const String key) {
        return get(key).toInteger();
    }

    /**
     * 浮動小数値を取得する
     */
    float getFloat(const String key) {
        return get(key).toFloat();
    }

    /**
     * 実数値を取得する
     */
    double getDouble(const String key) {
        return get(key).toDouble();
    }

    /**
     * 配列へ書き戻しを行う
     */
    void toArray(String *p) {
        string_vector::iterator itr = datas.begin(), end = datas.end();
        while (itr != end) {
            *p = *itr;

            ++p;
            ++itr;
        }
    }
};

}

#endif /* STRINGARRAYMAP_HPP_ */
