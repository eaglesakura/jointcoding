/*
 * KeyEvent.h
 *
 *  Created on: 2013/04/15
 */

#ifndef KEYEVENT_H_
#define KEYEVENT_H_

#include    "jc/widget/event/Event.h"
#include    "protocol/jcKeyEventProtocol.h"

namespace jc {
namespace view {

class KeyEventExtension: public Object {
    /**
     * Nativeから発行されたタッチイベント
     */
    jc_sp<KeyEventProtocol> event;

    class KeyEventPool: public Object, public KeyEventProtocol {
        s32 eventType;
        s32 keyCode;
    public:
        KeyEventPool(KeyEventProtocol *protocol) {
            this->eventType = protocol->getEventType();
            this->keyCode = protocol->getKeyCode();

            jcmark(this);
        }

        virtual ~KeyEventPool() {
            jcunmark(this);
        }

        virtual s32 getEventType() {
            return eventType;
        }
        virtual s32 getKeyCode() {
            return keyCode;
        }
    };
public:
    KeyEventExtension(const jc_sp<KeyEventProtocol> event) {
//        this->event = event;
        this->event.reset(mark_new KeyEventPool(event.get()));

        jcmark(this);
    }

    virtual ~KeyEventExtension() {
        jcunmark(this);
    }

    /**
     * プラットフォーム固有イベントを取得する
     */
    jc_sp<KeyEventProtocol> getPlatformEvent() const {
        return event;
    }

    /**
     * イベントを作成する
     */
    static MEvent createEvent( const jc_sp<KeyEventProtocol> event) {
        jc_sp<KeyEventExtension> extension(mark_new KeyEventExtension(event));
        MEvent result(mark_new Event(EventType_Key, extension));
        return result;
    }
};

/**
 * managed
 */
typedef jc_sp<KeyEventExtension> MKeyEventExtension;

}
}

#endif /* KEYEVENT_H_ */
