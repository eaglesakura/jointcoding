/*
 * EventQueue.h
 *
 *  Created on: 2013/04/11
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include    "jointcoding.h"
#include    "jc/widget/event/Event.h"
#include    <list>

namespace jc {
namespace view {

/**
 * イベントを待ち行列として管理する
 */
class EventQueue: public Object {
    /**
     * 複数スレッドアクセス用のmutex
     */
    jcmutex mutex;

    /**
     * 貯めこまれたイベント
     */
    std::list<MEvent> events;
public:
    EventQueue() {
    }

    virtual ~EventQueue() {
    }

    /**
     * イベントキューへ追加する
     */
    virtual void pushBackEvent(const MEvent event) {
        MutexLock lock(mutex);
        events.push_back(event);
    }

    /**
     * イベントがひとつも無くなったらtrueを返す
     */
    virtual jcboolean empty() {
        MutexLock lock(mutex);
        return events.empty();
    }

    /**
     * 一番上に積まれたイベントを返す
     */
    virtual MEvent popEvent() {
        MutexLock lock(mutex);

        if (events.empty()) {
            return MEvent();
        }

        MEvent result = events.front();
        events.erase(events.begin());

        return result;
    }

    /**
     * イベントを全て削除して空の状態にする
     */
    virtual void clear() {
        MutexLock lock(mutex);
        events.clear();
    }

    /**
     * 特定のイベントタイプのイベントを削除する
     */
    virtual void clear(const s32 eventType) {
        MutexLock lock(mutex);
        std::list<MEvent>::iterator itr = events.begin(), end = events.end();
        while (itr != end) {
            if ((*itr)->getType() == eventType) {
                // イベントタイプがヒットしたら削除する
                itr = events.erase(itr);
                end = events.end();
            } else {
                ++itr;
            }
        }
    }
};

/**
 * managed
 */
typedef jc_sp<EventQueue> MEventQueue;

}
}

#endif /* EVENTQUEUE_H_ */
