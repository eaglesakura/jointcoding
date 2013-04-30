/*
 * WindowTouchDetector.h
 *
 *  Created on: 2013/04/12
 */

#ifndef GLVIEW_WINDOWTOUCHDETECTOR_H_
#define GLVIEW_WINDOWTOUCHDETECTOR_H_

#include    "jc/widget/window/WindowContext.h"

namespace jc {
namespace view {

/**
 * Window内部のタッチ・コントロールを行う
 */
class WindowEventListener: public Object, public TouchListener, public KeyListener {
    /**
     *
     */
    MWindowContext windowContext;

    /**
     * 最後にイベントハンドリングを行った時刻
     */
    jctime lastHandleTime;

    /**
     * 前のフレームでのドラッグ位置
     */
    Vector2f beforeDragPosition;
public:
    WindowEventListener(MWindowContext windowContext);
    virtual ~WindowEventListener() {
    }

    /**
     * 最後にイベントハンドリングを行った時刻を取得する
     */
    virtual jctime getLastEventHandleTime() const {
        return lastHandleTime;
    }

    /**
     * 最後にハンドリングした時刻をリセットする
     */
    virtual void resetLastEventHandleTime() {
        lastHandleTime = windowContext->systemTime();
    }

    /**
     * 最後にイベントハンドリングを行ってから何秒経過したかの値を取得する
     */
    virtual double getElapsedLastEventHandleSec() const;

    /**
     * Viewがクリックされた。
     * 指が離された瞬間に認識される
     * onDragEnd()とどちらかが呼び出される。
     */
    virtual void onClick(const TouchDetector* detector, const TouchPoint &point);
    /**
     * ドラッグを終了させた。
     * onClick()とどちらかが呼び出される。
     */
    virtual void onDragEnd(const TouchDetector *detector, const TouchPoint &point);

    /**
     * タッチが開始された
     */
    virtual void onTouchBegin(const TouchDetector* detector, const TouchPoint &point);

    /**
     * タッチが終了された
     * onTouchBegin -> onClick -> onTouchEnd
     * onTouchBegin -> onDrag -> onDragEnd -> onTouchEnd
     */
    virtual void onSingleTouchEnd(const TouchDetector* detector, const TouchCompleteType_e type, const TouchPoint &point);

    /**
     * Viewがドラッグされた
     */
    virtual void onDrag(const TouchDetector* detector, const TouchPoint &point);

    /**
     * マルチタッチが開始された
     */
    virtual void onPinchBegin(const TouchDetector *detector, const Vector2f &center);

    /**
     * ピンチが変更された
     */
    virtual void onPinchScaling(const TouchDetector *detector, const float scale, const Vector2f &center);

    /**
     * ピンチ操作が終了された
     */
    virtual void onPinchEnd(const TouchDetector *detector, const Vector2f &center);

    /**
     * キーダウンを開始した
     */
    virtual void onKeyDown(KeyDetector *detector, const MKeyData keyData);

    /**
     * キーを長押した
     */
    virtual void onKeyLongDown(KeyDetector *detector, const MKeyData keyData);

    /**
     * キー押しっぱなしの状態が継続された。
     */
    virtual void onKeyKeeping(KeyDetector *detector, const MKeyData keyData);

    /**
     * キーを離した
     */
    virtual void onKeyUp(KeyDetector *detector, const MKeyData keyData);

};

/**
 * managed
 */
typedef jc_sp<WindowEventListener> MWindowEventListener;

}
}

#endif /* WINDOWTOUCHDETECTOR_H_ */
