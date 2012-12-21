/*
 * jcVec3.h
 *
 *  Created on: 2012/10/03
 */

#ifndef JCVEC3_H_
#define JCVEC3_H_

#include "jc/system/Macro.h"
#include <math.h>

namespace jc {

template<typename T>
struct _Vector3 {
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
     * 各座標を指定する
     */
    _Vector3(const T _x = 0, const T _y = 0, const T _z = 0) {
        x = _x;
        y = _y;
        z = _z;
    }

    /**
     * 加算を行う
     */
    _Vector3<T>& operator+=(const _Vector3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return (*this);
    }

    /**
     * 減算を行う
     */
    _Vector3<T>& operator-=(const _Vector3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return (*this);
    }

    /**
     * スカラー演算を行う
     */
    _Vector3<T>& operator*=(const T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return (*this);
    }

    /**
     * 減算を行う
     */
    _Vector3<T> operator-(const _Vector3<T> v) const {
        return _Vector3<T>(x - v.x, y - v.y, z - v.z);
    }

    /**
     * 除算を行う
     */
    _Vector3<T> operator/=(const T div) {
        x /= div;
        y /= div;
        z /= div;
        return (*this);
    }

    void set(const T _x, const T _y, const T _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    /**
     * 同一性チェック
     */
    bool operator==(const _Vector3<T> &v) const {
        return equals(x, v.x) && equals(y, v.y) && equals(z, v.z);
    }

    /**
     * 同一性チェック
     */
    bool operator!=(const _Vector3<T> &v) const {
        return !equals(x, v.x) || !equals(y, v.y) || !equals(z, v.z);
    }

    /**
     * 長さを取得する。
     *
     * @return
     */
    T length() const {
        return (T) sqrt(((double) x * (double) x) + ((double) y * (double) y) + ((double) z * (double) z));
    }

    /**
     * 外積を取得する。
     *
     *
     * @param v
     * @param result
     * @return
     *
     */
    _Vector3<T>* cross(const _Vector3<T> &v, _Vector3<T> *result) const {
        result->set((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x));
        return result;
    }

    /**
     * 内積を取得する。
     *
     *
     * @param v
     * @return
     *
     */
    T dot(const _Vector3<T> v) const {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    /**
     * ベクトルの長さを正規化する
     */
    void normalize() {
        const T _len = length();

        x /= _len;
        y /= _len;
        z /= _len;
    }

    /**
     *
     *
     * @param p
     * @return
     */
    T length(const _Vector3<T> &p) const {
        const double tx = x - p.x;
        const double ty = y - p.y;
        const double tz = z - p.z;
        return (T) sqrt((tx * tx) + (ty * ty) + (tz * tz));
    }

    /**
     *
     *
     * @param p
     * @return
     */
    T length(const T px, const T py, const T pz) const {
        const double tx = x - px;
        const double ty = y - py;
        const double tz = z - pz;
        return (T) sqrt((tx * tx) + (ty * ty) + (tz * tz));
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
typedef _Vector3<s32> Vector3i;

/**
 * 浮動小数のベクトル
 */
typedef _Vector3<float> Vector3f;

}

#endif /* JCVEC3_H_ */
