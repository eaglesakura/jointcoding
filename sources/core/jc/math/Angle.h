/*
 * Angle.h
 *
 *  Created on: 2013/01/23
 */

#ifndef ANGLE_H_
#define ANGLE_H_

#include    "jc/math/Math.h"

namespace jc {

/**
 * 360度系の正規化を行う。
 *
 *
 * @param now
 * @return
 *
 */
template<typename T>
inline T normalizeDegree(T now) {
    while (now < 0) {
        now += 360.0f;
    }

    while (now >= 360) {
        now -= 360;
    }

    return now;
}

/**
 * 円周率
 */
const double PI = 3.141592653589793;

/**
 * centerから見たpositionが何度になるか360度系で返す。
 * １２時の方向が0度で、反時計回りに角度を進ませる。
 * @param center
 * @param position
 * @return
 */
template<typename VectorType>
float getAngleDegree2D(const VectorType &center, const VectorType &position) {
    float result = 0;

    VectorType temp(position.x - center.x, center.y - position.y);
    if (temp.length() == 0) {
        return 0;
    }
    temp.normalize();

    result = (float) (atan2(temp.y, temp.x) / PI);
    result /= 2;
    result -= 0.25f;

    return normalizeDegree(result * 360.0f);
}

/**
 * -180〜180の間に角度を収める
 */
template<typename T>
inline T getHalfRotateDegree(const T f) {
    return f > 180 ? (f - 360) : f;
}

/**
 * 最短ルートを通って回転を行う
 */
template<typename T>
T turnAngleDegree(const T _current, const T _target, const T offset) {
    T current = getHalfRotateDegree(normalizeDegree(_current));
    T target = getHalfRotateDegree(normalizeDegree(_target));

    if (abs(current - target) <= 180) {
        return normalizeDegree(targetMove(current, target, offset));
    } else {
        if (current > target) {
            current -= 360;
        } else {
            current += 360;
        }
        return normalizeDegree(targetMove(current, target, offset));
    }
}

/**
 *
 */
template<typename T>
inline T degree2radian(const T degree) {
    return (degree * PI) / 180;
}

/**
 *
 */
template<typename T>
inline T radian2degree(const T radian) {
    return (radian * 180) / PI;
}

}

#endif /* ANGLE_H_ */
