/*
 * SafeArray.hpp
 *
 *  Created on: 2013/05/21
 */

#include    "jointcoding.h"

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

    s32 values_length;

    /**
     * 指定した長さの配列を確保する
     */
    inline void alloc(const s32 length) {
        assert(length > 0);

        SAFE_DELETE_ARRAY(values);
        values = new value_type[length];
        values_length = length;
    }

    /**
     * メモリ領域を高速コピーする
     */
    inline void memcpy(const value_type *origin, const s32 num) {
        // 正常な長さを持たなければならない
        assert(values_length >= num);
        memcpy(values, origin, sizeof(value_type) * num);
    }

    safe_array() {
        values = NULL;
        values_length = 0;
    }

    safe_array(const s32 length) {
        values = NULL;
        alloc(length);
    }

    ~safe_array() {
        SAFE_DELETE_ARRAY(values);
    }

    /**
     * オペレータアクセスを提供する
     */
    inline value_type& operator[](const s32 index) {
        assert(index < values_length);
        return values[index];
    }

    /**
     * オペレータアクセスを提供する
     */
    inline const value_type& operator[](const s32 index) const {
        assert(index < values_length);
        return values[index];
    }
};

}

