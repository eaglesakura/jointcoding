/*
 * jcThread.cpp
 *
 *  Created on: 2012/07/11

 */

#include "jointcoding.h"
#include "jc/thread/Runnable.h"
#include "jc/thread/BoostThread.h"
// boost library
#include "boost/thread.hpp"

#ifdef  BUILD_Android
#include "jointcoding-android.h"
#endif

using namespace boost;

namespace jc {

BoostThread::BoostThread(MRunnable runnable) :
        Thread(runnable) {
    pThread = NULL;
}

BoostThread::~BoostThread() {
    jclogf("delete thread!! %x", this);
    if (pThread) {
        thread *p = ((thread*) pThread);
        p->join();
        SAFE_DELETE(p);
        pThread = NULL;
    }
}

/**
 * 実際のスレッド実行を行う
 */
void BoostThread::runThread(BoostThread *_this) {
    if (_this) {
        // ミューテックスが取得できるまで待つ
        // スレッドの起動が早すぎる場合、newの結果が_this->threadに格納されていない場合があるため。
        {
            MutexLock lock(_this->mtx);
        }

#ifdef  BUILD_Android
        JavaVM *javaVM = ndk::NDK::getJavaVM();
        JNIEnv *env = NULL;

        // VMにアタッチする
        {
            JavaVMAttachArgs args;
            args.group = NULL;
            args.name = _this->getName().c_str();
            args.version = JNI_VERSION_1_6;
            javaVM->AttachCurrentThread(&env, (void*) &args);
        }
#endif
        // IDを発行する
        thread::id _id = ((thread*) _this->pThread)->get_id();
        _this->threadId.reset(new ThreadID(&_id));

        // 本体を実行する
        {
            _this->runnable->run();
        }

#ifdef  BUILD_Android
        // VMからデタッチする
        javaVM->DetachCurrentThread();
#endif
        // スレッドを削除する
        {
            MutexLock lock(_this->mtx);
            {
                // スレッドを削除する
                IMPL_SAFE_DELETE(_this->pThread, boost::thread);
            }
        }
        // 自動開放プールからスレッドを削除する
        Thread::releasePool(_this);
    }
}

/**
 * スレッドの実行を開始する
 */
void BoostThread::start() {
    MutexLock lock(this->mtx);
    if (!pThread) {
        pThread = (NativeThreadPtr) new boost::thread(boost::bind(runThread, this));
    }
}

/**
 * スレッドの終了を待つ
 */
void BoostThread::join() {
    if (pThread) {
        ((thread*) pThread)->join();
    }
}

} /* namespace jc */
