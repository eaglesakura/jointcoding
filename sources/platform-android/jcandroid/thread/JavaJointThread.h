/*
 * ndkJavaThread.h
 *
 *  Created on: 2012/07/22

 */

#ifndef NDKJAVATHREAD_H_
#define NDKJAVATHREAD_H_

#include    "jointcoding-android.h"
#include    "android-classes/ndkJavaNativeThread.h"
#include    "jc/thread/Thread.h"

namespace ndk {
class JavaJointThread: public Thread {
    /**
     * Java側のスレッド
     */
    jc_sp<JavaNativeThread> nativeThread;

public:
    JavaJointThread(MRunnable runnable);
    virtual ~JavaJointThread();

    /**
     * スレッド名を指定する
     */
    virtual void setName(String name);

    /**
     * スレッドを開始する
     */
    virtual void start();

    /**
     *
     */
    virtual void join();

    /**
     * 実行クラスを取得する
     */
    virtual MRunnable getRunnable() {
        return runnable;
    }

    /**
     * スレッドを削除する
     */
    static void removeAutorelease( JavaJointThread *thread ) {
        Thread::releasePool(thread);
    }
};

}

#endif /* NDKJAVATHREAD_H_ */
