/*
 * DebugFont.h
 *
 *  Created on: 2013/03/29
 */

#ifndef DEBUGFONT_H_
#define DEBUGFONT_H_

#include    "jc/gl/sprite/SpriteManager.h"
#include    "jc/gl/texture/FontTexture.h"
#include    "jc/math/Vec2.h"

namespace jc {
namespace gl {

class DebugFont: public Object {
public:
    enum {
        /**
         *
         */
        TEXTURE_NUM = 126 - 32,
    };
private:
    /**
     * スプライト管理クラス
     */
    MSpriteManager spriteManager;

    /**
     * フォント１文字分のサイズ
     */
    Vector2i fontCharSize;

    /**
     * フォントテクスチャ
     * １テクスチャ１文字で格納する
     */
    MFontTexture fonts[TEXTURE_NUM];

    /**
     * 利用するテクスチャユニット
     * 末尾のユニットを利用して、可能な限りユニットを汚染しないようにする。
     */
    u32 textureUnit;

    /**
     * 初期化を行う
     */
    void initialize(MDevice device);

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
    virtual void setColor(const u32 rgba) {
        fontColor.rgba = rgba;
    }

    /**
     * 文字色を設定する
     */
    virtual void setColor(const Color color) {
        fontColor = color;
    }

    /**
     * デバッグ用文字列を画面に表示する
     * レンダリング速度が低いため注意すること。
     */
    virtual void rendering(MDevice device, const String text, const s32 x, const s32 y);

    /**
     * レンダリングの幅・高さを計算する
     */
    virtual Vector2i calcRenderingSize(const String text);

    /**
     * レンダリングエリアを計算する
     */
    virtual RectI calcRenderingArea(const String text, const s32 x, const s32 y) {
        Vector2i size = calcRenderingSize(text);
        RectI result = createRectFromXYWH(x, y, size.x, size.y);
        return result;
    }
};

/**
 * managed
 */
typedef jc_sp<DebugFont> MDebugFont;

}
}

#endif /* DEBUGFONT_H_ */
