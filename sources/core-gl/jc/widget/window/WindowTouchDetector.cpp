/*
 * WindowTouchDetector.cpp
 *
 *  Created on: 2013/04/12
 */

#include    "jc/widget/window/WindowTouchDetector.h"
#include    "jc/widget/View.h"
#include    "jc/widget/window/Window.h"

namespace jc {
namespace view {

#define log_point(msg, _point)   jclogf("%s v(%.1f, %.1f) id(%d)", msg, _point.getCurrentPosition().x, _point.getCurrentPosition().y, _point.getId())

WindowTouchDetector::WindowTouchDetector(MWindowContext windowContext) {
    this->windowContext = windowContext;
}

/**
 * タッチ対象のViewを選択する
 */
void WindowTouchDetector::selectTouchTargetView(const TouchPoint &point) {
    MView target = windowContext->lockTouchTarget();
    MWindow window = windowContext->lockWindow();

    assert(window.get() != NULL);
    const Vector2f touchPos = point.getBeginPosition();

    if (!target) {
        // まだタッチが当たっていない場合はViewを取得する
        target = window->findTouchedView(touchPos);
    }

    if (!target) {
//        jclogf("target not found pos(%f, %f)", touchPos.x, touchPos.y);
        return;
    }
    windowContext->setTouchTarget(target);
#if 0
    {
        RectF area = target->getGlobalLayoutArea();
        jclogf("findTarget(%x) LTRB(%d, %d, %d, %d)", target.get(), (s32)area.left, (s32)area.top, (s32)area.right, (s32)area.bottom);
    }
#endif
}

/**
 * Viewがクリックされた。
 * 指が離された瞬間に認識される
 * onDragEnd()とどちらかが呼び出される。
 */
void WindowTouchDetector::onClick(const TouchDetector* detector, const TouchPoint &point) {
//    log_point("onClick", point);
}

/*
 * ドラッグを終了させた。
 * onClick()とどちらかが呼び出される。
 */
void WindowTouchDetector::onDragEnd(const TouchDetector *detector, const TouchPoint &point) {
//    log_point("onDragEnd", point);
}

/**
 * タッチが開始された
 */
void WindowTouchDetector::onTouchBegin(const TouchDetector* detector, const TouchPoint &point) {
//    log_point("onTouchBegin", point);
    selectTouchTargetView(point);
}

/**
 * タッチが終了された
 * onTouchBegin -> onClick -> onTouchEnd
 * onTouchBegin -> onDrag -> onDragEnd -> onTouchEnd
 */
void WindowTouchDetector::onSingleTouchEnd(const TouchDetector* detector, const TouchCompleteType_e type, const TouchPoint &point) {
    MView touchedView = windowContext->lockTouchTarget();
    if (!touchedView) {
        // 何も触ってなかった
        return;
    }
    //    log_point("onTouchEnd", point);

    // まだ触れられているかをチェックする
    const Vector2f touchPos = point.getCurrentPosition();

    // まだタップされている場合、クリック処理を行う
    if (touchedView->isIntersect(touchPos)) {
        jclogf("onClick(%x)", touchedView.get());
    } else {
        // クリック完了する前にViewから離された
        jclogf("onClick Out(%x)", touchedView.get());
    }

    // タッチターゲットを無効化しておく
    windowContext->clearTouchTarget();
}

/**
 * Viewがドラッグされた
 */
void WindowTouchDetector::onDrag(const TouchDetector* detector, const TouchPoint &point) {
//    log_point("onDrag", point);
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
//    jclog("onPinchScaling");
}

/**
 * ピンチ操作が終了された
 */
void WindowTouchDetector::onPinchEnd(const TouchDetector *detector, const Vector2f &center) {
    jclog("onPinchEnd");
    windowContext->clearTouchTarget();
}

}
}

