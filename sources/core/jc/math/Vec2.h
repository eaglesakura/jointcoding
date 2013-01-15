/*
 * jcVec2.h
 *
 *  Created on: 2012/07/23

 */

#ifndef JCVEC2_H_
#define JCVEC2_H_

#include "jc/system/Macro.h"
#include <math.h>

namespace jc {

template<typename T>
struct _Vector2 {
    /**
     * X座標を示す
     */
    T x;

    /**
     * Y座標を示す
     */
    T y;

    /**
     * 各座標を指定する
     */
    _Vector2(const T _x = 0, const T _y = 0) {
        x = _x;
        y = _y;
    }

    /**
     * 加算を行う
     */
    _Vector2<T>& operator+=(const _Vector2 &v) {
        x += v.x;
        y += v.y;
        return (*this);
    }

    /**
     * 減算を行う
     */
    _Vector2<T>& operator-=(const _Vector2 &v) {
        x -= v.x;
        y -= v.y;
        return (*this);
    }

    /**
     * スカラー演算を行う
     */
    _Vector2<T> operator*=(const T scalar) {
        x *= scalar;
        y *= scalar;
        return (*this);
    }

    /**
     * 除算を行う
     */
    _Vector2<T> operator/=(const T div) {
        x /= div;
        y /= div;
        return (*this);
    }

    /**
     * 同一性チェック
     */
    bool operator==(const _Vector2<T> &v) const {
        return equals(x, v.x) && equals(y, v.y);
    }

    /**
     * 同一性チェック
     */
    bool operator!=(const _Vector2<T> &v) const {
        return !equals(x, v.x) || !equals(y, v.y);
    }

    /**
     * 長さを取得する。
     *
     * @return
     */
    T length() const {
        return (T) sqrt(((double) x * (double) x) + ((double) y * (double) y));
    }

    /**
     *
     *
     * @param p
     * @return
     */
    T length(const _Vector2<T> &p) const {
        double tx = x - p.x, ty = y - p.y;
        return (T) sqrt((tx * tx) + (ty * ty));
    }

    /**
     *
     *
     * @param p
     * @return
     */
    T length(const T px, const T py) const {
        double tx = x - px, ty = y - py;
        return (T) sqrt((tx * tx) + (ty * ty));
    }

    /**
     * 配列風のアクセスを提供する
     */
    T& operator[](const s32 index) {
        return ((T*) this)[index];
    }
};

/**
 * signed int のベクトル
 */
typedef _Vector2<s32> Vector2i;

/**
 * 浮動小数のベクトル
 */
typedef _Vector2<float> Vector2f;

}

#endif /* JCVEC2_H_ */
