/*
 * WindowTouchDetector.cpp
 *
 *  Created on: 2013/04/12
 */

#include    "jc/widget/window/WindowTouchDetector.h"

namespace   jc {
namespace   view {

WindowTouchDetector::WindowTouchDetector(MWindowContext windowContext) {
    this->windowContext = windowContext;
}
/**
 * Viewがクリックされた。
 * 指が離された瞬間に認識される
 * onDragEnd()とどちらかが呼び出される。
 */
void WindowTouchDetector::onClick(const TouchDetector* detector, const TouchPoint &point) {

}

/*
 * ドラッグを終了させた。
 * onClick()とどちらかが呼び出される。
 */
void WindowTouchDetector::onDragEnd(const TouchDetector *detector, const TouchPoint &point) {

}

/**
 * タッチが開始された
 */
void WindowTouchDetector::onTouchBegin(const TouchDetector* detector, const TouchPoint &point) {

}

/**
 * Viewがドラッグされた
 */
void WindowTouchDetector::onDrag(const TouchDetector* detector, const TouchPoint &point) {

}

/**
 * マルチタッチが開始された
 */
void WindowTouchDetector::onPinchBegin(const TouchDetector *detector, const Vector2f &center) {

}

/**
 * ピンチが変更された
 */
void WindowTouchDetector::onPinchScaling(const TouchDetector *detector, const float scale, const Vector2f &center) {

}

/**
 * ピンチ操作が終了された
 */
void WindowTouchDetector::onPinchEnd(const TouchDetector *detector, const Vector2f &center) {

}

}
}

