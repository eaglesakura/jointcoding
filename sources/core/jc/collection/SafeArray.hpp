/*
 * SafeArray.hpp
 *
 *  Created on: 2013/05/21
 */

#ifndef  SAFE_ARRAY_HPP_
#define  SAFE_ARRAY_HPP_

#include    "jointcoding.h"
#include    "jc/math/Math.h"
#include    "jc/mem/MemoryUtil.h"

namespace jc {

/**
 * safe_arrayから一部を切り取ったクラス
 * ポインタの削除は行わないため、利用する箇所に注意すること
 */
template<typename value_type>
struct unsafe_array {
    value_type *ptr;
    s32 length;

    unsafe_array() {
        ptr = NULL;
        length = 0;
    }

    unsafe_array(const value_type *p, const s32 length) {
        this->ptr = (value_type*)p;
        this->length = length;
    }
    template<typename value_type2>
    unsafe_array(const unsafe_array<value_type2> &cpy) {
        this->ptr = (value_type*)cpy.ptr;
        this->length = cpy.length;
    }

    /**
     * オペレータアクセスを提供する
     */
    inline value_type& operator[](const s32 index) {
        assert(index < length);
        return ptr[index];
    }

    /**
     * オペレータアクセスを提供する
     */
    inline const value_type& operator[](const s32 index) const {
        assert(index < length);
        return ptr[index];
    }

    /**
     * ポインタを次へ移動する
     */
    inline unsafe_array<value_type>& operator++() {
        ++ptr;
        --length;
        if (!length) {
            ptr = NULL;
        }
        return *this;
    }

    /**
     * アロー演算子を提供する
     */
    inline value_type* operator->() const {
        assert(ptr);
        return ptr;
    }

    /**
     * 参照を提供する
     */
    inline value_type& operator*() const {
        assert(ptr);
        return *ptr;
    }

    /**
     * 有効な場合はtrueを返す
     */
    operator bool() const {
        return ptr != NULL;
    }

    /**
     * 無効な場合はtrueを返す
     */
    bool operator!() const {
        return ptr == NULL;
    }
};

/**
 * 配列のポインタと長さを安全に保持する
 * 単純な配列のみをサポートし、軽量に動作することを第一とする
 */
template<typename value_type>
struct safe_array {
private:
    /**
     * コピーコンストラクタはサポートしない
     */
    safe_array(const safe_array& cpy);

    /**
     * コピーをサポートしない
     */
    safe_array& operator=(const safe_array &cpy);
public:
    /**
     * 配列本体
     */
    value_type *ptr;

    s32 length;

    /**
     * 指定した長さの配列を確保し、古い配列をmemcpyする
     */
    inline void alloc(const s32 newLength) {
        assert(newLength >= 0);

        value_type *pOldValues = ptr;
        if (newLength > 0) {
            ptr = new value_type[newLength];
        } else {
            ptr = NULL;
        }

        // 古い配列が存在するならコピーする
        if (pOldValues) {
            memcpy(ptr, pOldValues, sizeof(value_type) * jc::min(length, newLength));

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
        assert(newLength >= 0);

        value_type *pOldValues = ptr;
        if (newLength > 0) {
            ptr = mark_new value_type[newLength];
        } else {
            ptr = NULL;
        }

        // 古い配列が存在するならコピーする
        if (pOldValues) {
            const s32 min_length = jc::min(length, newLength);

            for (s32 i = 0; i < min_length; ++i) {
                ptr[i] = pOldValues[i];
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
        memcpy(ptr, origin, sizeof(value_type) * num);
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
            ptr[i] = origin[i];
        }
    }

    /**
     * 内部で持っているメモリをゼロ埋めする
     */
    inline void zeromemory() {
        ::jc::zeromemory(ptr, sizeof(value_type) * length);
    }

    safe_array() {
        ptr = NULL;
        length = 0;
    }

    safe_array(const s32 length) {
        ptr = NULL;
        length = 0;
        alloc(length);
    }

    ~safe_array() {
        SAFE_DELETE_ARRAY(ptr);
    }

    /**
     * オペレータアクセスを提供する
     */
    inline value_type& operator[](const s32 index) {
        assert(index < length);
        return ptr[index];
    }

    /**
     * オペレータアクセスを提供する
     */
    inline const value_type& operator[](const s32 index) const {
        assert(index < length);
        return ptr[index];
    }

    /**
     * 配列の一部を切り取ったunmanaged配列を生成する
     */
    inline unsafe_array<value_type> slice(const s32 index, const s32 length) const {
        assert(index >= 0);
        assert((index + length) <= this->length);

        unsafe_array<value_type> result;
        result.ptr = (ptr + index);
        result.length = length;
        return result;
    }

    /**
     * 配列の一部を切り取ったunmanaged配列を生成する
     */
    inline unsafe_array<value_type> slice(const s32 index) const {
        return slice(index, length - index);
    }

    /**
     * イテレータアクセス用のunmanaged配列を生成する
     */
    inline unsafe_array<value_type> iterator() const {
        return slice(0, length);
    }

    /**
     * 有効な場合はtrueを返す
     */
    operator bool() const {
        return ptr != NULL;
    }

    /**
     * 無効な場合はtrueを返す
     */
    bool operator!() const {
        return ptr == NULL;
    }
};

/**
 * 個々の要素に対してSAFE_DELETEをかける
 */
template<typename value_type>
inline void safe_delete(safe_array<value_type> &values) {
    for (s32 i = 0; i < values.length; ++i) {
        SAFE_DELETE(values.ptr[i]);
    }
}

}

#endif
