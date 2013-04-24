/*
 * Button.h
 *
 *  Created on: 2013/04/24
 */

#ifndef GLVIEW_BUTTON_H_
#define GLVIEW_BUTTON_H_

#include    "jc/widget/view/text/TextView.h"

namespace jc {
namespace view {

/**
 * ボタンのレイアウトを確定する
 * 基準はnormal時の画像となる
 */
enum ButtonLayout_e {
    /**
     * 画像をレイアウトの領域フルに利用する
     */
    ButtonLayout_Fill,

    /**
     * 画像をレイアウト領域フルで利用するかつアスペクト比に合わせて拡縮する
     */
    ButtonLayout_AspectFit,

    /**
     * オリジナルの大きさで表示する
     */
    ButtonLayout_Origin,
};

/**
 * ボタンの画像タイプ
 */
enum ButtonImageType_e {
    /**
     * フォーカス状態
     */
    ButtonImageType_Focus,

    /**
     * 押下状態
     */
    ButtonImageType_Down,

    /**
     *
     */
    ButtonImageType_Normal,
};

/**
 * ボタンを制御する
 *
 * ボタンは次の状態を持つ
 * normal:押されていない状態
 * focus:フォーカスが当たっている状態
 * down:押されている状態
 */
class Button: public TextView {
protected:
    /**
     * ボタン用画像
     */
    MTextureImage images[ButtonImageType_Normal + 1];

public:
    Button();
    virtual ~Button();

public:
    /* setter */

    /**
     * URIを指定してボタン画像を設定する
     */
    virtual void setImage(const ButtonImageType_e type, MDevice device, const Uri &uri, const PixelFormat_e pixelFormat = PixelFormat_NativeRGBA, TextureLoadOption *option = NULL);

    /**
     * テクスチャを指定してボタン画像を設定する
     */
    virtual void setImage(const ButtonImageType_e type, const MTextureImage texture);

public:
    /* layout */
    /**
     * 親レイアウトの位置を元に設定する
     */
    virtual void layoutFromImage(const ButtonLayout_e layoutType);

    /**
     * 親レイアウトの位置を元に設定する
     */
    virtual void layoutFromImage(const ButtonLayout_e layoutType, const Vector2f &parentLayoutSize);

    /**
     * コンテンツと同サイズにレイアウトし直す。
     */
    virtual void layoutWrap( const float marginX, const float marginY );

protected:
    /* override */

    /**
     * レンダリングを行う
     */
    virtual void onSelfRendering();
};

/**
 * managed
 */
typedef jc_sp<Button> MButton;

}
}

#endif /* BUTTON_H_ */
