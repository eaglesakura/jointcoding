/*
 * Thread.h
 *
 *  Created on: 2012/07/10

 */

#ifndef JCTHREAD_H_
#define JCTHREAD_H_

#include    "jc/thread/Thread.h"

namespace jc {

/**
 * スレッドをBoost.Threadで構築する。
 * 内部実装は基本的にboostの実装に任せる。
 * 自動変数では作らないこと。
 * NDKの場合、自動的にJavaVMにアタッチ / デタッチされる。
 * スレッドオブジェクトの寿命を明示的に管理しない場合、autoRelease()を利用することで自動的にスレッドオブジェクトの開放を行う。
 * ネイティブAPIの呼び出しの可否は保証されないため、このクラスを直接利用することは非推奨
 */
class BoostThread: public Thread {

    /**
     * 実装を隠蔽
     */
    typedef void* NativeThreadPtr;

    /**
     * 実行対象のスレッド
     */
    NativeThreadPtr pThread;

    /**
     * コピーコンストラクタ不可
     */
    BoostThread(const BoostThread &origin);
protected:
    /**
     * 実際のスレッド実行を行う
     */
    static void runThread(BoostThread *pThread);
public:
    BoostThread(MRunnable runnable);
    virtual ~BoostThread();

    /**
     * スレッドの実行を開始する
     */
    void start();

    /**
     * スレッドの終了を待つ
     */
    void join();
};

} /* namespace jc */
#endif /* THREAD_H_ */
