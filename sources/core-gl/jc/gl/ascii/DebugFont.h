/*
 * DebugFont.h
 *
 *  Created on: 2013/03/29
 */

#ifndef DEBUGFONT_H_
#define DEBUGFONT_H_

#include    "jc/gl/SpriteManager.h"
#include    "jc/gl/FontTexture.h"
#include    "jc/math/Vec2.h"

namespace jc {
namespace gl {

class DebugFont: public Object {
    /**
     * スプライト管理クラス
     */
    MSpriteManager spriteManager;

    /**
     * フォント１文字分のサイズ
     */
    Vector2i fontCharSize;

    /**
     * レンダリングステート
     */
    MGLState state;

    /**
     * レンダリングデバイス
     */
    MDevice device;

    /**
     * フォントテクスチャ
     * １テクスチャ１文字で格納する
     */
    MFontTexture fonts[126 - 32];

    /**
     * 利用するテクスチャユニット
     * 末尾のユニットを利用して、可能な限りユニットを汚染しないようにする。
     */
    u32 textureUnit;

    /**
     * 初期化を行う
     */
    void initialize();

    /**
     * フォント色
     */
    Color fontColor;

public:
    /**
     * デバッグ用のフォントを作成する
     */
    DebugFont(const u32 fontTextureHeight, MDevice device, MSpriteManager spriteManager);

    /**
     *
     */
    virtual ~DebugFont();

    /**
     * 文字色を設定する
     */
    void setColor(const u32 rgba) {
        fontColor.rgba = rgba;
    }

    /**
     * 文字色を設定する
     */
    void setColor(const Color color) {
        fontColor = color;
    }

    /**
     * デバッグ用文字列を画面に表示する
     * レンダリング速度が低いため注意すること。
     */
    void rendering(const String text, const s32 x, const s32 y);
};

/**
 * managed
 */
typedef jc_sp<DebugFont> MDebugFont;

}
}

#endif /* DEBUGFONT_H_ */
