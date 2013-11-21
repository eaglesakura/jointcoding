/*
 * RequestFocusEvent.h
 *
 *  Created on: 2013/04/15
 */

#ifndef REQUESTFOCUSEVENT_H_
#define REQUESTFOCUSEVENT_H_

#include    "jc/widget/event/Event.h"

namespace jc {
namespace view {

class View;
class RequestFocusEventExtension: public Object {
public:
    /**
     * フォーカスを持つか、失うかの設定
     */
    jcboolean hasFocus;

    /**
     * 対象となるView
     */
    jc_sp<View> target;

    RequestFocusEventExtension(const jcboolean has, const jc_sp<View> target) {
        this->hasFocus = has;
        this->target = target;
    }

    static MEvent createInstance(const jcboolean hasFocus, const jc_sp<View> target) {
        return Event::createEvent(EventType_RequestFocus, jc_sp<Object>(mark_new RequestFocusEventExtension(hasFocus, target)) );
    }
};

typedef jc_sp<RequestFocusEventExtension> MRequestFocusEventExtension;

}
}

#endif /* REQUESTFOCUSEVENT_H_ */
