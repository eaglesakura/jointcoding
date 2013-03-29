/*
 * FrameCounter.h
 *
 *  Created on: 2013/03/29
 */

#ifndef FRAMECOUNTER_H_
#define FRAMECOUNTER_H_

#include    "jc/platform/Timer.h"

namespace jc {

class FrameCounter {
    /**
     * 最後に更新した時刻
     */
    jctime updated_time;

    /**
     * フレームレートの実数
     */
    s32 real_rate;

    /**
     * 現在の値
     */
    s32 current_frames;
public:
    FrameCounter() {
        updated_time = Timer::currentTime();
        real_rate = 0;
        current_frames = 0;
    }

    ~FrameCounter() {
    }

    /**
     * フレームレートの更新を行う
     */
    void update() {
        ++current_frames;

        // 経過時間が1000msを超えたら、フレームレートを記録する
        if (Timer::lapseTimeMs(updated_time) >= 1000) {
            real_rate = current_frames;
            current_frames = 0;
            updated_time = Timer::currentTime();
        }
    }

    /**
     * フレームレートの実数を取得する
     */
    s32 getRealFramerate() const {
        return real_rate;
    }
};

}

#endif /* FRAMECOUNTER_H_ */
