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
     * 表示領域を設定する
     */
    void setViewport(const RectF &viewport) {
        this->viewport = viewport;
    }

    /**
     * Viewの実際にある領域を設定する
     */
    void setViewArea(const RectF &view) {
        this->view = viewport;
    }

    /**
     * 表示レンジを取得する
     * 値は上端 0.0〜下端 1.0で取得する
     */
    void getVerticalRange(float *barBeginPos, float *barEndPos) const {
        float viewTop;
        float viewBottom;
        const float viewHeight = getViewHeight(&viewTop, &viewBottom);
        const float vpHeight = viewport.height();

        // viewportに対するスクロールバーの割合を決定する
        const float barWeight = (vpHeight / viewHeight);

        // 現在のTOP位置の割合を見る
        const float topWeight = (viewTop - viewport.top) / vpHeight;

        const float barBegin = viewport.top + (vpHeight * topWeight);
        const float barEnd = barBegin + (vpHeight * barWeight);

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