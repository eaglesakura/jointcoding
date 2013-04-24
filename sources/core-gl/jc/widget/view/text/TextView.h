/*
 * TextView.h
 *
 *  Created on: 2013/04/22
 */

#ifndef GLVIEW_TEXTVIEW_H_
#define GLVIEW_TEXTVIEW_H_

#include    "jc/widget/View.h"
#include    "jc/gl/texture/FontTexture.h"

namespace jc {
namespace view {

enum TextMultilineGravity_e {
    /**
     * テキストを左寄せする
     */
    TextMultilineGravity_Left = 0x1 << 0,

    /**
     * テキストを中央配置する
     */
    TextMultilineGravity_Center = 0x1 << 1,

    /**
     * テキストを右寄せする
     */
    TextMultilineGravity_Right = 0x1 << 2,
};

class TextView: public View {
protected:
    /**
     * エリアに対してテキストをどのように配置するか
     * LayoutGravity_eの組み合わせで表現する。
     */
    u32 textGravity;

    /**
     * 複数行配置
     */
    u32 multilineGravity;

    /**
     * 設定されているテキスト
     */
    String text;

    /**
     * テキストの高さ（ピクセル数）
     */
    u32 fontHeightPixel;

    /**
     * 改行ごとのマージン値
     */
    s32 lineMargin;

    /**
     * フォント色
     */
    Color fontColor;

    /**
     * 生成されたテクスチャリスト
     */
    std::list<MFontTexture> fontTextures;

    /**
     * テクスチャが生成済みならtrue
     */
    jcboolean textureCreated;

    /**
     * レンダリング用テクスチャを生成する
     */
    virtual void createTexture();

public:
    TextView();

    virtual ~TextView();

    /**
     * テキストを設定する
     */
    virtual void setText(const String &text);

    /**
     * テキストを取得する
     */
    virtual const String& getText() const {
        return text;
    }

    /**
     * フォントの高さを設定する
     */
    virtual void setTextHeightPixel(const u32 height);

    /**
     * フォントの高さを取得する
     */
    virtual u32 getTextHeightPixel() const {
        return fontHeightPixel;
    }

    /**
     * テキストレンダリング色を設定する
     */
    virtual void setTextColor(const Color color) {
        this->fontColor = color;
    }

    /**
     * テキストレンダリング色を取得する
     */
    virtual Color getTextColor() const {
        return fontColor;
    }

    /**
     * 改行ごとのマージン値を設定する
     */
    virtual void setLineMargin(const s32 margin) {
        lineMargin = margin;
    }

    /**
     * 改行マージンを取得する
     */
    virtual s32 getLineMargin() const {
        return lineMargin;
    }

    /**
     * フォントの描画領域の領域サイズを取得する
     */
    virtual Vector2f getFontAreaSize() const;

    /**
     * フォントの描画エリアを取得する
     */
    virtual RectF getFontRendringArea() const;

    /**
     * テキストの描画全体領域合わせを設定する
     */
    virtual void setTextGravity(const u32 LayoutGravity_flags) {
        textGravity = LayoutGravity_flags;
    }

    /**
     * テキスト位置合わせを設定する
     */
    virtual void setTextMultilineGravity(const u32 TextMultilineGravity_flags) {
        multilineGravity = TextMultilineGravity_flags;
    }
protected:
    /**
     * レンダリングを行う
     */
    virtual void onSelfRendering();

    /**
     * ウィンドウ登録時
     */
    virtual void onRegisteredWindow();
};

/**
 * managed
 */
typedef jc_sp<TextView> MTextView;

}
}

#endif /* TEXTVIEW_H_ */
