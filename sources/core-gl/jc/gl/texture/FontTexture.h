/*
 * FontTexture.h
 *
 *  Created on: 2013/02/19
 */

#ifndef FONTTEXTURE_H_
#define FONTTEXTURE_H_

#include    "jc/gl/texture/TextureImage.h"

namespace jc {
namespace gl {

class FontTexture: public TextureImage {
    /**
     * 記述されているテキスト
     */
    String text;

protected:
    FontTexture(const u32 width, const u32 height, MDevice device, const String &text);
public:
    virtual ~FontTexture();

    /**
     * テクスチャとして作成されているテキストを取得する。
     */
    const String& getText() const {
        return text;
    }

    /**
     * フォントテクスチャを作成する
     */
    static jc_sp<FontTexture> createInstance(const String &text, const u32 height, MDevice device);
};

/**
 * managed
 */
typedef jc_sp<FontTexture> MFontTexture;

}
}

#endif /* FONTTEXTURE_H_ */
