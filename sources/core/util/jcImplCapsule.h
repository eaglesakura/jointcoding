/*
 * jcImplCapsule.h
 *
 *  Created on: 2012/08/20
 */

#ifndef JCIMPLCAPSULE_H_
#define JCIMPLCAPSULE_H_

#include    "jcSmartPtr.h"

namespace jc {


/**
 * デフォルトの解放用関数
 */
template<typename T>
void impl_release_func(void *ref) {
    T* p = (T*) ref;
    SAFE_DELETE(p);
}


typedef void* impl_ref;

/**
 * オブジェクトの解放を行う
 */
typedef void (*impl_release_func_t)(impl_ref ref);

/**
 * 即興で解放関数を作成する
 */
#define JC_CAPSULE_RELEAE(T)  ((impl_release_func_t) &jc::impl_release_func<T>)

/**
 * 実装部分を完全に隠蔽して、依存をCPP内のみに抑えるようにする。
 * as()による変換失敗については保証しない。
 */
class ImplCapsule {

    /**
     * ポインタと解放関数のペアを持つ。
     * 廃棄されるとき、自動的に解放関数を呼び出す
     */
    struct tagImpl {
        /**
         * 実装本体
         */
        impl_ref ref;

        /**
         * 開放関数
         */
        impl_release_func_t release_func;

        tagImpl() {
            ref = NULL;
            release_func = NULL;
        }

        ~tagImpl() {
            if (release_func) {
                release_func(ref);
                ref = NULL;
            }
        }
    };

    /**
     * 実装本体。
     * shared_ptrで内部を共有する。
     */
    jc_sp<tagImpl> impl;

public:
    /**
     * 実装を指定する
     */
    ImplCapsule(impl_ref ref, impl_release_func_t func) {
        this->impl.reset(new tagImpl);
        this->impl->ref = ref;
        this->impl->release_func = func;
    }

    /**
     * デフォルト
     */
    ImplCapsule( ) {
    }

    /**
     * コピー
     */
    ImplCapsule(const ImplCapsule &origin) {
        this->impl = origin.impl;
    }

    ~ImplCapsule() {
    }

    /**
     * 実装オブジェクトに変換して取得する
     */
    template<typename T>
    T& as() {
        return *((T*) impl->ref);
    }

    /**
     * 実装オブジェクトに変換して取得する
     */
    template<typename T>
    const T& as() const {
        return *((T*) impl->ref);
    }

    template<typename T>
    T* get() {
        return (T*)impl->ref;
    }

    template<typename T>
    const T* get() const {
        return (T*)impl->ref;
    }
};

/**
 * 実装カプセルから型を取り出す
 */
#define     CAPSULE_AS( type, var_name, capsule )       type &var_name = capsule.as<type>();

}

#endif /* JCIMPLCAPSULE_H_ */
