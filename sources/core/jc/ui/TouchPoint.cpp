/*
 * TouchPoint.cpp
 *
 *  Created on: 2012/07/24

 */
#include    "jc/system/Macro.h"
#include    "jc/ui/TouchPoint.h"
#include    "jc/ui/TouchDetector.h"

namespace jc {

TouchPoint::TouchPoint(const s32 id, const Vector2f &beginPos) {
    this->id = id;
    this->beginPos = beginPos;
    this->currentPos = beginPos;
    this->touchTimeMs = 0;
    this->draged = jcfalse;
}

TouchPoint::~TouchPoint() {

}

/**
 * イベントが更新された
 */
void TouchPoint::onTouchEvent(TouchEventProtocol *event, const float dragDistance) {
    currentPos.x = event->getEventPosX();
    currentPos.y = event->getEventPosY();
    if (getMoveDistance() > dragDistance) {
        draged = jctrue;
    }

    if (event->getTouchID() == TouchEventProtocol::EVENT_TYPE_END) {
        // タッチされていた時間を設定
        touchTimeMs = getTouchTimeMs();
    }
}

}
