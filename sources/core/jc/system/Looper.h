/*
 * jcLooper.h
 *
 *  Created on: 2012/07/29
 */

#ifndef JCLOOPER_H_
#define JCLOOPER_H_

#include    "jc/model/Object.h"
#include    "jc/mem/SmartPtr.h"
#include    "jc/thread/Runnable.h"
#include    "jc/thread/Mutex.h"
#include    "jc/thread/ThreadId.h"
#include    "jc/platform/Timer.h"
#include    <list>

namespace jc {

/**
 * Looperに登録する実行タスク。
 * 実行時刻と実行クラスのペアになっている。
 */
class LooperTask {
    /**
     * 実行クラス
     */
    MRunnable runnable;

    /**
     * ルーパーを実行させる時刻。
     * この時間を過ぎていない場合、実行させない
     */
    jctime runningTime;
public:
    /**
     * 開始時刻を指定して実行させる
     */
    LooperTask(MRunnable runnable, jctime running) {
        this->runnable = runnable;
        this->runningTime = running;
    }

    ~LooperTask() {

    }

    /**
     * 自分がotherより前に実行される場合はtrueを返す。
     */
    jcboolean isBefore(const LooperTask *other) const {
        return this->runningTime <= other->runningTime;
    }

    /**
     * 自分がotherより後に実行される場合はtrueを返す
     */
    jcboolean isAfter(const LooperTask *other) const {
        return this->runningTime >= other->runningTime;
    }

    /**
     * 実行時刻を過ぎていたらtrue
     */
    jcboolean isRuntime() const {
        jctime now = Timer::currentTime();
        return now >= runningTime;
    }

    /**
     * 実行を行う
     */
    void run() {
        runnable->run();
    }
};

/**
 * managed
 */
typedef jc_sp<LooperTask> MLooperTask;

/**
 * 特定のループ処理を行うクラス。
 * 基本的にpostのみを扱う。
 * 明示的に作成したThread内でのみ扱い、UIスレッドへのPOSTは別途扱う。
 */
class Looper: public Object {
    /**
     * スレッド操作用のMutex
     */
    jcmutex mutex;

    /**
     * 実行キャンセルを行う場合はtrue
     */
    jcboolean canceled;

    /**
     * 実行されているスレッドのID
     */
    MThreadID threadId;

    /**
     * 実行クラス一覧
     */
    std::list<MLooperTask> runners;

    /**
     * タスクが無い場合にsleepさせる時間
     */
    s32 sleepMs;

public:
    Looper();

    virtual ~Looper();

    /**
     * 実行がキャンセルされていたらtrue
     */
    virtual jcboolean isCanceled() const {
        return canceled;
    }

    /**
     * 実行キャンセルを行う
     * キャンセルは別のスレッドから呼び出すことができる。
     */
    virtual void cancel() {
        this->canceled = jctrue;
    }

    /**
     * キャンセル状態を解除して再実行できるようにする
     */
    virtual void reset() {
        this->canceled = jcfalse;
    }

    /**
     * ループを開始する。
     * ループはcancelされるまで続く。
     * 基本的にcancelされるまで実行が戻ることはない。
     * Looperを動作させたいスレッドから呼ぶこと。
     */
    virtual void execute();

    /**
     * 残タスクが残っている場合はtrueを返す。
     */
    virtual jcboolean hasTask();

    /**
     * 特定スレッドにPOSTする
     */
    virtual void post(const MRunnable runnable) {
        // すぐに実行させるランナーを作成する
        postAtDelay(runnable, 0);
    }

    /**
     * 指定したスレッドにPOSTする
     */
    virtual void postAtDelay(const MRunnable runnable, const s32 milliSec);

    /**
     * 実行中のスレッドIDを取得する
     */
    virtual MThreadID getThreadID() const {
        return threadId;
    }

    /**
     * スレッドの停止時間を指定する
     */
    virtual void setSleepTime(const s32 ms) {
        this->sleepMs = ms;
    }

protected:
    /**
     * タスクを実行する
     */
    virtual void onTaskRunning(MLooperTask data);

    /**
     * 実行すべきタスクがあれば引き出す。
     */
    virtual MLooperTask popTask();

    /**
     * 実行に不要な時間をスリープさせる。
     */
    virtual void onSleep() const;
};

}

#endif /* JCLOOPER_H_ */
