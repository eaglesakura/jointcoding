/*
 * jcmath.h
 *
 *  Created on: 2012/05/16
 *      Author: Takeshi
 */

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
    T bit = 2;
    const s32 BIT_SIZE = sizeof(T) * 8;

    if (!n) {
        return jcfalse;
    }

    for (s32 i = 0; i < BIT_SIZE; ++i) {
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
 * 円周率
 */
const double PI = 3.141592653589793;

template<typename T>
inline T degree2radian(const T degree) {
    return (degree * PI) / 180;
}

template<typename T>
inline T radian2degree(const T radian) {
    return (radian * 180) / PI;
}

}
#endif /* JCMATH_H_ */
