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
 * 例えば、ライブラリ内のリスナポインタ保持等、個々のアプリごとに設計思想が違う場合に利用する。
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
    template<typename T2>
    SelectionPtr(const T2 *p) {
        this->raw = (T*) p;
    }

    /**
     *
     */
    template<typename T2>
    SelectionPtr(const jc_sp<T2> &sp) {
        this->raw = NULL;
        this->shared = sp;
    }

    /**
     *
     */
    template<typename T2>
    SelectionPtr(const jc_wp<T2> &wp) {
        this->raw = NULL;
        this->weak = wp;
    }

    /**
     * ポインタが有効であればtrueを返す
     */
    jcboolean exist() const {
        return (raw || shared || !weak.expired());
    }

    /**
     * 生ポインタを取得する
     */
    T* get() {
        if (raw) {
            return raw;
        } else if (shared) {
            return shared.get();
        } else {
            return weak.lock().get();
        }
    }

    /**
     * 生ポインタを取得する
     */
    T* get() const {
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
    T* operator->() {
        T* result = get();
        assert(result != NULL);
        return result;
    }

    /**
     * アロー演算子
     */
    const T* operator->() const {
        T* result = get();
        assert(result != NULL);
        return result;
    }

    /**
     * ポインタをリセットする
     */
    void reset() {
        raw = NULL;
        weak.reset();
        shared.reset();
    }

    /**
     * ポインタをリセットする
     */
    void reset(const T *p) {
        reset();
        raw = p;
    }

    /**
     * ポインタをリセットする
     */
    void reset(const jc_sp<T> &p) {
        reset();
        shared = p;
    }

    /**
     * ポインタをリセットする
     */
    void reset(const jc_wp<T> &p) {
        reset();
        weak = p;
    }

    /**
     * 特定の型へダウンキャストする
     */
    template<typename DC>
    SelectionPtr<DC> downcast() const {
        SelectionPtr<DC> result;

        if (raw) {
            result.raw = dynamic_cast<DC>(raw);
        } else if (shared) {
            result.shared = jc::downcast<DC>(shared);
        } else {
            result.weak = jc::downcast<DC>(weak.lock());
        }

        return result;
    }

    /**
     * 同値である場合はtrue
     */
    bool operator==(const SelectionPtr &p) const {
        return get() == dynamic_cast<T*>(p.get());
    }

    /**
     * 異なる値である場合はtrue
     */
    bool operator!=(const SelectionPtr &p) const {
        return get() != dynamic_cast<T*>(p.get());
    }

    operator bool() const {
        return exist();
    }

    bool operator!() const {
        return !exist();
    }

    SelectionPtr<T>& operator=(const SelectionPtr<T> &cpy) {
        this->raw = cpy.raw;
        this->shared = cpy.shared;
        this->weak = cpy.weak;
        return (*this);
    }
};
}

/**
 * selection_ptr
 */
#define jc_selp     ::jc::SelectionPtr

#endif /* SELECTIONPTR_HPP_ */
