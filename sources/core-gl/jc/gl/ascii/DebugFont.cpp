/*
 * DebugFont.cpp
 *
 *  Created on: 2013/03/29
 */

#include    "jc/gl/ascii/DebugFont.h"
#include    "jc/gl/gpu/GPUCapacity.h"

namespace jc {
namespace gl {

/**
 * デバッグ用のフォントを作成する
 */
DebugFont::DebugFont(const u32 fontTextureHeight, MDevice device, MSpriteManager spriteManager) {
    this->fontCharSize.y = fontTextureHeight;
    this->spriteManager = spriteManager;
    this->textureUnit = GPUCapacity::getMaxTextureUnits() - 1;
    this->fontColor = Color::fromRGBAi(255, 0, 0, 255);
    this->initialize(device);
}

/**
 *
 */
DebugFont::~DebugFont() {

}

const u32 FONT_INDEX_HEAD = 32;
const u32 FONT_INDEX_NUM = 126 - 32;

/**
 * 初期化を行う
 */
void DebugFont::initialize(MDevice device) {
    for (u32 i = 0; i < FONT_INDEX_NUM; ++i) {
        u8 str[2] = { (u8) (i + FONT_INDEX_HEAD), 0x00 };
        fonts[i] = FontTexture::createInstance((charactor*) str, fontCharSize.y, device);
        assert(this->fonts[i].get() != NULL);
        fontCharSize.x = jc::max<s32>(fontCharSize.x, fonts[i]->getWidth());
    }
}

/**
 * デバッグ用文字列を画面に表示する
 * レンダリング速度が低いため注意すること。
 */
void DebugFont::rendering(MDevice device, const String text, const s32 x, const s32 y) {

    const charactor* font_charactors = text.c_str();
    const s32 text_length = text.length();

    s32 current_x = x;
    s32 current_y = y;

    for (u32 i = 0; i < text_length; ++i) {
        s8 font_index = (s8) font_charactors[i] - FONT_INDEX_HEAD;

        if (font_charactors[i] == ' ') {
            current_x += fontCharSize.x;
        } else if (font_charactors[i] == '\n') {
            current_x = x;
            current_y += fontCharSize.y;
        } else if (font_index >= 0 && font_index < FONT_INDEX_NUM) {
            MTextureImage tex = fonts[font_index];
            // 所定のユニットにバインドしておく
            tex->bind(textureUnit, device->getState());
            spriteManager->renderingImage(tex, current_x, current_y, fontColor);
            current_x += fontCharSize.x;
        }
    }
}

/**
 * レンダリングの幅・高さを計算する
 */
Vector2i DebugFont::calcRenderingSize(const String text) {
    const charactor* font_charactors = text.c_str();
    const s32 text_length = text.length();

    s32 current_x = 0;
    s32 current_y = 0;

    s32 max_width = 0;
    for (u32 i = 0; i < text_length; ++i) {
        s8 font_index = (s8) font_charactors[i] - FONT_INDEX_HEAD;
        if (font_charactors[i] == ' ') {
            current_x += fontCharSize.x;
        } else if (font_charactors[i] == '\n') {
            current_x = 0;
            current_y += fontCharSize.y;
        } else if (font_index >= 0 && font_index < FONT_INDEX_NUM) {
            current_x += fontCharSize.x;
        }

        max_width = jc::max(max_width, current_x);
    }

    // 最後が改行以外だったらもう一行分ずらす
    if(font_charactors[text_length - 1] != '\n'){
        current_y += fontCharSize.y;
    }

    return Vector2i(max_width, current_y);
}

}
}
