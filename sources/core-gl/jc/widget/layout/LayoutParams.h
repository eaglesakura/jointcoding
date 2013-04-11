/*
 * LayoutParams.h
 *
 *  Created on: 2013/04/11
 */

#ifndef LAYOUTPARAMS_H_
#define LAYOUTPARAMS_H_

#include    "jc/widget/View.h"

namespace jc {
namespace view {

enum LayoutGravity_e {
    /**
     * 左寄せを行う
     */
    LayoutGravity_Left = 0,

    /**
     * 上寄せを行う
     */
    LayoutGravity_Top = 0,

    /**
     * 右寄せを行う
     */
    LayoutGravity_Right = 0x01 << 0,

    /**
     * 下寄せを行う
     */
    LayoutGravity_Bottom = 0x01 << 1,

    /**
     * 中央寄せを行う
     */
    LayoutGravity_CenterX = 0x01 << 2,

    /**
     * 中央寄せを行う
     */
    LayoutGravity_CenterY = 0x01 << 3,

    /**
     * 中央寄せを行う
     */
    LayoutGravity_Center = (0x01 << 2) | (0x01 << 3),
};

class LayoutParams: public Object {
public:
    /**
     * レイアウトのリクエスト幅
     * 負の値の場合、親Viewの幅が設定される
     */
    s32 layout_width;

    /**
     * レイアウトのリクエスト高さ
     * 負の値の場合、親Viewの高さが設定される
     */
    s32 layout_height;

    /**
     * 寄せ設定
     */
    s32 gravity;

    /**
     * 左からのマージン
     */
    s32 mergin_left;

    /**
     * 右からのマージン
     */
    s32 mergin_right;

    /**
     * 上からのマージン
     */
    s32 mergin_top;

    /**
     * 下からのマージン
     */
    s32 mergin_bottom;

    LayoutParams() {
        // 幅・高さリセット
        layout_width = layout_height = -1;
        // マージンリセット
        mergin_left = mergin_right = mergin_top = mergin_bottom = 0;
        gravity = 0x00000000;
    }

    virtual ~LayoutParams() {
    }

    /**
     * レイアウトの再配置を行う
     */
    virtual void layout(View *parent, View *target);
};

}
}

#endif /* LAYOUTPARAMS_H_ */
