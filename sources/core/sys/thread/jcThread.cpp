/*
 * NativeThread.cpp
 *
 *  Created on: 2012/07/22
 */

#include    "jcThread.h"
#include    "jcMath.h"
#include    "jcTimer.h"

#include    "prv_jcThread.h"

namespace jc {

namespace {

/**
 * スレッドプール用のミューテックス
 */
static jcmutex g_threadMutex;

/**
 * ネイティブスレッドプール
 */
static std::list<MNativeThread> g_threads;

}

/**
 * プールに登録する。
 */
void Thread::addAutoreleasePool(Thread *thread) {
    MutexLock lock(g_threadMutex);
    {
        g_threads.push_back(MNativeThread(thread));
    }
}

/**
 * プールから削除する。
 */
void Thread::releasePool(Thread *thread) {
    MutexLock lock(g_threadMutex);

    {
        std::list<MNativeThread>::iterator itr = g_threads.begin(), end = g_threads.end();
        while (itr != end) {
            if (itr->get() == thread) {
                g_threads.erase(itr);
                return;
            }
            ++itr;
        }
    }

}

/**
 * 自動削除セットに追加する。
 */
Thread* Thread::autoRelease() {
    addAutoreleasePool(this);
    return this;
}

namespace {
/**
 * sleepを呼び出すために必要な時間。
 */
static s32 g_sleep_booting_ms = 0;
}

/**
 * スレッドを一定時間停止させる
 */
void Thread::sleep(s32 ms) {
    // スリープ時刻を設定
    const s32 sleepTime = jc::max(1, ms - g_sleep_booting_ms);

    // 開始時刻を記録
    boost::timer sleep_timer;

    // 休止を行う
    boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(sleepTime));

    // 実際に休止させた時間との差分を設定する
    g_sleep_booting_ms = (s32) (sleep_timer.elapsed() * 1000) - sleepTime;

//    jclogf("g_sleep_booting_ms = %d", g_sleep_booting_ms);
//    boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(ms));
}

}
