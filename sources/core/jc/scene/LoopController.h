/*
 * LoopController.h
 *
 *  Created on: 2013/01/24
 */

#ifndef LOOPCONTROLLER_H_
#define LOOPCONTROLLER_H_

#include    "jointcoding.h"
#include    "jc/platform/Timer.h"

namespace jc {

class LoopController: public Object {
    /**
     * 前回のフレームからの経過時間（秒）
     */
    double elapsed_sec;

    /**
     * 前のフレームの終了時刻
     */
    jctime beforeFrameFinishTime;

    /**
     * 現在のフレームの終了時刻
     */
    jctime frameBeginTime;

    /**
     * フレームレート
     * default = 60fps
     */
    u32 frameRate;

    /**
     * １フレームにかけられる時間（ms単位）
     */
    u32 onceFrameTimeMs;

public:
    LoopController();

    virtual ~LoopController();

    /**
     * フレーム動作を開始する
     */
    virtual void beginFrame();

    /**
     * フレームレートを設定する。
     * default = 60fps
     */
    virtual void setFrameRate(const u32 rate) {
        frameRate = rate;
        onceFrameTimeMs = 1000 / rate;
    }

    /**
     * フレームレートを取得する
     */
    virtual u32 getFrameRate() const {
        return frameRate;
    }

    /**
     * １フレームの動作を終了し、経過時間を返す。
     *
     * @param result_frame_timems != NULLの場合、１フレームにかかった時間をms単位で返す
     * @param result_sleep_timems != NULLの場合、次のフレームまでにスリープすべき時間を返す
     */
    virtual void endFrame(u32 *result_frame_timems, u32 *result_sleep_timems);

    /**
     * 次のフレームの開始時刻までsleepを行う
     */
    virtual jcboolean sleepToNextFrame();

    /**
     * 秒単位のパラメータをフレーム単位のパラメータに変換する。
     *
     */
    template<typename T>
    T sec2frame(const T param_sec) {
        return (T) ((double) param_sec * elapsed_sec);
    }
};

/**
 * managed
 */
typedef jc_sp<LoopController> MLoopController;

}

#endif /* LOOPCONTROLLER_H_ */
