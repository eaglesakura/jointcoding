/*
 * jcNativeThread.h
 *
 *  Created on: 2012/07/22

 */

#ifndef JCNATIVETHREAD_H_
#define JCNATIVETHREAD_H_

#include    "jcMacro.h"
#include    "jcSmartPtr.h"
#include    "jcObject.h"
#include    "jcRunnable.h"
#include    "jcThreadId.h"
#include    "jcMutex.h"

namespace jc {

/**
 * スレッドの抽象クラス。
 * 実装についてはサブクラスであるBoostThreadかJavaThreadに任せる。
 * ただし、BoostThreadについてはJavaのメソッドが呼べない場合があるため、確実な実装クラスを得たい場合は
 * Application#newThread()を呼び出すこと。
 * #start()メソッドが呼ばれるまで、実際のスレッド作成は行われない。
 * # ネイティブスレッド作成
 * # runnable->run()の呼び出し
 * # autorelease()が必要なら自動でプールから削除
 */
class Thread: public Object {

    /**
     * スレッド名
     */
    String name;

protected:

    /**
     * 生成されるスレッドID。
     * スレッドがstart()されるまではnullが設定されている
     */
    MThreadID threadId;

    /**
     * 排他処理
     */
    jcmutex mtx;

    /**
     * 実行クラス
     */
    MRunnable runnable;

    /**
     * プールに登録する。
     */
    static void addAutoreleasePool(Thread *thread);

    /**
     * プールから削除する。
     */
    static void releasePool(Thread *thread);

    /**
     * コピー禁止
     */
    Thread(const Thread& cpy);
public:
    Thread(const MRunnable runnable) {
        this->runnable = runnable;
        this->name = "native_thread";
    }

    virtual ~Thread() {
    }

    /**
     * スレッド名を指定する
     */
    virtual void setName(const String &name) {
        this->name = name;
    }

    /**
     * スレッド名を取得する
     */
    virtual const String& getName() const {
        return name;
    }

    /**
     * 自動削除プールに追加する。
     * このメソッドを呼んだ場合、join()を行うことはできない。
     */
    virtual Thread* autoRelease();

    /**
     * スレッドID管理
     */
    virtual MThreadID getThreadID() {
        return threadId;
    }

    /**
     * 指定したミリ秒だけスレッドを停止させる。
     */
    static void sleep(s32 ms);

    // 実装の必要があるメソッド
public:
    /**
     * スレッドの開始を行う
     */
    virtual void start() = 0;

    /**
     * スレッドの停止待ちを行う。
     */
    virtual void join() = 0;

};

/**
 * managed
 */
typedef jc_sp<Thread> MNativeThread;

}

#endif /* JCNATIVETHREAD_H_ */
