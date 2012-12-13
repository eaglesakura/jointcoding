/*
 * jcRunnable.h
 *
 *  Created on: 2012/07/10

 */

#ifndef JCRUNNABLE_H_
#define JCRUNNABLE_H_

#include    "jcMacro.h"
#include    "jclog.h"
#include    "jcSmartPtr.h"

namespace jc {

/**
 * 実行用のプロトコル
 */
class Runnable {
public:
    virtual ~Runnable() {
    }

    /**
     * 実行を行う
     */
    virtual void run() = 0;
};

typedef jc_sp<Runnable> MRunnable;

/**
 * 非同期実行関数ポインタ
 */
typedef void (*data_runnable_function)(void* pointer);

/**
 * データと紐付いたRunnable
 */
template<typename T>
class DataRunnable: public Runnable {
    /**
     * 引数データ
     */
    jc_sp<T> data;

    /**
     * 実行関数
     */
    data_runnable_function function;
public:
    DataRunnable(data_runnable_function func, jc_sp<T> d) {
        this->data = d;
        this->function = func;
        jclogf("runnable new %x", this);
    }

    ~DataRunnable() {
        jclogf("runnable delete %x", this);
    }

    /**
     * 実行を行う
     */
    virtual void run() {
        function((void*)data.get());
    }
};


}

#endif /* JCRUNNABLE_H_ */
