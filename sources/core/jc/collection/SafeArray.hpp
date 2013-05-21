/*
 * SafeArray.hpp
 *
 *  Created on: 2013/05/21
 */

#ifndef  SAFE_ARRAY_HPP_
#define  SAFE_ARRAY_HPP_

#include    "jointcoding.h"
#include    "jc/math/Math.h"

namespace jc {

/**
 * 配列のポインタと長さを安全に保持する
 * 単純な配列のみをサポートし、軽量に動作することを第一とする
 */
template<typename value_type>
struct safe_array {
    /**
     * コピーコンストラクタはサポートしない
     */
    safe_array(const safe_array& cpy);
private:
    /**
     * 配列本体
     */
    value_type *values;

    s32 length;

    /**
     * 指定した長さの配列を確保し、古い配列をmemcpyする
     */
    inline void alloc(const s32 newLength) {
        assert(newLength > 0);

        value_type *pOldValues = values;
        values = new value_type[newLength];

        // 古い配列が存在するならコピーする
        if (pOldValues) {
            memcpy(values, pOldValues, sizeof(value_type) * jc::min(length, newLength));

            // 古い配列を削除する
            SAFE_DELETE_ARRAY(pOldValues);
        }

        // 長さを保存する
        length = newLength;
    }

    /**
     * 指定した長さの配列を確保し、要素をコピーする
     * operator=の起動が必要なシーンで利用する
     */
    inline void reserve(const s32 newLength) {
        assert(newLength > 0);

        value_type *pOldValues = values;
        values = new value_type[newLength];

        // 古い配列が存在するならコピーする
        if (pOldValues) {
            const s32 min_length = jc::min(length, newLength);

            for (s32 i = 0; i < min_length; ++i) {
                values[i] = pOldValues[i];
            }

            // 古い配列を削除する
            SAFE_DELETE_ARRAY(pOldValues);
        }

        // 長さを保存する
        length = newLength;
    }

    /**
     * メモリ領域を高速コピーする
     * operatorが不要な場合に利用する
     */
    inline void memcopyFrom(const value_type *origin, const s32 num) {
        // 正常な長さを持たなければならない
        assert(length >= num);
        memcpy(values, origin, sizeof(value_type) * num);
    }

    /**
     * メモリ領域を一つ一つコピーする
     * operatorが必要な場合に利用する
     * memcpyFromに比べて低速になる
     */
    inline void copyFrom(const value_type *origin, const s32 num) {
        // 正常な長さを持たなければならない
        assert(length >= num);

        for (s32 i = 0; i < num; ++i) {
            values[i] = origin[i];
        }
    }

    safe_array() {
        values = NULL;
        length = 0;
    }

    safe_array(const s32 length) {
        values = NULL;
        length = 0;
        alloc(length);
    }

    ~safe_array() {
        SAFE_DELETE_ARRAY(values);
    }

    /**
     * オペレータアクセスを提供する
     */
    inline value_type& operator[](const s32 index) {
        assert(index < length);
        return values[index];
    }

    /**
     * オペレータアクセスを提供する
     */
    inline const value_type& operator[](const s32 index) const {
        assert(index < length);
        return values[index];
    }
};

}

#endif
