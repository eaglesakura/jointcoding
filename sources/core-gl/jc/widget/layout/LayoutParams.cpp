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
void LayoutParams::layout(RectF *result, const Vector2f &parentLocalSize) {
    assert(result != NULL);

    RectF local;

    // 幅・高さを設定する
    {
        s32 width = layout_width;
        s32 height = layout_height;

        if (layout_width <= 0) {
            width = parentLocalSize.x;
        }

        if (layout_height <= 0) {
            height = parentLocalSize.y;
        }
        local.right = width;
        local.bottom = height;
    }

    // 重心合わせを行う
    {
        // X方向チェック
        if (gravity & LayoutGravity_CenterX) {
            // 中央寄せ
            local.moveToCenterX(parentLocalSize.x / 2);
        } else if (gravity & LayoutGravity_Right) {
            // 右寄せ
            local.moveToRight(parentLocalSize.x - margin_right);
        } else {
            // 左寄せ
            local.moveToLeft(margin_left);
        }

        // Y方向チェック
        if (gravity & LayoutGravity_CenterY) {
            // 中央寄せ
            local.moveToCenterY(parentLocalSize.y / 2);
        } else if (gravity & LayoutGravity_Bottom) {
            // 下寄せ
            local.moveToBottom(parentLocalSize.y - margin_bottom);
        } else if (gravity & LayoutGravity_OnTop) {
            // 親Layoutの上に配置
            local.moveToBottom(-margin_bottom);
        } else if (gravity & LayoutGravity_ToUnder) {
            // 親Layoutの下に配置
            local.moveToTop(parentLocalSize.y + margin_top);
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

