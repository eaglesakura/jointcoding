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

    /**
     * Viewにフォーカスを当てるようにリクエストする
     */
    EventType_RequestFocus,

    /**
     * WindowManagerから定期的に呼び出されるコールバック命令
     */
    EventType_Tick,
};

/**
 * 各Viewにブロードキャストされるイベント
 */
enum BroadcastType_e {
    /**
     * クリックイベントが行われた
     */
    BroadcastType_Click = 0x00100000,

    /**
     * ドラッグが行われた
     */
    BroadcastType_Drag,

    /**
     * ドラッグが終了した
     */
    BroadcastType_DragEnd,

    /**
     * キーイベントが行われた
     */
    BroadcastType_Key,
};

/**
 * Viewに伝えるイベント情報
 */
class Event: public Object {
    /**
     * 拡張アイテム
     */
    jc_sp<Object> extension;

    /**
     * イベントの種類
     */
    s32 type;

    /**
     * 全Viewにブロードキャストを行う場合true
     */
    jcboolean broadcast;
public:
    Event(const s32 EventType_type, jc_sp<Object> extension) {
        this->type = EventType_type;
        this->extension = extension;
        this->broadcast = jcfalse;
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
     * ブロードキャスト属性が付与されている場合true
     */
    jcboolean isBroadcast() const {
        return broadcast;
    }

    /**
     * ブロードキャスト属性を設定する
     */
    void setBroadcast(const jcboolean set) {
        broadcast = set;
    }

    /**
     * 拡張クラスを取得する
     */
    template<typename T>
    jc_sp<T> getExtension() const {
        return downcast<T>(extension);
    }

    /**
     * イベントを作成する
     */
    static jc_sp<Event> createEvent( const s32 type, const jc_sp<Object> extension = jc_sp<Object>() ) {
        return jc_sp<Event>(mark_new Event(type, extension));
    }

    /**
     * ブロードキャストを作成する
     */
    static jc_sp<Event> createBroadcast( const s32 type, const jc_sp<Object> extension = jc_sp<Object>() ) {
        jc_sp<Event> result = jc_sp<Event>(mark_new Event(type, extension));
        result->broadcast = jctrue;
        return result;
    }
};

/**
 * イベントクラス
 */
typedef jc_sp<Event> MEvent;

}
}

#endif /* EVENT_H_ */
