/*
 * Event.h
 *
 *  Created on: 2013/04/11
 */

#ifndef EVENT_H_
#define EVENT_H_

#include    "jointcoding.h"

namespace jc {
namespace view {

/**
 * イベントの種類を定義する
 */
enum EventType_e {
    /**
     * タッチイベントが発行された
     */
    EventType_Touch = 0x00010000,

    /**
     * キーイベントが発行された
     */
    EventType_Key,
};

/**
 * Viewに伝えるイベント情報
 */
class Event: Object {
    /**
     * 拡張アイテム
     */
    jc_sp<Object> extension;

    /**
     * イベントの種類
     */
    s32 type;
public:
    Event(const s32 EventType_type, jc_sp<Object> extension) {
        this->type = EventType_type;
        this->extension = extension;
    }

    virtual ~Event() {
    }

    /**
     * イベントの種類を取得する.
     * 標準イベント以外も設定できる
     */
    s32 getType() const {
        return type;
    }

    /**
     * タイプが適当するならtrueを返す
     */
    jcboolean isThisType( const EventType_e type ) const {
        return this->type == type;
    }

    /**
     * 拡張クラスを取得する
     */
    template<typename T>
    jc_sp<T> getExtension() const {
        return dynamic_pointer_cast<T>(extension);
    }
};

/**
 * イベントクラス
 */
typedef jc_sp<Event> MEvent;

}
}

#endif /* EVENT_H_ */
