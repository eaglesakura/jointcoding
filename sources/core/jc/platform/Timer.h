/*
 * jcTimer.h
 *
 *  Created on: 2012/07/30
 */

#ifndef JCTIMER_H_
#define JCTIMER_H_

#include    "jc/system/Macro.h"
#include    "boost/thread/thread_time.hpp"
#include    "boost/timer.hpp"

namespace jc {

/**
 * 時刻を扱うクラスを別名にする
 */
typedef boost::posix_time::ptime jctime;

class Timer {
public:
    /**
     * 現在時刻からミリ秒単位で送れた時刻を取得する
     */
    inline static jctime delayTime(const s32 milliSec) {
        return boost::get_system_time() + boost::posix_time::milliseconds(milliSec);
    }

    /**
     * 指定時刻から指定ミリ秒ずらした時刻を取得する
     */
    inline static jctime delayTime(const jctime &time, const s32 milliSec) {
        return time + boost::posix_time::milliseconds(milliSec);
    }

    /**
     * 指定時刻から指定ミリ秒ずらした時刻を取得する
     */
    inline static jctime delayTime(const jctime &time, const double sec) {
        return time + boost::posix_time::milliseconds((s32)(sec * 1000.0));
    }

    /**
     * 現在時刻を取得する
     */
    inline static jctime currentTime() {
        return boost::get_system_time();
    }

    /**
     * 時刻tからの経過時間をミリ秒単位で取得する
     */
    inline static s32 lapseTimeMs(const jctime &t) {
        const jctime now = currentTime();
        return (now - t).total_milliseconds();
    }

    /**
     * 時刻tからの経過時間をミリ秒単位で取得する
     */
    inline static u64 lapseTimeMsLong(const jctime &t) {
        const jctime now = currentTime();
        return (now - t).total_milliseconds();
    }

    /**
     * 時刻tからの経過時間を秒単位で取得する
     */
    inline static double lapseTimeSec(const jctime &t) {
        return (double) lapseTimeMsLong(t) / 1000.0;
    }

    /**
     * beforeからafterの経過時間をミリ秒単位で取得する
     */
    inline static s32 lapseTimeMs(const jctime &before, const jctime &after) {
        return (after - before).total_milliseconds();
    }

    /**
     * beforeからafterの経過時間をミリ秒単位で取得する
     */
    inline static double lapseTimeSec(const jctime &before, const jctime &after) {
        return ((double)(after - before).total_milliseconds()) / 1000.0;
    }

};
}

#endif /* JCTIMER_H_ */
