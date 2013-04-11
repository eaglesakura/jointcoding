/*
 * LayoutParams.cpp
 *
 *  Created on: 2013/04/11
 */

#include    "jc/widget/layout/LayoutParams.h"

namespace jc {
namespace view {

/**
 * レイアウトの再配置を行う
 */
void LayoutParams::layout(View *parent, View *target) {
    assert(parent != NULL);
    assert(target != NULL);

    RectF parentLocal = parent->getLocalLayoutArea();
    RectF local;

    // 幅・高さを設定する
    {
        s32 width = layout_width;
        s32 height = layout_height;

        if (layout_width <= 0) {
            width = parentLocal.width();
            width -= (mergin_left + mergin_right);
        }

        if (layout_height <= 0) {
            height = parentLocal.width();
            height -= (mergin_top + mergin_bottom);
        }
        local.right = width;
        local.bottom = height;
    }

    // 重心合わせを行う
    {
        // X方向チェック
        if (gravity & LayoutGravity_CenterX) {
            // 中央寄せ
            local.moveToCenterX(parentLocal.centerX());
        } else if (gravity & LayoutGravity_Right) {
            // 右寄せ
            local.moveToRight(parentLocal.width() - mergin_right);
        } else {
            // 左寄せ
            local.moveToLeft(mergin_left);
        }

        // Y方向チェック
        if (gravity & LayoutGravity_CenterY) {
            // 中央寄せ
            local.moveToCenterY(parentLocal.centerY());
        } else if (gravity & LayoutGravity_Bottom) {
            // 下寄せ
            local.moveToBottom(parentLocal.height() - mergin_bottom);
        } else {
            // 上
            local.moveToTop(mergin_top);
        }
    }

    // 位置を確定する
    target->layout(local);
}

}
}

