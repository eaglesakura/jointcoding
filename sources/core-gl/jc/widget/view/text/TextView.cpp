/*
 * TextView.cpp
 *
 *  Created on: 2013/04/22
 */

#include    "jc/widget/view/text/TextView.h"
#include    "jc/widget/layout/LayoutParams.h"

namespace jc {
namespace view {

TextView::TextView(MWindowContext context) : View(context) {
    lineMargin = 0;
    textureCreated = jcfalse;
    textGravity = LayoutGravity_Left | LayoutGravity_Top;
    multilineGravity = TextMultilineGravity_Left;
    fontHeightPixel = 24;

    // テキストはRead-Onlyのため、アクションは起こせない
    setFocusable(jcfalse);
    setTouchable(jcfalse);
}

TextView::~TextView() {

}
/**
 * コンテンツと同サイズにレイアウトし直す。
 */
void TextView::layoutWrap() {
    const Vector2f textSize = getFontAreaSize();
    layoutDirect(createRectFromXYWH(localArea.left, localArea.top, textSize.x, textSize.y));
}

/**
 * レンダリング用テクスチャを生成する
 */
void TextView::createTexture(MDevice device) {
    assert(device);

    if (textureCreated) {
        // テクスチャ生成済み
        return;
    }

    // 既存のフォントを削除する
    fontTextures.clear();

    // テキストが空なら何もしない
    if (text.empty()) {
        return;
    }

    jclogf("create texture y(%d) [%s]", getTextHeightPixel(), text.c_str());

    // 改行で分割する
    std::vector<String, StlAllocator<String> > lines = text.split("\n");

    std::vector<String, StlAllocator<String> >::const_iterator itr = lines.begin(), end = lines.end();

    while (itr != end) {
        // フォントを生成する
        MFontTexture font = FontTexture::createInstance(*itr, getTextHeightPixel(), device);
        fontTextures.push_back(font);
        ++itr;
    }

    textureCreated = jctrue;

    // 領域が設定されてないなら再設定
    if (localArea.empty()) {
        layoutWrap();
    }
}

/**
 * テキストを設定する
 */
void TextView::setText(const String &text) {
    setText(getDeviceOrNull(), text);
}

/**
 * テキストを設定する
 */
void TextView::setText(MDevice device, const String text) {
    if (this->text == text) {
        // 同一テキストなら何もしない
        return;
    }

    this->text = text;

    textureCreated = jcfalse;
    if (device) {
        createTexture(device);
    }
}

/**
 * フォントの高さを設定する
 */
void TextView::setTextHeightPixel(const u32 height) {
    setTextHeightPixel(getDeviceOrNull(), height);
}

/**
 * フォントの高さを設定する
 */
void TextView::setTextHeightPixel(MDevice device, const u32 height) {
    if (fontHeightPixel == height) {
        return;
    }

    fontHeightPixel = height;

    // テクスチャを再生成する
    textureCreated = jcfalse;

    if (device) {
        createTexture(device);
    }
}

/**
 * ウィンドウ登録時
 */
void TextView::onRegisteredWindow() {
    // レンダリング用テクスチャを生成する
    createTexture(getDevice());
}

/**
 * フォントの描画領域の領域サイズを取得する
 */
Vector2f TextView::getFontAreaSize() const {
    std::list<MFontTexture>::const_iterator itr = fontTextures.begin(), end = fontTextures.end();

    Vector2f result;
    s32 index = 0;
    while (itr != end) {
        result.x = jc::max<float>((*itr)->getWidth(), result.x);

        result.y += (*itr)->getHeight();
        if (index > 0) {
            result.y += getLineMargin();
        }

        ++itr;
        ++index;
    }

    return result;
}

/**
 * フォントの描画エリアを取得する
 */
RectF TextView::getFontRendringArea() const {

    // 全体エリア
    RectF selfArea = getGlobalLayoutArea();

    // フォント描画に必要なサイズ
    Vector2f textSize = getFontAreaSize();

    // 戻り
    RectF result;

    LayoutParams params;
    params.layout_width = textSize.x;
    params.layout_height = textSize.y;
    params.gravity = textGravity;

    params.layout(&result, selfArea);

    // 内部に収める
    result.offset(selfArea.left, selfArea.top);
    return result;
}

/**
 * レンダリングを行う
 */
void TextView::onSelfRendering() {
    if (!textureCreated) {
        return;
    }

    MSpriteManager spriteManager = getSpriteManager();

    // テキスト全体の描画エリア
    const RectF textArea = getFontRendringArea();
#if 0
    // エリアをデバッグ描画
    spriteManager->renderingArea(textArea, 0xFF00007F, 0x0000FFFF);
    spriteManager->renderingArea(getGlobalLayoutArea(), 0x00FF007F, 0xFF00FFFF);
#endif

    float y = textArea.top;
    const float AREA_WIDTH = textArea.width();
    // 全テクスチャの描画
    std::list<MFontTexture>::const_iterator itr = fontTextures.begin(), end = fontTextures.end();

    const Color color = getTextColorWithVisible();
    while (itr != end) {
        MFontTexture font = *itr;

        if (multilineGravity & TextMultilineGravity_Left) {
            // 左寄せ描画
            spriteManager->renderingImage(font, textArea.left, y, color);
        } else if (multilineGravity & TextMultilineGravity_Center) {
            // 中央寄せ描画
            const float offset_x = (AREA_WIDTH - font->getWidth()) / 2;
            spriteManager->renderingImage(font, textArea.left + offset_x, y, color);
        } else {
            // 左寄せ描画
            const float offset_x = (AREA_WIDTH - font->getWidth());
            spriteManager->renderingImage(font, textArea.left + offset_x, y, color);
        }

        // 次のY位置に移動
        y += (font->getHeight() + lineMargin);
        ++itr;
    }
}

}
}

