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
    elapsed_sec = (double) onceFrameTimeMs / 1000.0;
    setFrameRateRange(15, 60);
}

LoopController::~LoopController() {

}

/**
 * フレーム動作を開始する
 */
void LoopController::beginFrame() {
    // 経過時間を秒単位に変換する
    // 120分の１秒未満、もしくは15分の1秒以上の超過は不自然な値とみなしてシャットアウトする
    elapsed_sec = Timer::lapseTimeSec(frameBeginTime);
    elapsed_sec = jc::minmax<double>(1.0 / (double)(frameRateRange.maxRate), 1.0 / (double)frameRateRange.minRate, elapsed_sec);
    frameBeginTime = Timer::currentTime();
}

/**
 * １フレームの動作を終了し、経過時間を返す。
 *
 * @param result_frame_timems != NULLの場合、１フレームにかかった時間をms単位で返す
 * @param result_sleep_timems != NULLの場合、次のフレームまでにスリープすべき時間を返す
 */
void LoopController::endFrame(u32 *result_frame_timems, u32 *result_sleep_timems) {
    // 現在時刻を打刻する
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
 * 可変フレームレートの範囲を設定する。
 * minRateの設定値よりもフレームレートが下がった場合、強制的にminRateとして扱う
 * maxRateの設定値よりもフレームレートが上がった場合、強制的にmaxRateとして扱う
 */
void LoopController::setFrameRateRange(const u32 minRate, const u32 maxRate) {
    frameRateRange.minRate = jc::min<u32>(minRate, maxRate);
    frameRateRange.maxRate = jc::max<u32>(minRate, maxRate);
}



/**
 * 次のフレームの開始時刻までsleepを行う
 */
jcboolean LoopController::sleepToNextFrame() {
    return jcfalse;
}
}
