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
        if( n == bit ) {
            return jctrue;
        }
        bit <<= 1;
    }

    return jcfalse;
}

}
#endif /* JCMATH_H_ */
