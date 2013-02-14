/*
 * JCRect.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCRECT_H_
#define JCRECT_H_

namespace jc {

/**
 * 矩形を示す。
 */
template<typename T>
struct RectT {
    T left;
    T top;
    T right;
    T bottom;

    RectT() {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    }

    /**
     * x/y/width/heightを指定する
     */
    void setXYWH(const T x, const T y, const T w, const T h) {
        left = x;
        top = y;
        right = x + w;
        bottom = y + h;
    }

    /**
     * Left/Top/Right/Bottomで一致を確認する
     */
    jcboolean equalsLTRB(const T l, const T t, const T r, const T b) const {
        return left == l && top == t && right == r && bottom == b;
    }

    T width() const {
        return abs(right - left);
    }

    T height() const {
        return abs(bottom - top);
    }

    /**
     * LTRBを指定値動かす
     */
    void offset(const T x, const T y) {
        left += x;
        right += x;

        top += y;
        bottom += y;
    }
};

/**
 * s32型の矩形構造体
 */
typedef RectT<s32> RectI;

/**
 * float型の矩形構造体
 */
typedef RectT<float> RectF;

}

#endif /* JCRECT_H_ */
