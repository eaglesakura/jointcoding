/*
 * SelectionPtr.hpp
 *
 *  Created on: 2013/04/12
 */

#ifndef SELECTIONPTR_HPP_
#define SELECTIONPTR_HPP_

namespace jc {

/**
 * 通常のポインタ、スマートポインタ、ウィークポインタのいずれかを選んで設定する。
 * jc_sp/jc_wpを利用する。
 * 生ポインタは、例えば最上位クラス等、オブジェクトの生存を設計上完全にコントロール可能な場合に利用する。
 *
 * 利用する側は通常のポインタのように振る舞う。
 */
template<typename T>
class SelectionPtr {
    /**
     * 生ポインタ
     */
    T *raw;

    /**
     *
     */
    jc_sp<T> shared;

    /**
     *
     */
    jc_wp<T> weak;

public:
    SelectionPtr() {
        raw = NULL;
    }

    /**
     * コピーコンストラクタ
     */
    SelectionPtr(const SelectionPtr &origin) {
        this->raw = origin.raw;
        this->shared = origin.shared;
        this->weak = origin.weak;
    }

    /**
     * 生ポインタで生成
     */
    SelectionPtr(const T *p) {
        this->raw = p;
    }

    /**
     *
     */
    SelectionPtr(const jc_sp<T> sp) {
        this->raw = NULL;
        this->shared = sp;
    }

    /**
     *
     */
    SelectionPtr(const jc_wp<T> wp) {
        this->raw = NULL;
        this->weak = wp;
    }

    /**
     * ポインタが有効であればtrueを返す
     */
    jcboolean expired() const {
        return (raw || shared || weak.expired());
    }

    /**
     * アロー演算子
     */
    T* operator->() {
        if (raw) {
            return raw;
        } else if (shared) {
            return shared.get();
        } else {
            return weak.lock().get();
        }
    }

    /**
     * アロー演算子
     */
    const T* operator->() const {
        if (raw) {
            return raw;
        } else if (shared) {
            return shared.get();
        } else {
            return weak.lock().get();
        }
    }

    /**
     * ポインタをリセットする
     */
    void reset() {
        raw = NULL;
        weak.reset();
        shared.reset();
    }

    void reset(const T *p) {
        reset();
        raw = p;
    }

    void reset(const jc_sp<T> p) {
        reset();
        shared = p;
    }

    void reset(const jc_wp<T> p) {
        reset();
        weak = p;
    }

    template<typename DC>
    SelectionPtr<DC> downcast() const {
        SelectionPtr<DC> result;

        if (raw) {
            result.raw = dynamic_cast<DC>(raw);
        } else if (shared) {
            result.shared = jc::downcast<DC>(shared);
        } else {
            result.weak = jc::downcast_weak<DC>(weak);
        }

        return result;
    }
};
}

/**
 * selection_ptr
 */
#define jc_selp     ::jc::SelectionPtr

#endif /* SELECTIONPTR_HPP_ */
