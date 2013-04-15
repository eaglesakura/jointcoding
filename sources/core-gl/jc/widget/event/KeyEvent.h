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
public:
    KeyEventExtension(const jc_sp<KeyEventProtocol> event) {
        this->event = event;
    }

    virtual ~KeyEventExtension() {
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
        jc_sp<KeyEventExtension> extension(new KeyEventExtension(event));
        MEvent result(new Event(EventType_Key, extension));
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
