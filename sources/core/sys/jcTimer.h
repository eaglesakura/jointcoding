/*
 * jcTimer.h
 *
 *  Created on: 2012/07/30
 */

#ifndef JCTIMER_H_
#define JCTIMER_H_

#include    "jcMacro.h"
#include    "boost/thread/thread_time.hpp"
#include    "boost/timer.hpp"

namespace jc {

/**
 * 時刻を扱うクラスを別名にする
 */
typedef boost::posix_time::ptime time;

class Timer {
public:
    /**
     * 現在時刻からミリ秒単位で送れた時刻を取得する
     */
    inline static time delayTime(const s32 milliSec) {
        return boost::get_system_time() + boost::posix_time::milliseconds(milliSec);
    }

    /**
     * 現在時刻を取得する
     */
    inline static time currentTime() {
        return boost::get_system_time();
    }

    /**
     * 時刻tからの経過時間をミリ秒単位で取得する
     */
    inline static s32 lapseTimeMs(const time &t) {
        const time now = currentTime();
        return (now - t).total_milliseconds();
    }
};
}

#endif /* JCTIMER_H_ */
