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
void LayoutParams::layout(RectF *result, const RectF parentLocal) {
    assert(result != NULL);

    RectF local;

    // 幅・高さを設定する
    {
        s32 width = layout_width;
        s32 height = layout_height;

        if (layout_width <= 0) {
            width = parentLocal.width();
        }

        if (layout_height <= 0) {
            height = parentLocal.width();
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
            local.moveToRight(parentLocal.width() - margin_right);
        } else {
            // 左寄せ
            local.moveToLeft(margin_left);
        }

        // Y方向チェック
        if (gravity & LayoutGravity_CenterY) {
            // 中央寄せ
            local.moveToCenterY(parentLocal.centerY());
        } else if (gravity & LayoutGravity_Bottom) {
            // 下寄せ
            local.moveToBottom(parentLocal.height() - margin_bottom);
        } else if (gravity & LayoutGravity_OnTop) {
            local.moveToBottom(parentLocal.top - margin_bottom);
        } else {
            // 上
            local.moveToTop(margin_top);
        }
    }

    // 位置を確定する
    *result = local;
}

}
}

