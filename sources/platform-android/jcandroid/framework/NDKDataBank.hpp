/*
 * NDKDataBank.hpp
 *
 *  Created on: 2013/12/06
 */
#ifndef NDKDATABANK_HPP_
#define NDKDATABANK_HPP_

#include    "jointcoding-android.h"
#include    "jc/framework/app/IDataBank.hpp"
#include    "android-classes/ndkDataBank.h"

namespace ndk {

using namespace jc;

class NDKDataBank: public Object, public IDataBank {
    jc_sp<DataBank> bank;
public:
    NDKDataBank(jc_sp<DataBank> bank) {
        this->bank = bank;
    }

    virtual ~NDKDataBank() {
    }

    /**
     * データを追加する
     */
    virtual bank_key add(native_class_ref obj) {
        return bank->add( (jobject)obj );
    }

    /**
     * データを取得する
     */
    virtual native_class_ref pop(const bank_key key) {
        return (native_class_ref)bank->pop_unsafe(key);
    }
};

}

#endif /* NDKDATABANK_HPP_ */
