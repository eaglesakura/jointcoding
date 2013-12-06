/*
 * IDataBank.hpp
 *
 *  Created on: 2013/12/06
 */
#ifndef IDATABANK_HPP_
#define IDATABANK_HPP_

#include    "jointcoding.h"

namespace jc {

class IDataBank {
protected:
    IDataBank() {
    }
public:

    /**
     * バンクのIO用キー
     */
    typedef s64 bank_key;

    /**
     * Java :: jobjectのスタティックキャスト
     */
    typedef void* native_class_ref;

    virtual ~IDataBank() {
    }

    /**
     * データを追加する
     */
    virtual bank_key add(native_class_ref obj) = 0;

    /**
     * データを取得する
     *
     * NDK版はunsafeな状態で取り出すため、適宜jobjectへのキャストとAddGlobalRefが必要
     */
    virtual native_class_ref pop(const bank_key key) = 0;

    /**
     * 文字列化されたキーから取り出す
     *
     * NDK版はunsafeな状態で取り出すため、適宜jobjectへのキャストとAddGlobalRefが必要
     */
    virtual native_class_ref pop(const String &string_key) {
        return pop(string_key.toLong());
    }
};

/**
 * Nativeとの連携に必要なデータをIOする
 */
typedef jc_sp<IDataBank> MDataBank;

}

#endif /* IDATABANK_HPP_ */
