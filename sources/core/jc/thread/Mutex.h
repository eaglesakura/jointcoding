/*
 * jcMutex.h
 *
 *  Created on: 2012/07/21
 */

#ifndef JCMUTEX_H_
#define JCMUTEX_H_

#include    "jc/mem/SmartPtr.h"

namespace   boost {
class recursive_mutex;
}
namespace jc {


class MutexLock;
/**
 * Mutexをもう1枚ラップして、boostの直接includeを行わないようにする。
 */
class Mutex {
    jc_sp<boost::recursive_mutex> mutex;

    /**
     * コピーコンストラクタは作らせない
     */
    Mutex(const Mutex &cp);

public:
    Mutex();
    ~Mutex();

    friend class MutexLock;
};

/**
 * mutexをリネーム
 */
typedef jc::Mutex jcmutex;

/**
 * syncronize用にロックオブジェクトをラップする。
 */
class MutexLock {
    /**
     * 解放が終わるまで、mutexは確保しておく
     */
    jc_sp<boost::recursive_mutex> mutex;

    /**
     * ロックオブジェクト
     */
    void* lock;

public:
    MutexLock(const Mutex &mtx);
    ~MutexLock();

private:
    /**
     * ミューテックスのロックを行う。
     * 実際にロックが行えるまで、このメソッドは返らない。
     */
    static jc_sp<MutexLock> reqeuestLock(Mutex &mtx) {
        return jc_sp < MutexLock > (mark_new MutexLock(mtx));
    }
};

/**
 * managed
 */
typedef jc_sp<MutexLock> MMutexLock;

}

#endif /* JCMUTEX_H_ */
