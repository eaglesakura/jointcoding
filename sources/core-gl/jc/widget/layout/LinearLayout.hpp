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
enum LayoutDirection_e {
    /**
     * 縦方向に配置する
     */
    LayoutDirection_Vertical,

    /**
     * 横方向に配置する
     */
    LayoutDirection_Horizontal,
};

class LinearLayout: public View {

protected:

    /**
     * 次のViewを表示するヘッダ位置
     */
    Vector2f nextHeader;

    /**
     * 最後に登録されたViewを取得する
     */
    virtual MView getLastView() const {
        if (childs.empty()) {
            return MView();
        }

        return downcast<View>(childs.back());
    }

    /**
     * レイアウトの構築方向
     */
    LayoutDirection_e directin;
public:
    LinearLayout() {
        directin = LayoutDirection_Vertical;
    }

    virtual ~LinearLayout() {
    }

    /**
     * 次のViewを登録する
     * 登録と同時にレイアウト位置を修正する
     */
    virtual void addSubView(MView layout, const Vector2f &margin) {
        layout->moveTo(nextHeader + margin);
        addSubView(layout, isRegisteredWindow());

        const Vector2f viewUseSize((layout->getLocalLayoutSize().x + margin.x), (layout->getLocalLayoutSize().y + margin.y));
        if (directin == LayoutDirection_Horizontal) {
            // X方向
            nextHeader.x += viewUseSize.x;
            localArea.right += viewUseSize.x;

            // 縦は最大値に合わせる
            if (viewUseSize.y > localArea.height()) {
                localArea.bottom = localArea.top + viewUseSize.y;
            }
        } else {
            // Y方向
            nextHeader.y += viewUseSize.x;
            localArea.right = jc::max(localArea.right, viewUseSize.x);

            // 横は最大値に合わせる
            if (viewUseSize.x > localArea.width()) {
                localArea.right = localArea.left + viewUseSize.x;
            }
        }

    }
};

typedef jc_sp<LinearLayout> MLinearLayout;

}
}

#endif /* LINEARLAYOUT_HPP_ */
