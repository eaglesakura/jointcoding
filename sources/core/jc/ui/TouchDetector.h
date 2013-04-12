/*
 * jcTouchDetector.h
 *
 *  Created on: 2012/07/24

 */

#ifndef JCTOUCHDETECTOR_H_
#define JCTOUCHDETECTOR_H_

#include    "jc/ui/TouchPoint.h"
#include    "jc/mem/SmartPtr.h"
#include    "protocol/jcTouchEventProtocol.h"
#include    "jc/math/Math.h"
#include    <vector>

namespace jc {

class TouchDetector;

enum TouchCompleteType_e {
    /**
     * クリックされた
     */
    TouchCompleteType_SingleClick,

    /**
     * ドラッグされた
     */
    TouchCompleteType_Drag,

    /**
     * ピンチ操作
     */
    TouchCompleteType_Pinch,
};

/**
 * タッチ操作のコールバックを受け取る。
 */
class TouchListener {
public:
    virtual ~TouchListener() {

    }

    /**
     * Viewがクリックされた。
     * 指が離された瞬間に認識される
     * onDragEnd()とどちらかが呼び出される。
     */
    virtual void onClick(const TouchDetector* detector, const TouchPoint &point) = 0;

    /**
     * ドラッグを終了させた。
     * onClick()とどちらかが呼び出される。
     */
    virtual void onDragEnd(const TouchDetector *detector, const TouchPoint &point) = 0;

    /**
     * タッチが開始された
     */
    virtual void onTouchBegin(const TouchDetector* detector, const TouchPoint &point) = 0;

    /**
     * タッチが終了された
     * onTouchBegin -> onClick -> onTouchEnd
     * onTouchBegin -> onDrag -> onDragEnd -> onTouchEnd
     */
    virtual void onSingleTouchEnd(const TouchDetector* detector, const TouchCompleteType_e type, const TouchPoint &point) = 0;

    /**
     * Viewがドラッグされた
     */
    virtual void onDrag(const TouchDetector* detector, const TouchPoint &point) = 0;

    /**
     * マルチタッチが開始された
     */
    virtual void onPinchBegin(const TouchDetector *detector, const Vector2f &center) = 0;

    /**
     * ピンチが変更された
     */
    virtual void onPinchScaling(const TouchDetector *detector, const float scale, const Vector2f &center) = 0;

    /**
     * ピンチ操作が終了された
     */
    virtual void onPinchEnd(const TouchDetector *detector, const Vector2f &center) = 0;
};

/**
 * タッチの解析を行う
 * タッチ地点は今のところ2点まで認識させる。
 */
class TouchDetector: public Object {
    /**
     *
     */
    std::vector<MTouchPoint> points;

    /**
     * 通知するリスナ
     */
    jc_sp<TouchListener> listener;

    /**
     * 最後にチェックした場所のポイント感の距離
     */
    float pinchDistance;

    /**
     * IDを指定してポイント取得する
     */
    virtual MTouchPoint getTouchPointFromId(s32 id);

    /**
     * IDを指定してポイントを削除する
     */
    virtual void removeTouchPointFromId(s32 id );

    /**
     * 1点タッチをしている
     */
    virtual jcboolean isSingleTap() const {
        return points.size() == 1;
    }

    /**
     * 2点間タップをしている
     */
    virtual jcboolean isMultiTap() const {
        return points.size() == 2;
    }

    /**
     * マルチタップで両方ドラッグしていたtrue
     */
    virtual jcboolean isMultiDrag() const;

    /**
     * マルチタップの中央を取得する
     */
    virtual Vector2f getMultiTapCenter() const;
    /**
     * マルチタッチポイント間の距離を取得する
     */
    virtual float getMultiTapPointDistance() const;

public:
    TouchDetector(const jc_sp<TouchListener> listener );

    virtual ~TouchDetector();

    /**
     * タッチされている地点の数を取得する
     */
    virtual s32 getTouchPoints() const {
        return points.size();
    }

    /**
     * インデックスを指定してポイント取得する
     */
    virtual MTouchPoint getTouchPointFromIndex(s32 index);

    /**
     * イベントが行われたことを通知する。
     */
    virtual void onTouchEvent(TouchEventProtocol *event);
};

/**
 * managed
 */
typedef jc_sp<TouchDetector> MTouchDetector;

}

#endif /* JCTOUCHDETECTOR_H_ */
