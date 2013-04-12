/*
 * TouchEvent.h
 *
 *  Created on: 2013/04/11
 */

#ifndef TOUCHEVENT_H_
#define TOUCHEVENT_H_

#include    "jc/widget/event/Event.h"
#include    "protocol/jcTouchEventProtocol.h"

namespace jc {
namespace view {

class TouchEventExtension: public Object {
    /**
     * Nativeから発行されたタッチイベント
     */
    jc_sp<TouchEventProtocol> event;
public:
    TouchEventExtension(const jc_sp<TouchEventProtocol> event) {
        this->event = event;
    }

    virtual ~TouchEventExtension() {
    }

    /**
     * プラットフォーム固有イベントを取得する
     */
    jc_sp<TouchEventProtocol> getPlatformEvent() const {
        return event;
    }

    /**
     * イベントを作成する
     */
    static MEvent createEvent( const jc_sp<TouchEventProtocol> event) {
        jc_sp<TouchEventExtension> extension(new TouchEventExtension(event));
        MEvent result(new Event(EventType_Touch, extension));
        return result;
    }
};

typedef jc_sp<TouchEventExtension> MTouchEventExtension;

}
}

#endif /* TOUCHEVENT_H_ */
