/*
 * Sprite.h
 *
 *  Created on: 2013/02/28
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include    "jointcoding.h"
#include    "jc/math/Rect.h"
#include    "jc/gl/TextureImage.h"

namespace jc {
namespace gl {

enum SpritePosition_e {
    /**
     * X方向中央
     */
    SpritePosition_CenterX = 0x1 << 0,

    /**
     * Y方向中央
     */
    SpritePosition_CenterY = 0x1 << 1,

    /**
     * 右寄せ
     */
    SpritePosition_Right = 0x1 << 2,

    /**
     * 下寄せ
     */
    SpritePosition_Bottom = 0x1 << 2,

    /**
     * 上寄せ
     */
    SpritePosition_Top = 0,

    /**
     * 左寄せ
     */
    SpritePosition_Left = 0,

    /**
     * 中央
     */
    SpritePosition_Center = SpritePosition_CenterX | SpritePosition_CenterY,
};

class Sprite: public Object {

    /**
     * レンダリング対象テクスチャ
     */
    MTextureImage texture;

    /**
     * レンダリング色
     */
    Color color;

    /**
     * 画像のソース位置
     */
    RectF srcArea;

    /**
     * 画像のレンダリング位置
     */
    RectF dstArea;

    /**
     * スプライトの回転角
     */
    float rotate;

protected:
    MTextureImage getRenderingTexture() const {
        return texture;
    }
public:
    Sprite(MTextureImage texture) {
        this->texture = texture;
        rotate = 0;
        color.rgba = 0xFFFFFFFF;

        srcArea.setXYWH(0, 0, texture->getWidth(), texture->getHeight());
        dstArea = srcArea;
    }

    /**
     *
     */
    const RectF& getSrc() const {
        return srcArea;
    }

    /**
     *
     */
    const RectF& getDst() const {
        return dstArea;
    }

    /**
     * 描画領域の幅を取得する。
     * @return
     */
    float getDstWidth() {
        return dstArea.width();
    }

    /**
     * 描画領域の高さを取得する。
     * @return
     */
    float getDstHeight() {
        return dstArea.height();
    }

    float getDstLeft() {
        return dstArea.left;
    }

    float getDstRight() {
        return dstArea.right;
    }

    float getDstTop() {
        return dstArea.top;
    }

    float getDstBottom() {
        return dstArea.bottom;
    }

    float getDstCenterX() {
        return dstArea.centerX();
    }

    float getDstCenterY() {
        return dstArea.centerY();
    }

    /**
     * 描画位置を設定する。
     *
     * @param x
     * @param y
     * @param scaleX
     * @param scaleY
     * @param flags
     */
    Sprite* setSpritePosition(float x, float y, float dstWidth, float dstHeight, u32 SpritePosition_flags) {
        //! 横方向の補正を行う
        {
            if (SpritePosition_flags & SpritePosition_CenterX) {
                x -= (dstWidth / 2);
            } else if (SpritePosition_flags & SpritePosition_Right) {
                x -= dstWidth;
            }
        }

        //! 縦方向の補正を行う
        {
            if (SpritePosition_flags & SpritePosition_CenterY) {
                y -= (dstHeight / 2);
            } else if (SpritePosition_flags & SpritePosition_Bottom) {
                y -= (dstHeight);
            }
        }

        dstArea.setXYWH(x, y, dstWidth, dstHeight);

        return this;
    }

    /**
     * 指定したピクセル数、描画エリアを移動する。
     * @param x
     * @param y
     */
    Sprite* offsetSpritePosition(int x, int y) {
        dstArea.offset(x, y);
        return this;
    }

    /**
     * 描画色RGBAを設定する。
     *
     * @param color
     */
    Sprite* setColorRGBA(const Color color) {
        this->color = color;
        return this;
    }

    /**
     * 描画色RGBのみを設定する。
     * Aの値は保たれる。
     * @param colorRGBX
     */
    Sprite* setColorRGB(const Color colorRGBX) {
        u8 alpha = color.tag.a;
        color = colorRGBX;
        color.tag.a = alpha;
        return this;
    }

    /**
     * 色を指定した位置へ遷移させる
     * @param targetRGBA
     * @param offset
     */
    Sprite* moveColorRGBA(const Color targetRGBA, const u8 offset) {
        color = Color::moveColor(color, targetRGBA, offset);
        return this;
    }

    /**
     * αのみ遷移させる
     * RGBは固定される。
     * @param targetA
     * @param offset
     */
    Sprite* moveColorA(u32 targetA, u32 offset) {
        moveColorRGBA(Color::fromRGBAi(color.tag.r, color.tag.g, color.tag.b, targetA), offset);
        return this;
    }

    /**
     * RGBのみ遷移させる。
     * αは固定される。
     * @param targetRGB
     * @param offset
     */
    Sprite* moveColorRGB(Color targetRGB, u32 offset) {
        targetRGB.tag.a = color.tag.a;
        moveColorRGBA(targetRGB, offset);
        return this;
    }

    /**
     * 画像のαのみを変更する
     * @param alpha
     */
    Sprite* setColorA(u32 alpha) {
        color.tag.a = alpha;
        return this;
    }

    /**
     * 画像のアルファのみを変更する。
     * @param alpha
     */
    Sprite* setColorA(float alpha) {
        setColorA((u32) (alpha * 255));
        return this;
    }

    /**
     * スプライトの描画色を取得する
     */
    Color getColor() const {
        return color;
    }

    /**
     * 描画色RGBAを設定する。 値は0.0f〜1.0fである必要がある。
     *
     * @param r
     * @param g
     * @param b
     * @param a
     */
    Sprite* setColorRGBA(const float r, const float g, const float b, const float a) {
        color = Color::fromRGBAf(r, g, b, a);
        return this;
    }

    Sprite* setColor(Color color) {
        this->color = color;
        return this;
    }

};

typedef jc_sp<Sprite> MSprite;}
}

#endif /* SPRITE_H_ */
