/*
 * WindowTouchDetector.cpp
 *
 *  Created on: 2013/04/12
 */

#include    "jc/widget/window/WindowTouchDetector.h"

namespace   jc {
namespace   view {

#define log_point(msg, _point)   jclogf("%s v(%.1f, %.1f) id(%d)", msg, _point.getCurrentPosition().x, _point.getCurrentPosition().y, _point.getId())

WindowTouchDetector::WindowTouchDetector(MWindowContext windowContext) {
    this->windowContext = windowContext;
}
/**
 * Viewがクリックされた。
 * 指が離された瞬間に認識される
 * onDragEnd()とどちらかが呼び出される。
 */
void WindowTouchDetector::onClick(const TouchDetector* detector, const TouchPoint &point) {
    log_point("onClick", point);
}

/*
 * ドラッグを終了させた。
 * onClick()とどちらかが呼び出される。
 */
void WindowTouchDetector::onDragEnd(const TouchDetector *detector, const TouchPoint &point) {
    log_point("onDragEnd", point);
}

/**
 * タッチが開始された
 */
void WindowTouchDetector::onTouchBegin(const TouchDetector* detector, const TouchPoint &point) {
    log_point("onTouchBegin", point);
}

/**
 * タッチが終了された
 * onTouchBegin -> onClick -> onTouchEnd
 * onTouchBegin -> onDrag -> onDragEnd -> onTouchEnd
 */
void WindowTouchDetector::onSingleTouchEnd(const TouchDetector* detector, const TouchCompleteType_e type, const TouchPoint &point) {
    log_point("onTouchEnd", point);
}

/**
 * Viewがドラッグされた
 */
void WindowTouchDetector::onDrag(const TouchDetector* detector, const TouchPoint &point) {
    log_point("onDrag", point);
}

/**
 * マルチタッチが開始された
 */
void WindowTouchDetector::onPinchBegin(const TouchDetector *detector, const Vector2f &center) {
    jclog("onPinchBegin");
}

/**
 * ピンチが変更された
 */
void WindowTouchDetector::onPinchScaling(const TouchDetector *detector, const float scale, const Vector2f &center) {
    jclog("onPinchScaling");
}

/**
 * ピンチ操作が終了された
 */
void WindowTouchDetector::onPinchEnd(const TouchDetector *detector, const Vector2f &center) {
    jclog("onPinchEnd");
}

}
}

