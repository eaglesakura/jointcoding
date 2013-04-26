/*
 * WindowTouchDetector.cpp
 *
 *  Created on: 2013/04/12
 */

#include    "jc/widget/window/WindowEventListener.h"
#include    "jc/widget/View.h"
#include    "jc/widget/window/Window.h"
#include    "jc/widget/event/DragEvent.h"

namespace jc {
namespace view {

#define log_point(msg, _point)   jclogf("%s v(%.1f, %.1f) id(%d)", msg, _point.getCurrentPosition().x, _point.getCurrentPosition().y, _point.getId())

WindowEventListener::WindowEventListener(MWindowContext windowContext) {
    this->windowContext = windowContext;
    lastHandleTime = Timer::currentTime();
}

/**
 * 最後にイベントハンドリングを行ってから何秒経過したかの値を取得する
 */
double WindowEventListener::getElapsedLastEventHandleSec() const {
    return Timer::lapseTimeSec(lastHandleTime, windowContext->systemTime());
}

/**
 * Viewがクリックされた。
 * 指が離された瞬間に認識される
 * onDragEnd()とどちらかが呼び出される。
 */
void WindowEventListener::onClick(const TouchDetector* detector, const TouchPoint &point) {
//    log_point("onClick", point);
    resetLastEventHandleTime();
}

/**
 * タッチが開始された
 */
void WindowEventListener::onTouchBegin(const TouchDetector* detector, const TouchPoint &point) {
    resetLastEventHandleTime();

    jcboolean refreshTouch = jcfalse; // タッチが更新されたらtrue
    MView target = windowContext->lockTouchTarget();
    MWindow window = windowContext->lockWindow();

    assert(window.get() != NULL);
    const Vector2f touchPos = point.getBeginPosition();

    if (!target) {
        // まだタッチが当たっていない場合はタッチを更新してViewを取得する
        target = window->findTouchedView(touchPos);
        refreshTouch = jctrue;
    }

    if (!target) {
        return;
    }

    // 現在位置=前の位置に上書きする
    beforeDragPosition = point.getBeginPosition();

    // タッチイベントをブロードキャストする
    if (refreshTouch) {
        // タッチ対象を更新する
        windowContext->setTouchTarget(target);

        // ダウンイベントを飛ばす
        target->dispatchDownEvent(jctrue);

        // タッチ開始イベントを飛ばす
        window->broadcastEvent(Event::createEvent(EventType_Touch, target));
    }
}

/**
 * タッチが終了された
 * onTouchBegin -> onClick -> onTouchEnd
 * onTouchBegin -> onDrag -> onDragEnd -> onTouchEnd
 */
void WindowEventListener::onSingleTouchEnd(const TouchDetector* detector, const TouchCompleteType_e type, const TouchPoint &point) {
    resetLastEventHandleTime();

    MView touchedView = windowContext->lockTouchTarget();
    MWindow window = windowContext->lockWindow();
    assert(window.get() != NULL);

    if (!touchedView) {
        // 何も触ってなかった
        return;
    }
    //    log_point("onTouchEnd", point);

    // まだ触れられているかをチェックする
    const Vector2f touchPos = point.getCurrentPosition();

    // ダウンイベントを送信する
    touchedView->dispatchDownEvent(jcfalse);

    // まだタップされている場合、クリック処理を行う
    if (touchedView->isIntersect(touchPos)) {
//        jclogf("onClick(%x)", touchedView.get());
        // クリックイベントをブロードキャストする
        if (type == TouchCompleteType_SingleClick) {
            window->broadcastEvent(Event::createEvent(BroadcastType_Click, touchedView));
        }
    } else {
        // クリック完了する前にViewから離された
//        jclogf("onClick Out(%x)", touchedView.get());
    }

    // タッチターゲットを無効化しておく
    windowContext->clearTouchTarget();
}

/**
 * Viewがドラッグされた
 */
void WindowEventListener::onDrag(const TouchDetector* detector, const TouchPoint &point) {
    resetLastEventHandleTime();
//    log_point("onDrag", point);
    MView touchedView = windowContext->lockTouchTarget();

    if (!touchedView) {
        return;
    }

    windowContext->sendEvent(DragEventExtension::createEvent(point, beforeDragPosition, touchedView));
    // 現在位置を保存
    beforeDragPosition = point.getCurrentPosition();
}

/*
 * ドラッグを終了させた。
 * onClick()とどちらかが呼び出される。
 */
void WindowEventListener::onDragEnd(const TouchDetector *detector, const TouchPoint &point) {
//    log_point("onDragEnd", point);
    MView touchedView = windowContext->lockTouchTarget();

    if (!touchedView) {
        return;
    }

    resetLastEventHandleTime();
    windowContext->sendEvent(DragEventExtension::createDragEndEvent(point, beforeDragPosition, touchedView));
}

/**
 * マルチタッチが開始された
 */
void WindowEventListener::onPinchBegin(const TouchDetector *detector, const Vector2f &center) {
    resetLastEventHandleTime();
    jclog("onPinchBegin");
}

/**
 * ピンチが変更された
 */
void WindowEventListener::onPinchScaling(const TouchDetector *detector, const float scale, const Vector2f &center) {
    resetLastEventHandleTime();
//    jclog("onPinchScaling");
}

/**
 * ピンチ操作が終了された
 */
void WindowEventListener::onPinchEnd(const TouchDetector *detector, const Vector2f &center) {
    jclog("onPinchEnd");
    resetLastEventHandleTime();
    windowContext->clearTouchTarget();
}

/**
 * キーダウンを開始した
 */
void WindowEventListener::onKeyDown(KeyDetector *detector, const MKeyData keyData) {
    jclogf("onKeyDown(%d)", keyData->getKeyCode());
    resetLastEventHandleTime();

    MWindow window = windowContext->lockWindow();
    assert(window.get() != NULL);

    // ブロードキャストする
    window->broadcastEvent(Event::createEvent(BroadcastType_Key, keyData));
}

/**
 * キーを長押した
 */
void WindowEventListener::onKeyLongDown(KeyDetector *detector, const MKeyData keyData) {
    jclogf("onKeyLongDown(%d)", keyData->getKeyCode());
    resetLastEventHandleTime();

    MWindow window = windowContext->lockWindow();
    assert(window.get() != NULL);

    // ブロードキャストする
    window->broadcastEvent(Event::createEvent(BroadcastType_Key, keyData));
}

/**
 * キーを離した
 */
void WindowEventListener::onKeyUp(KeyDetector *detector, const MKeyData keyData) {
    jclogf("onKeyUp(%d)", keyData->getKeyCode());
    resetLastEventHandleTime();

    MWindow window = windowContext->lockWindow();
    assert(window.get() != NULL);

    // ブロードキャストする
    window->broadcastEvent(Event::createEvent(BroadcastType_Key, keyData));

    // フォーカスがないならフォーカスを探してあげる
    {
        MView focusView = window->findFocusedView();
        if (!focusView) {
            // フォーカスが無ければ、適当なフォーカスを当てる
            focusView = window->findFocusableView();

            jclogf("focused view(%x)", focusView.get());

            // フォーカスターゲットを見つけたら、リクエストを投げる
            if (focusView) {
                focusView->requestFocus(jctrue);
            }
        } else {
            // フォーカスがあるなら、クリックイベントを送信してあげる
            if (keyData->isEnterKey()) {
                window->broadcastEvent(Event::createEvent(BroadcastType_Click, focusView));
            }
        }
    }

}

}
}

