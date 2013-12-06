/*
 * jcThreadUtil.h
 *
 *  Created on: 2012/09/25
 */

#ifndef JCTHREADUTIL_H_
#define JCTHREADUTIL_H_

#include    "jc/thread/Thread.h"
#include    "jc/platform/Platform.h"

namespace jc {
class ThreadUtil {
public:
    /**
     * 非同期に関数を実行する。
     * 自動的にスレッドを生成し、スレッドは自動的に廃棄される。
     */
    template<typename T>
    static void asyncFunction(::jc::data_runnable_function func, jc_sp<T> data, String thread_name) {
        MRunnable runnable((Runnable*) mark_new DataRunnable<T>(func, data));
        Thread *thread = Platform::newThread(runnable)->autoRelease();
        thread->setName(thread_name);
        thread->start();
    }
};

}

#endif /* JCTHREADUTIL_H_ */
