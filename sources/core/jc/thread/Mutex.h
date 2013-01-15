/*
 * jcMutex.h
 *
 *  Created on: 2012/07/21
 */

#ifndef JCMUTEX_H_
#define JCMUTEX_H_

#include    "jc/mem/SmartPtr.h"
#include    "jc/util/ImplCapsule.h"

namespace jc {

class MutexLock;
/**
 * Mutexをもう1枚ラップして、boostの直接includeを行わないようにする。
 */
class Mutex {
    ImplCapsule mutex;

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
     * 実装を隠蔽する
     */
    typedef void* NativeMutexLockPtr;

    /**
     * 解放が終わるまで、mutexは確保しておく
     */
    ImplCapsule mutex;

    /**
     * ロックオブジェクト
     */
    NativeMutexLockPtr nativeLock;

public:
    MutexLock(Mutex &mtx);
    ~MutexLock();

    /**
     * ミューテックスのロックを行う。
     * 実際にロックが行えるまで、このメソッドは返らない。
     */
    static jc_sp<MutexLock> lock(Mutex &mtx) {
        return jc_sp < MutexLock > (new MutexLock(mtx));
    }
};

/**
 * managed
 */
typedef jc_sp<MutexLock> MMutexLock;

}

#endif /* JCMUTEX_H_ */
