/*
 * ArrayHandle.hpp
 *
 *  Created on: 2013/05/17
 */

#ifndef ARRAYHANDLE_HPP_
#define ARRAYHANDLE_HPP_

#include    "jointcoding.h"
#include    <vector>

namespace jc {

/**
 * ハンドル情報
 */
struct handle {
    s32 index;

    handle() {
        index = 0;
    }
};

/**
 * データカプセル
 * 実体を指し示す
 */
template<typename value_type>
struct array_capsule {
    /**
     * 最終的に参照する実体データ
     */
    std::vector<value_type> values;

    /**
     * 次の割り当てインデックス
     */
    s32 alloc_index;

    array_capsule() {
        alloc_index = 0;
    }
};

/**
 * 直列的な配列をハンドルを利用して管理する
 * 実体インスタンスの切り替えやポインタの生存を一元管理するようにする
 *
 * VRAM等の管理に利用する
 */
template<typename value_type>
class ArrayHandle: public Object {
public:
private:
    /**
     * 配列実体を保持する
     */
    jc_sp< array_capsule<value_type> > ref;

public:
    ArrayHandle() {
    }

    /**
     * masterの配列を共有し、続きから割り当てを行う
     */
    void sharedFrom( const jc_sp<ArrayHandle> master ) {
        this->ref = master->ref;
    }

    /**
     * 値の取得を行う
     */
    const value_type get(const s32 index) const {
        assert(index >= 0);
        assert(index < ref->array->size());
        return ref->values[index];
    }

    /**
     * 値の取得を行う
     */
    value_type get(const s32 index) {
        assert(index >= 0);
        assert(index < ref->array->size());
        return ref->values[index];
    }

    /**
     * 次のハンドルを割り当てる
     */
    handle alloc() {
        handle result;
        result.index = ref->alloc_index++;
        return result;
    }
};

template<typename value_type>
class Handle {
    /**
     * マスターデータ
     */
    jc_sp<ArrayHandle<value_type> > master;

    /**
     * 自身のハンドルデータ
     */
    handle self;

public:
    Handle() {
    }

    /**
     * 値を取得する
     */
    const value_type get() const {
        if(self.index < 0) {
            return (value_type)0;
        }

        assert(self.index >= 0);
        assert(master);

        return master->get(self.index);
    }

    /**
     * 値を取得する
     */
    value_type get() {
        if(self.index < 0) {
            return (value_type)0;
        }

        assert(self.index >= 0);
        assert(master);

        return master->get(self.index);
    }

    /**
     * オペレータアクセスを行う
     */
    const value_type operator->() const {
        assert(self.index >= 0);
        return get();
    }

    /**
     * オペレータアクセスを行う
     */
    value_type operator->() {
        assert(self.index >= 0);
        return get();
    }
};

}

#endif /* ARRAYHANDLE_HPP_ */
