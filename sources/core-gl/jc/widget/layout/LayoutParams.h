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


    /**
     * 親Layoutの上に乗るように修正する
     */
    LayoutGravity_OnTop = 0x01 << 4,
}
;

class LayoutParams: public Object {
public:
/**
 * レイアウトのリクエスト幅
 * 負の値の場合、親Viewの幅が設定される
 */
float layout_width;

/**
 * レイアウトのリクエスト高さ
 * 負の値の場合、親Viewの高さが設定される
 */
float layout_height;

/**
 * 寄せ設定
 */
s32 gravity;

/**
 * 左からのマージン
 */
float margin_left;

/**
 * 右からのマージン
 */
float margin_right;

/**
 * 上からのマージン
 */
float margin_top;

/**
 * 下からのマージン
 */
float margin_bottom;

LayoutParams() {
    // 幅・高さリセット
    layout_width = layout_height = -1;
    // マージンリセット
    margin_left = margin_right = margin_top = margin_bottom = 0;
    gravity = 0x00000000;
}

virtual ~LayoutParams() {
}

/**
 * レイアウトの再配置を行う
 * @param result 結果の格納先
 * @param parentLocal 基準となるLocal座標
 */
virtual void layout(RectF *result, const RectF parentLocal);
};

}
}

#endif /* LAYOUTPARAMS_H_ */
