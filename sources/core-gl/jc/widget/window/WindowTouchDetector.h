/*
 * WindowTouchDetector.h
 *
 *  Created on: 2013/04/12
 */

#ifndef WINDOWTOUCHDETECTOR_H_
#define WINDOWTOUCHDETECTOR_H_

#include    "jc/widget/window/WindowContext.h"

namespace jc {
namespace view {

/**
 * Window内部のタッチ・コントロールを行う
 */
class WindowTouchDetector: public Object, public TouchListener {
    MWindowContext windowContext;

    /**
     * タッチ対象のViewを選択する
     */
    void selectTouchTargetView(const TouchPoint &point);

public:
    WindowTouchDetector(MWindowContext windowContext);
    virtual ~WindowTouchDetector() {

    }

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
};

/**
 * managed
 */
typedef jc_sp<WindowTouchDetector> MWindowTouchDetector;

}
}

#endif /* WINDOWTOUCHDETECTOR_H_ */
