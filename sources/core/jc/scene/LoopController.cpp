/*
 * LoopController.cpp
 *
 *  Created on: 2013/01/24
 */

#include    "jc/scene/LoopController.h"
#include    "jc/math/Math.h"

namespace jc {

LoopController::LoopController() {
    beforeFrameFinishTime = Timer::currentTime();
    frameBeginTime = Timer::currentTime();
    frameRate = 60;
    onceFrameTimeMs = 1000 / frameRate;
    elapsed_sec = (double)onceFrameTimeMs/ 1000.0;
}

LoopController::~LoopController() {

}

/**
 * フレーム動作を開始する
 */
void LoopController::beginFrame() {
    frameBeginTime = Timer::currentTime();
    const double elapsed_ms = (double) Timer::lapseTimeMs(beforeFrameFinishTime, frameBeginTime);
    // 経過時間を秒単位に変換する
    elapsed_sec = elapsed_ms / 1000.0f;
}

/**
 * １フレームの動作を終了し、経過時間を返す。
 *
 * @param result_frame_timems != NULLの場合、１フレームにかかった時間をms単位で返す
 * @param result_sleep_timems != NULLの場合、次のフレームまでにスリープすべき時間を返す
 */
void LoopController::endFrame(u32 *result_frame_timems, u32 *result_sleep_timems) {
    beforeFrameFinishTime = Timer::currentTime();

    // 1フレームの所要時間を計算する
    const u32 frame_time = Timer::lapseTimeMs(frameBeginTime, beforeFrameFinishTime);
    if (result_frame_timems) {
        *result_frame_timems = frame_time;
    }
    if (result_sleep_timems) {
        *result_sleep_timems = max<u32>(0, onceFrameTimeMs - frame_time);
    }
}

/**
 * 次のフレームの開始時刻までsleepを行う
 */
jcboolean LoopController::sleepToNextFrame() {
    return jcfalse;
}
}
