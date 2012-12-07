/*
 * Looper.cpp
 *
 *  Created on: 2012/07/30
 */

#include    "jcLooper.h"
#include    "jcThread.h"

namespace jc {

Looper::Looper() {
    sleepMs = 1;
    canceled = jcfalse;
}

Looper::~Looper() {

}
/**
 * ループを開始する。
 * ループはcancelされるまで続く。
 * 基本的にcancelされるまで実行が戻ることはない。
 */
void Looper::execute() {
    this->threadId.reset(new ThreadID());
    MLooperTask task;
    while (!isCanceled()) {
        if (hasTask()) {
            // タスクが残っているなら処理を行う
            while ((task = popTask()).get()) {
                task->run(); // 実行する
            }
        }
        onSleep();
    }
}

/**
 * 残タスクが残っている場合はtrueを返す。
 */
jcboolean Looper::hasTask() {
    MutexLock lock(mutex);

    return this->runners.empty() ? jcfalse : jctrue;
}

/**
 * 指定したスレッドにPOSTする
 */
void Looper::postAtDelay(const MRunnable runnable, const s32 milliSec) {
    MutexLock lock(mutex);

    MLooperTask ist(new LooperTask(runnable, Timer::delayTime(milliSec)));

    std::list<MLooperTask>::iterator itr = runners.begin(), end = runners.end();

    while (itr != end) {
        MLooperTask next = (*itr);
        if (next->isAfter(ist.get())) {
            runners.insert(itr, ist);
            return;
        }
        ++itr;
    }

    // 挿入できないからラストへpush
    runners.push_back(ist);
}

/**
 * タスクを実行する
 */
void Looper::onTaskRunning(MLooperTask data) {
    data->run();
}

/**
 * 実行すべきタスクがあれば引き出す。
 */
MLooperTask Looper::popTask() {
    MutexLock lock(mutex);

    // タスクが空
    if (runners.empty() || isCanceled()) {
        return MLooperTask();
    }

    // 基準となるべきは先頭データ
    MLooperTask task = runners.front();

    if (task->isRuntime()) {
        // 実行時刻を過ぎているから、先頭要素を消してデータを返す
        runners.pop_front();
        return task;
    } else {
        // 実行時刻に達していないから、何も返さない
        return MLooperTask();
    }
}

/**
 * 実行に不要な時間をスリープさせる。
 */
void Looper::onSleep() const {
    Thread::sleep(sleepMs);
}

}

