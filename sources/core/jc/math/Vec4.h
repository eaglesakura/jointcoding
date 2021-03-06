/*
 * jcVec3.h
 *
 *  Created on: 2012/10/03
 */

#ifndef JCVEC4_H_
#define JCVEC4_H_

#include "jc/system/Macro.h"
#include <math.h>

namespace jc {

template<typename T>
struct _Vector4 {
    /**
     * X座標を示す
     */
    T x;

    /**
     * Y座標を示す
     */
    T y;

    /**
     * Z座標を示す
     */
    T z;

    /**
     *
     */
    T w;

    /**
     * 各座標を指定する
     */
    _Vector4(const T _x = 0, const T _y = 0, const T _z = 0, const T _w = 0) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    /**
     * 加算を行う
     */
    _Vector4<T>& operator+=(const _Vector4 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return (*this);
    }

    /**
     * 減算を行う
     */
    _Vector4<T>& operator-=(const _Vector4 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        x -= x.z;
        return (*this);
    }

    /**
     * スカラー演算を行う
     */
    _Vector4<T> operator*=(const T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return (*this);
    }

    /**
     * 除算を行う
     */
    _Vector4<T> operator/=(const T div) {
        x /= div;
        y /= div;
        z /= div;
        w /= div;
        return (*this);
    }

    /**
     * 同一性チェック
     */
    bool operator==(const _Vector4<T> &v) {
        return equals(x, v.x) && equals(y, v.y) && equals(z, v.z) && equals(w, v.w);
    }

    /**
     * 同一性チェック
     */
    bool operator!=(const _Vector4<T> &v) {
        return !equals(x, v.x) || !equals(y, v.y) || !equals(z, v.z) || !equals(w, v.w);
    }

    /**
     * 長さを取得する。
     *
     * @return
     */
    T length() const {
        return (T) sqrt(((double) x * (double) x) + ((double) y * (double) y) + ((double) z * (double) z) + ((double) w * (double) w));
    }

    /**
     *
     *
     * @param p
     * @return
     */
    T length(const _Vector4<T> &p) const {
        const double tx = x - p.x;
        const double ty = y - p.y;
        const double tz = z - p.z;
        const double tw = w - p.w;
        return (T) sqrt((tx * tx) + (ty * ty) + (tz * tz) + (tw * tw));
    }

    /**
     *
     *
     * @param p
     * @return
     */
    T length(const T px, const T py, const T pz, const T pw) const {
        const double tx = x - px;
        const double ty = y - py;
        const double tz = z - pz;
        const double tw = w - pw;
        return (T) sqrt((tx * tx) + (ty * ty) + (tz * tz) + (tw * tw));
    }

    /**
     * 値を設定する
     */
    void set(const T X, const T Y, const T Z, const T W) {
        x = X;
        y = Y;
        z = Z;
        w = W;
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
typedef _Vector4<s32> Vector4i;

/**
 * 浮動小数のベクトル
 */
typedef _Vector4<float> Vector4f;

/**
 * ２ベクトルを線形補間する
 */
template<typename T>
inline _Vector4<T>* lerp(const _Vector4<T> &before, const _Vector4<T> &after, const float weight, _Vector4<T> *result) {

    const T x = (after.x * weight) + (before.x * (1.0f - weight));
    const T y = (after.y * weight) + (before.y * (1.0f - weight));
    const T z = (after.z * weight) + (before.z * (1.0f - weight));
    const T w = (after.w * weight) + (before.w * (1.0f - weight));

    result->x = x;
    result->y = y;
    result->z = z;
    result->w = w;

    return result;
}

}

#endif /* JCVEC3_H_ */
