/*
 * LinearLayout.hpp
 *
 *  Created on: 2013/04/24
 */

#ifndef LINEARLAYOUT_HPP_
#define LINEARLAYOUT_HPP_

#include    "jc/widget/View.h"

namespace jc {
namespace view {

/**
 * レイアウトの配置方向
 */
enum LayoutOrientation_e {
    /**
     * 縦方向に配置する
     */
    LayoutOrientation_Vertical,

    /**
     * 横方向に配置する
     */
    LayoutOrientation_Horizontal,
};

class LinearLayout: public View {

protected:

    /**
     * 最後に登録されたViewを取得する
     */
    virtual MView getLastView() const {
        if (childs.empty()) {
            return MView();
        }

        return downcast<View>(childs.back());
    }

    virtual RectF getLastViewArea() const {
        MView view = getLastView();
        if (view) {
            RectF last = view->getLocalLayoutArea();
            if (last.empty()) {
                last = view->getLocalLayoutAreaNest();
                view->layoutDirect(last);
            }
            return last;
        } else {
            return RectF();
        }
    }

    /**
     * レイアウトの構築方向
     */
    LayoutOrientation_e orientation;
public:
    LinearLayout(MWindowContext context) : View(context) {
        orientation = LayoutOrientation_Vertical;

        setFocusable(jcfalse);
        setTouchable(jcfalse);
        setFocusableFromTouch(jcfalse);
    }

    virtual ~LinearLayout() {
    }

    /**
     * レイアウトの配置方向を取得する
     */
    virtual void setOrientation(const LayoutOrientation_e orientation) {
        this->orientation = orientation;
    }

    /**
     * 次のViewを登録する
     * 登録と同時にレイアウト位置を修正する
     */
    virtual void addLayout(MView layout, const Vector2f &margin) {

        // 最後に追加されたViewのローカル位置を取得する
        const RectF lastArea = getLastViewArea();

        if (orientation == LayoutOrientation_Horizontal) {
            layout->moveTo(lastArea.right + margin.x, margin.y);
        } else {
            layout->moveTo(margin.x, lastArea.bottom + margin.y);
        }
        addSubView(layout);

        // 自身の大きさを調整する
        layoutDirect(getLocalLayoutAreaNest());
    }
};

typedef jc_sp<LinearLayout> MLinearLayout;

}
}

#endif /* LINEARLAYOUT_HPP_ */
