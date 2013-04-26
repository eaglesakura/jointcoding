/*
 * JCRect.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCRECT_H_
#define JCRECT_H_

#include    "jc/math/Math.h"
#include    "jc/math/Vec2.h"

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

    RectT(const RectT &origin) {
        left = (T) origin.left;
        top = (T) origin.top;
        right = (T) origin.right;
        bottom = (T) origin.bottom;
    }

    /**
     * 中央地点を設定する
     */
    void setCenter(const T x, const T y) {
        const T _width = width() / 2;
        const T _height = height() / 2;

        left = x - _width;
        right = x + _width;

        top = y - _height;
        bottom = y + _height;
    }

    /**
     * 内側にX/Y分小さくする
     */
    void inset(const T x, const T y) {
        left += x;
        right -= x;
        top += y;
        bottom -= y;
    }

    /**
     * XとYがRectに含まれているならばtrueを返す
     * 同一座標も含まれる。
     */
    jcboolean isIntersect(const T x, const T y) const {
        return x >= left && x <= right && y >= top && y <= bottom;
    }

    /**
     * 面積を持たない場合trueを返す
     */
    jcboolean empty() {
        return (left == right) || (top == bottom);
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
     * 指定座標がTOPになるように移動させる
     */
    void moveToTop(const T top) {
        const T _height = height();
        this->top = top;
        this->bottom = top + _height;
    }

    /**
     * 指定座標がXの中央になるように移動させる
     */
    void moveToCenterX(const T center) {
        const T _width = width();
        left = center - (_width / 2);
        right = left + _width;
    }

    /**
     * 指定座標がYの中央になるように移動させる
     */
    void moveToCenterY(const T center) {
        const T _height = height();
        top = center - (_height / 2);
        bottom = top + _height;
    }

    /**
     * 指定座標がBOTTOMになるように移動させる
     */
    void moveToBottom(const T bottom) {
        const T _height = height();
        this->bottom = bottom;
        this->top = bottom - _height;
    }

    /**
     * 左位置を設定する
     */
    void moveToLeft(const T left) {
        const T _width = width();
        this->left = left;
        this->right = left + _width;
    }

    /**
     * 右位置を設定する
     */
    void moveToRight(const T right) {
        const T _width = width();
        this->right = right;
        this->left = right - _width;
    }

    /**
     * 特定のXY位置に移動する
     */
    void moveTo(const T newLeft, const T newTop) {
        moveToLeft(newLeft);
        moveToTop(newTop);
    }

    /**
     * Left/Top/Right/Bottomで一致を確認する
     */
    jcboolean equalsLTRB(const T l, const T t, const T r, const T b) const {
        return left == l && top == t && right == r && bottom == b;
    }

    /**
     * 同一の場合はtrueを返す
     */
    bool operator==(const RectT &check) const {
        return left == check.left && top == check.top && right == check.right && bottom == check.bottom;
    }

    /**
     * 違う場合はtrueを返す
     */
    bool operator!=(const RectT &check) const {
        return !(left == check.left && top == check.top && right == check.right && bottom == check.bottom);
    }

    T width() const {
        return abs(right - left);
    }

    T height() const {
        return abs(bottom - top);
    }

    T centerX() const {
        return (right + left) / 2;
    }

    T centerY() const {
        return (top + bottom) / 2;
    }

    /**
     * center
     */
    _Vector2<T> center() const {
        return _Vector2<T>(centerX(), centerY());
    }

    /**
     * width/height
     */
    _Vector2<T> wh() const {
        return _Vector2<T>(width(), height());
    }

    /**
     * left/top
     */
    _Vector2<T> lt() const {
        return _Vector2<T>(left, top);
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

    /**
     * 矩形を動かす
     */
    void offset(const T off_left, const T off_top, const T off_right, const T off_bottom) {
        left += off_left;
        top += off_top;
        right += off_right;
        bottom += off_bottom;
    }

    /**
     * すべての値を四捨五入する
     */
    void round() {
        left = jc::round(left);
        top = jc::round(top);
        right = jc::round(right);
        bottom = jc::round(bottom);
    }

    void cutdown() {
        left = (s32) left;
        top = (s32) top;
        right = (s32) right;
        bottom = (s32) bottom;
    }

    /**
     * 面積を求める
     */
    T getAreaSize() const {
        return jc::abs(width() * height());
    }

    void copy(const RectT<s32> origin) {
        left = (T) origin.left;
        top = (T) origin.top;
        right = (T) origin.right;
        bottom = (T) origin.bottom;
    }

    void copy(const RectT<float> origin) {
        left = (T) origin.left;
        top = (T) origin.top;
        right = (T) origin.right;
        bottom = (T) origin.bottom;
    }

    /**
     * 中央値を保ったままスケーリングする
     */
    void scaling(const float scale_x, const float scale_y) {
        const T cen_x = centerX();
        const T cen_y = centerY();
        const T _width = (T) ((float) width() * scale_x);
        const T _height = (T) ((float) height() * scale_y);

        left = cen_x - (_width / 2);
        top = cen_y - (_height / 2);
        right = left + _width;
        bottom = top + _height;
    }

    /**
     * 中央値を保ったままスケーリングする
     */
    void scaling(const float scale) {
        scaling(scale, scale);
    }

    /**
     * Left/Topの位置に移動したRectを生成する
     */
    RectT<T> createMoveTo(const T left, const T top) const {
        RectT<T> result = (*this);
        result.moveTo(left, top);
        return result;
    }

    /**
     * X中心の位置に移動したRectを生成する
     */
    RectT<T> createMoveToCenterX(const T x) const {
        RectT<T> result = (*this);
        result.moveToCenterX(x);
        return result;
    }

    /**
     * Y中心の位置に移動したRectを生成する
     */
    RectT<T> createMoveToCenterY(const T y) const {
        RectT<T> result = (*this);
        result.moveToCenterY(y);
        return result;
    }

    /**
     * Rightの位置に移動したRectを生成する
     */
    RectT<T> createMoveToRight(const T right) const {
        RectT<T> result = (*this);
        result.moveToRight(right);
        return result;
    }

    /**
     * Bottomの位置に移動したRectを生成する
     */
    RectT<T> createMoveToBottom(const T bottom) const {
        RectT<T> result = (*this);
        result.moveToBottom(bottom);
        return result;
    }

    /**
     * 内側にnだけ移動させた四角形を作成する
     */
    RectT<T> createInset(const T x, const T y) const {
        RectT<T> result;
        result.left = (left + x);
        result.top = (top + y);
        result.right = (right - x);
        result.bottom = (bottom - y);

        return result;
    }

    /**
     * 矩形を動かす
     */
    RectT<T> createOffset(const T off_left, const T off_top, const T off_right, const T off_bottom) const {
        RectT<T> result;

        result.left = (left + off_left);
        result.top = (top + off_top);
        result.right = (right + off_right);
        result.bottom = (bottom + off_bottom);

        return result;
    }

    RectT<T> createScaling(const float scale_x, const float scale_y) const {
        const T cen_x = centerX();
        const T cen_y = centerY();
        const T _width = (T) ((float) width() * scale_x);
        const T _height = (T) ((float) height() * scale_y);

        RectT<T> result;
        result.left = cen_x - (_width / 2);
        result.top = cen_y - (_height / 2);
        result.right = result.left + _width;
        result.bottom = result.top + _height;
        return result;
    }

    RectT<T> createScaling(const float scale) const {
        return createScaling(scale, scale);
    }

    /**
     * 水平方向が長い四角形の場合true
     */
    jcboolean isHorizontalLong() const {
        return width() > height();
    }

    /**
     * 垂直方向が長い四角形の場合true
     */
    jcboolean isVerticalLong() const {
        return width() < height();
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

template<typename T>
RectT<T> createRectFromXYWH(const T x, const T y, const T w, const T h) {
    RectT<T> result;
    result.left = x;
    result.top = y;
    result.right = x + w;
    result.bottom = y + h;

    return result;
}

template<typename T>
RectT<T> createRectFromLTRB(const T x, const T y, const T r, const T b) {
    RectT<T> result;
    result.left = x;
    result.top = y;
    result.right = r;
    result.bottom = b;

    return result;
}

template<typename T>
RectT<T> createRectFromCWH(const T centerX, const T centerY, const T width, const T height) {
    RectT<T> result;
    result.left = centerX - (width / 2);
    result.top = centerY - (height / 2);
    result.right = result.left + width;
    result.bottom = result.top + height;

    return result;
}

}

#endif /* JCRECT_H_ */
