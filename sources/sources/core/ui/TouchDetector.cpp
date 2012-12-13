/*
 * TouchDetector.cpp
 *
 *  Created on: 2012/07/24

 */
#include    "jc/ui/TouchDetector.h"

namespace jc {

TouchDetector::TouchDetector(const jc_sp<TouchListener> listener ) {
    this->listener = listener;
    this->pinchDistance = 1;
}

TouchDetector::~TouchDetector() {

}

/**
 * IDを指定してポイント取得する
 */
MTouchPoint TouchDetector::getTouchPointFromId(s32 id) {
    for (int i = 0; i < points.size(); ++i) {
        if (points[i]->getId() == id) {
            return points[i];
        }
    }

    return MTouchPoint();
}

/**
 * IDを指定してポイントを削除する
 */
void TouchDetector::removeTouchPointFromId(s32 id) {
    std::vector<MTouchPoint>::iterator itr = points.begin(), end = points.end();

    while (itr != end) {
        MTouchPoint point = (*itr);
        if (point->getId() == id) {
            itr = points.erase(itr);
            end = points.end();
        } else {
            ++itr;
        }
    }
}

/**
 * インデックスを指定してポイント取得する
 */
MTouchPoint TouchDetector::getTouchPointFromIndex(s32 index) {
    if (index >= 0 && index < points.size()) {
        return points[index];
    }

    return MTouchPoint();
}

namespace {
/**
 * ドラッグとして扱う距離
 */
const float DRUG_DISTANCE = 30.0f;
}

jcboolean TouchDetector::isMultiDrag() const {
    if (!isMultiTap()) {
        return jcfalse;
    }
    return points[0]->isDrag(1) && points[1]->isDrag(1);
}

/**
 * マルチタップの中央を取得する
 */
Vector2f TouchDetector::getMultiTapCenter() const {
    const float x = (points[0]->getBeginPosition().x + points[1]->getBeginPosition().x) / 2;
    const float y = (points[0]->getBeginPosition().y + points[1]->getBeginPosition().y) / 2;
    return Vector2f(x, y);
}

/**
 * マルチタッチポイント間の距離を取得する
 */
float TouchDetector::getMultiTapPointDistance() const {
    const float x = (points[0]->getCurrentPosition().x - points[1]->getCurrentPosition().x);
    const float y = (points[0]->getCurrentPosition().y - points[1]->getCurrentPosition().y);

    const Vector2f v(x, y);
//        jclogf("len v(%f, %f)", x, y);
    return v.length();
}

/**
 * イベントが行われたことを通知する。
 */
void TouchDetector::onTouchEvent(TouchEventProtocol *event) {
//    if (event->getTouchID())
    {
//        jclogf("id = %d | type = %d", event->getTouchID(), event->getEventType());
//        jclogf( "touch v(%f, %f)", event->getEventPosX(), event->getEventPosY());
    }

    s32 id = event->getTouchID(); // タッチポイントのID
    s32 type = event->getEventType(); // イベントタイプ
    MTouchPoint _point = getTouchPointFromId(id); // 対象となるIDを取得する
//    s32 nowPoints = getTouchPoints(); // 現在タッチされている数

    if (!_point) {
        // ポイントが見つからなかった

        // 既に規定以上のポイントがタップされているから無視
        if (getTouchPoints() >= 2 || event->getEventType() != TouchEventProtocol::EVENT_TYPE_BEGIN) {
            return;
        }

        // 既にイベントが発生中だったら何もしない
        /*
         if (!points.empty()) {
         MTouchPoint oncePoint = points[0];
         // ドラッグ中だから何もしない
         if (oncePoint->isDrag(DRUG_DISTANCE)) {
         return;
         }
         }
         */

        // 最初のポイントが既に動いてる？
        Vector2f v(event->getEventPosX(), event->getEventPosY());
        _point.reset(new TouchPoint(id, v));

        // ポイントを追加する
        points.push_back(_point);

        if (isSingleTap()) {
            // シングルタップ開始
            listener->onTouchBegin(this, (*_point));
        } else {
            /*
             for (int i = 0; i < points.size(); ++i) {
             jclogf( "touch[%d] v(%f, %f)", i, points[i]->getBeginPosition().x, points[i]->getBeginPosition().y);
             }
             */
            // ピンチの開始
            const Vector2f centr = getMultiTapCenter();
            pinchDistance = getMultiTapPointDistance();
            listener->onPinchBegin(this, centr);
        }
    } else {
        // イベントを更新する
        _point->onTouchEvent(event, DRUG_DISTANCE);

        if (TouchEventProtocol::EVENT_TYPE_BEGIN == type) {

        } else if (TouchEventProtocol::EVENT_TYPE_MOVE == type) {
            if (isSingleTap()) {
                if (_point->isDrag(DRUG_DISTANCE)) {
                    // ドラッグされた
                    listener->onDrag(this, *_point);
                }
            } else if (isMultiTap()) {
                const Vector2f center = getMultiTapCenter();
                const float now_distance = getMultiTapPointDistance();
                if (now_distance != pinchDistance) {
//                    jclogf("now_distance / pinchDistance = %f / %f", now_distance, pinchDistance);
                    listener->onPinchScaling(this, now_distance / pinchDistance, center);
                    pinchDistance = now_distance;
                }
            }

        } else if (TouchEventProtocol::EVENT_TYPE_END == type) {
            if (isSingleTap()) {
                // シングルタップ
                if (_point->isDraging()) {
                    listener->onDragEnd(this, (*_point));
                } else {
                    listener->onClick(this, (*_point));
                }
                removeTouchPointFromId(event->getTouchID());
            } else {
                const Vector2f center = getMultiTapCenter();
                listener->onPinchEnd(this, center);
                pinchDistance = 0.1f;
                // マルチタップだったら、どっちのポイントも排除する
                points.clear();
            }
        }
    }
}

}

