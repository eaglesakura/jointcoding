/*
 * jcmath.h
 *
 *  Created on: 2012/05/16
 *      Author: Takeshi
 */

#include <math.h>

#ifndef JCMATH_H_
#define JCMATH_H_

namespace jc {

/**
 * 最小値を返す
 */
template<class T>
inline const T min(const T a, const T b) {
    return a < b ? a : b;
}

/**
 * 最大値を返す
 */
template<class T>
inline const T max(const T a, const T b) {
    return a > b ? a : b;
}

/**
 * 値が_min <= value <= _maxになるように正規化してvalueを返す。
 */
template<class T>
inline const T minmax(const T _min, const T _max, const T value) {
    if (value < _min) {
        return _min;
    }

    if (value > _max) {
        return _max;
    }

    return value;
}

/**
 * 整数nが2の乗数の場合、trueを返す。
 */
template<class T>
inline jcboolean isPowerOfTwo(const T n) {
    if (!n) {
        return jcfalse;
    }

    T bit = 1;
    for (s32 i = 0; i < 32; ++i) {
        if (n == bit) {
            return jctrue;
        }
        bit <<= 1;
    }

    return jcfalse;
}

enum {
    /**
     * OpenGL ESで利用する固定小数値
     */
    FIXED_BIT_GL = 16,

    /**
     * MascotCapsuleで利用する固定小数値
     */
    FIXED_BIT_MC = 12,
};
/**
 * 実数を固定小数に変換する
 */
template<typename InRealType, typename OutIntegerType>
inline OutIntegerType real2fixed(const InRealType data, const s32 bits) {
    const s32 mul = 0x1 << bits;
    return (OutIntegerType) (((double) data) * (double) mul);
}

/**
 * 固定小数を実数に変換する
 */
template<typename InIntegerType, typename OutRealType>
inline OutRealType fixed2real(const InIntegerType data, const s32 bits) {
    const s32 mul = 0x1 << bits;
    return (OutRealType) (((double) data) / ((double) mul));
}

/**
 * 特定値に近づくように値を移動する
 */
template<typename T>
inline T targetMove(const T now, const T target, const T _offset) {
    const T offset = abs(_offset);
    if (abs(target - now) <= offset) {
        return target;
    } else if (target > now) {
        return now + offset;
    } else {
        return now - offset;
    }
}
}
#endif /* JCMATH_H_ */
