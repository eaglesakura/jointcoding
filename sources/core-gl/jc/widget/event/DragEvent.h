/*
 * DragEvent.h
 *
 *  Created on: 2013/04/26
 */

#ifndef DRAGEVENT_H_
#define DRAGEVENT_H_

#include    "jc/widget/event/Event.h"
#include    "protocol/jcTouchEventProtocol.h"
#include    "jc/ui/TouchDetector.h"

namespace jc {
namespace view {

/**
 * ドラッグが行われた
 */
class DragEventExtension: public Object {
    /**
     * 対象のView
     */
    jc_sp<View> view;

    /**
     * ドラッグを始めた位置
     */
    Vector2f beginPosition;

    /**
     * 前に呼び出された時の場所
     */
    Vector2f beforePosition;

    /**
     * 現在の位置
     */
    Vector2f currentPosition;
public:
    DragEventExtension(const TouchPoint &point, const Vector2f &beforePos, const jc_sp<View> view) {
        this->beginPosition = point.getBeginPosition();
        this->currentPosition = point.getCurrentPosition();
        this->beforePosition = beforePos;
        this->view = view;
    }

    virtual ~DragEventExtension() {
    }

    /**
     * ドラッグを開始した位置を取得する
     */
    virtual Vector2f getBeginPosition() const {
        return beginPosition;
    }

    /**
     * ドラッグの現在位置を取得する
     */
    virtual Vector2f getCurrentPosition() const {
        return currentPosition;
    }

    /**
     * ドラッグの移動量を取得する
     */
    virtual Vector2f getQuantity() const {
        return currentPosition - beforePosition;
    }

    /**
     * ドラッグ中イベントを作成する
     */
    static MEvent createEvent(const TouchPoint &point, const Vector2f beforePosition, const jc_sp<View> view) {
        jc_sp<DragEventExtension> extension(new DragEventExtension(point, beforePosition, view));
        MEvent result(new Event(BroadcastType_Drag, extension));
        return result;
    }

    /**
     * ドラッグ終了イベントを作成する
     */
    static MEvent createDragEndEvent(const TouchPoint &point, const Vector2f beforePosition, const jc_sp<View> view) {
        jc_sp<DragEventExtension> extension(new DragEventExtension(point, beforePosition, view));
        MEvent result(new Event(BroadcastType_DragEnd, extension));
        return result;
    }
};
}
}

#endif /* DRAGEVENT_H_ */
