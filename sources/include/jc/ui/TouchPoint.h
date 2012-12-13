/*
 * jcTouchPoint.h
 *
 *  Created on: 2012/07/23

 */

#ifndef JCTOUCHPOINT_H_
#define JCTOUCHPOINT_H_

#include    "jc/math/Vec2.h"
#include    "boost/timer.hpp"
#include    "jc/model/Object.h"
#include    "jc/mem/SmartPtr.h"
#include    "protocol/jcTouchEventProtocol.h"

namespace jc {

class TouchDetector;
/**
 * 一枚のViewに対して、触れられている点を管理する。
 * Viewのローカル座標系を管理する。
 */
class TouchPoint: public Object {
    /**
     * タッチ位置のユニークなID。
     * IDは使いまわされる可能性があることに注意する。
     */
    s32 id;

    /**
     * タッチの開始座標
     */
    Vector2f beginPos;

    /**
     * タッチの現在座標
     */
    Vector2f currentPos;

    /**
     * タッチ開始時刻
     */
    boost::timer beginTime;

    /**
     * タッチ時間
     */
    u32 touchTimeMs;

    /**
     * ドラッグしたことがあったらtrue
     */
    jcboolean draged;
public:

    TouchPoint(const s32 id, const Vector2f &beginPos);

    virtual ~TouchPoint();

    /**
     * タッチ開始した地点を取得する
     */
    virtual const Vector2f& getBeginPosition() const {
        return beginPos;
    }

    /**
     * 現在タッチしている地点を取得する
     */
    virtual const Vector2f getCurrentPosition() const {
        return currentPos;
    }

    /**
     * タッチしていた時間を取得する。
     * 但し、指を話していない場合は0が帰る。
     */
    virtual u32 getTouchTimeMs() const {
        return touchTimeMs;
    }

    /**
     * タッチが終わったポイントだったらtrueを返す。
     */
    virtual jcboolean isTouchFinished() const {
        return touchTimeMs != 0;
    }

    /**
     * タッチを開始した時間を取得する。
     */
    virtual const boost::timer& getBeginTime() const {
        return beginTime;
    }

    /**
     * タップ時間をミリ秒単位で取得する
     */
    virtual u32 getTapTimeMs() const {
        return (u32) (beginTime.elapsed() * 1000);
    }

    /**
     * ロングタップされているならtrue
     * @timeMs このミリ秒以上のタップ時間だったらロングタップとして扱う
     */
    virtual jcboolean isLongTap(const u32 timeMs) const {
        return getTapTimeMs() > timeMs;
    }

    /**
     * 指定距離以上ドラッグしていたらドラッグとして扱う
     */
    virtual jcboolean isDrag(const float distance) const {
        float drug_length = beginPos.length(currentPos);
        return drug_length > distance;
    }

    /**
     * タッチの移動距離を取得する
     */
    virtual float getMoveDistance() const {
        float length = beginPos.length(currentPos);
        return length;
    }

    /**
     * 識別用のIDを取得する
     */
    virtual s32 getId() const {
        return id;
    }

    /**
     * ドラッグ済みだったらtrueを返す
     */
    virtual jcboolean isDraging() const {
        return draged;
    }

    /**
     * イベントが更新された
     * @return 削除の必要があるならtrue
     */
    virtual void onTouchEvent(TouchEventProtocol *event, const float dragDist);
};

/**
 * managed
 */
typedef jc_sp<TouchPoint> MTouchPoint;

}

#endif /* JCTOUCHPOINT_H_ */
