/*
 * JCColor.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCCOLOR_H_
#define JCCOLOR_H_

namespace jc {

/**
 * RGBAで色情報を設定する
 */
typedef u32 rgba32;

/**
 * ARGBで色情報を設定する
 */
typedef u32 argb32;

/**
 * 色情報を扱う。
 * 色情報は一貫してRGBAの順番とする。
 * ライブラリ内部でnativeへ変換する。
 */
union Color {
    /**
     * 1要素ごとの情報
     * リトルエンディアン格納のため、並びは反転する
     */
    struct {
        u8 a;
        u8 b;
        u8 g;
        u8 r;
    } tag;

    /**
     * RGBA情報
     */
    rgba32 rgba;

    Color() {
        RGBAi(255, 255, 255, 255);
    }

    Color(const Color &origin) {
        rgba = origin.rgba;
    }

    inline
    float rf() const {
        return (float) tag.r / 255.0f;
    }

    inline
    float gf() const {
        return (float) tag.g / 255.0f;
    }

    inline
    float bf() const {
        return (float) tag.b / 255.0f;
    }

    inline
    float af() const {
        return (float) tag.a / 255.0f;
    }

    inline u8 r() const {
        return tag.r;
    }

    inline u8 g() const {
        return tag.g;
    }

    inline u8 b() const {
        return tag.b;
    }

    inline u8 a() const {
        return tag.a;
    }
    /**
     * RGBAを0〜255で指定する
     */
    inline
    void RGBAi(const u8 r, const u8 g, const u8 b, const u8 a) {
        tag.r = r;
        tag.g = g;
        tag.b = b;
        tag.a = a;
    }

    /**
     * RGBAを0〜1.0fで指定する
     */
    inline
    void RGBAf(const float r, const float g, const float b, const float a) {
        tag.r = (u8) (r * 255.0f);
        tag.g = (u8) (g * 255.0f);
        tag.b = (u8) (b * 255.0f);
        tag.a = (u8) (a * 255.0f);
    }

    /**
     * ARGB色に変換する
     */
    inline argb32 argb() const {
        struct {
            u8 a;
            u8 r;
            u8 g;
            u8 b;
        } _argb;
        _argb.a = tag.a;
        _argb.r = tag.r;
        _argb.g = tag.g;
        _argb.b = tag.b;

        return (*((argb32*) &_argb));
    }

    /**
     * 同一ならtrue
     */
    inline
    bool operator==(const Color &col) const {
        return rgba == col.rgba;
    }
    /**
     * 同一以外ならtrue
     */
    inline
    bool operator!=(const Color &col) const {
        return rgba != col.rgba;
    }

    /**
     * RGBA32形式へはキャストを提供する
     */
    inline operator rgba32() const {
        return rgba;
    }

    inline static Color fromRGBAf(float r, float g, float b, float a) {
        Color result;
        result.RGBAf(r, g, b, a);
        return result;
    }

    inline static Color fromRGBAi(const u8 r, const u8 g, const u8 b, const u8 a) {
        Color result;
        result.RGBAi(r, g, b, a);
        return result;
    }

    /**
     * intから色情報へ変換する
     */
    inline static Color fromRGBAi(const rgba32 rgba) {
        Color result;
        result.rgba = rgba;
        return result;
    }

    /**
     * 色をブレンドして返す。
     * 遷移は次の通り
     * blend = 0.0f -> rgba0
     * blend = 1.0f -> rgba1
     * @param rgba0
     * @param rgba1
     * @param blend
     * @return
     */
    static Color blendColor(const Color rgba0, const Color rgba1, const float blend) {
        Color result;

        result.tag.r = (u8) minmax<s32>(0, 255, (s32) (((float) rgba0.tag.r * blend) + ((float) rgba1.tag.r * (1.0f - blend))));
        result.tag.g = (u8) minmax<s32>(0, 255, (s32) (((float) rgba0.tag.g * blend) + ((float) rgba1.tag.g * (1.0f - blend))));
        result.tag.b = (u8) minmax<s32>(0, 255, (s32) (((float) rgba0.tag.b * blend) + ((float) rgba1.tag.b * (1.0f - blend))));
        result.tag.a = (u8) minmax<s32>(0, 255, (s32) (((float) rgba0.tag.a * blend) + ((float) rgba1.tag.a * (1.0f - blend))));

        return result;
    }
};

}

#endif /* JCCOLOR_H_ */
