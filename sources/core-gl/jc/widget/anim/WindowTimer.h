/*
 * WindowTimer.h
 *
 *  Created on: 2013/04/18
 */

#ifndef WINDOWTIMER_H_
#define WINDOWTIMER_H_

#include    "jc/widget/window/WindowContext.h"

namespace jc {
namespace view {

/**
 * 時限イベントを設定する。
 * 実時刻ではなく、WindowContextが持つシステムタイマーによって動作する。
 * 処理落ちによるウィンドウ時間と実時間の誤差に対応する。
 */
class WindowTimer {
    /**
     *
     */
    MWindowContext windowContext;

    /**
     * 開始時刻
     */
    jctime startTime;

    /**
     * 規定するミリ秒
     */
    s32 timeMilliSeconds;
public:
    WindowTimer() {
        startTime = Timer::currentTime();
        timeMilliSeconds = 1;
    }

    ~WindowTimer() {
    }

    /**
     * 時刻を指定して設定する
     */
    void initialize(MWindowContext windowContext, const u32 timeMilliSeconds) {
        this->windowContext = windowContext;
        this->timeMilliSeconds = timeMilliSeconds;
        reset();
    }

    /**
     * 開始時刻をリセットする
     */
    void reset() {
        startTime = windowContext->systemTime();
    }

    /**
     * 時限設定をミリ秒単位で行う
     */
    void setTimerMS(const u32 timeMilliSeconds) {
        this->timeMilliSeconds = timeMilliSeconds;
    }

    /**
     * 時限設定をミリ秒単位で行う
     */
    void setTimerSec(const double timeSec) {
        this->timeMilliSeconds = (u32) (timeSec * 1000.0);
    }

    /**
     * 経過時刻を取得する
     */
    u32 elapseMs() const {
        return Timer::lapseTimeMs(startTime, windowContext->systemTime());
    }

    /**
     * 経過時刻を取得する
     */
    double elapseSec() const {
        return Timer::lapseTimeSec(startTime, windowContext->systemTime());
    }

    /**
     * 制限時間をオーバーした場合true
     */
    jcboolean over() const {
        assert(windowContext.get() != NULL);

        // 0以下の場合、常にfalseを返す。
        if (timeMilliSeconds <= 0) {
            return jcfalse;
        }

        return Timer::lapseTimeMs(startTime, windowContext->systemTime()) >= timeMilliSeconds;
    }

    /**
     * 時間をオーバーしていたらリセットを行い、trueを返す。
     * 定期イベントのチェックに利用する
     */
    jcboolean overWithReset() {
        if (over()) {
            reset();
            return jctrue;
        } else {
            return jcfalse;
        }
    }
};

/**
 * managed
 */
typedef jc_sp<WindowTimer> MWindowTimer;

}
}

#endif /* WINDOWTIMER_H_ */
