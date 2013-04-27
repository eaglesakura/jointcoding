/*
 * ScrollRange.hpp
 *
 *  Created on: 2013/04/26
 */

#ifndef SCROLLRANGE_HPP_
#define SCROLLRANGE_HPP_

#include    "jointcoding.h"

namespace jc {

/**
 * スクロール表示管理用
 */
class SrollRange {
    /**
     * 表示エリア
     */
    RectF viewport;

    /**
     * Viewの存在エリア
     */
    RectF view;

    float getViewTop() const {
        return jc::min(viewport.top, view.top);
    }
    float getViewBottom() const {
        return jc::max(viewport.bottom, view.bottom);
    }

    /**
     * Viewの高さを取得する
     */
    float getViewHeight(float *top, float *bottom) const {
        assert(top);
        assert(bottom);

        *top = getViewTop();
        *bottom = getViewBottom();

        return (*bottom) - (*top);
    }

public:
    SrollRange() {
    }

    ~SrollRange() {
    }

    /**
     * スクロールエリアが有効であることを確認する
     */
    jcboolean valid() const {
        return !viewport.empty() && !view.empty();
    }
    /**
     * 表示領域を設定する
     */
    void setViewport(const RectF &viewport) {
        this->viewport = viewport;
    }

    RectF getViewport() const {
        return viewport;
    }

    /**
     * Viewの実際にある領域を設定する
     */
    void setViewArea(const RectF &view) {
        this->view = view;
    }

    RectF getViewArea() const {
        return view;
    }

    /**
     * 縦方向がレンジ外になったらtrueを返す
     */
    jcboolean isVerticalOverRange() const {
        if (!valid()) {
            return jcfalse;
        }
        return view.top < viewport.top || view.bottom > viewport.bottom;
    }

    /**
     * 縦方向スクロールが有効ならtrue
     */
    jcboolean isVerticalScrollEnable() const {
        return view.height() > viewport.height();
    }

    /**
     * Viewの上端がViewportよりも下ならtrueを返す
     */
    jcboolean isTopOverRange() const {
        return view.top > viewport.top;
    }

    /**
     * Viewの下端がViewportよりも上ならtrueを返す
     */
    jcboolean isBottomOverRange() const {
        return view.bottom < viewport.bottom;
    }

    /**
     * View位置が下端に達していたらtrueを返す
     */
    jcboolean isBottomLine() const {
        return getViewBottom() <= viewport.bottom;
    }

    /**
     * View位置が上端に達していたらtrueを返す
     */
    jcboolean isTopLine() const {
        return getViewTop() >= viewport.top;
    }

    /**
     * スクロールバーを表示する高さを設定する
     * 表示領域はviewport上に投影される
     */
    void getVerticalRange(float *barBeginPos, float *barEndPos) const {
        float viewTop;
        float viewBottom;
        const float viewHeight = getViewHeight(&viewTop, &viewBottom);
        const float vpHeight = viewport.height();

        if (viewHeight <= vpHeight) {
            *barBeginPos = viewport.top;
            *barEndPos = viewport.bottom;
            return;
        }

        // viewportに対するスクロールバーの割合を決定する
        const float barWeight = (vpHeight / viewHeight);
        const float barHeight = vpHeight * barWeight;
        const float barOver = vpHeight - barHeight;

        // Viewのはみ出した量
        const float viewOverPixel = (viewHeight - vpHeight);
        const float nowOverPixel = (viewBottom - viewport.bottom);

        // Viewのはみ出した量の割合
        const float nowOverWeight = nowOverPixel / viewOverPixel;

        const float barEnd = viewport.bottom - (barOver * nowOverWeight);
        const float barBegin = barEnd - barHeight;

        *barBeginPos = barBegin;
        *barEndPos = barEnd;
    }

    /**
     * 縦方向のスクロールバーを持つならtrueを返す
     */
    jcboolean hasVerticalScroll() const {
        return viewport.height() < view.height();
    }
};

}

#endif /* SCROLLRANGE_HPP_ */
