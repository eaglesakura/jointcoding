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
     * 加算を行う
     */
    _Vector2<T> operator+(const _Vector2 &v) const {
        return _Vector2<T>(x + v.x, y + v.y);
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
     * 減算を行う
     */
    _Vector2<T> operator-(const _Vector2 &v) const {
        return _Vector2<T>(x - v.x, y - v.y);
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
     * ベクトルの長さを正規化する
     */
    void normalize() {
        const T _len = length();

        x /= _len;
        y /= _len;
    }

    /**
     * 配列風のアクセスを提供する
     */
    T& operator[](const s32 index) {
        return ((T*) this)[index];
    }

    void set(const T _x, const T _y) {
        x = _x;
        y = _y;
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

inline jcboolean equals(const Vector2f &v0, const Vector2f &v1) {
    return jc::equals(v0.x, v1.x) && jc::equals(v0.y, v1.y);
}

inline jcboolean equals(const Vector2f &v0, const Vector2f &v1, const float _check) {
    return jc::equals(v0.x, v1.x, _check) && jc::equals(v0.y, v1.y, _check);
}

/**
 * ２ベクトルを線形補間する
 * 0.0 = before
 * 1.0 = after
 * となる。
 */
template<typename T>
inline _Vector2<T>* lerp(const _Vector2<T> &before, const _Vector2<T> &after, const float weight, _Vector2<T> *result) {

    const T x = (after.x * weight) + (before.x * (1.0f - weight));
    const T y = (after.y * weight) + (before.y * (1.0f - weight));

    result->x = x;
    result->y = y;

    return result;
}

}

#endif /* JCVEC2_H_ */
