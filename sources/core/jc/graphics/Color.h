/*
 * JCColor.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCCOLOR_H_
#define JCCOLOR_H_

#include    "jc/math/Math.h"

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

    Color(const rgba32 color) {
        rgba = color;
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
     * 色の加算を行う
     */
    inline Color operator+(const Color &col) const {
        const u8 R = (u8) min<u32>(255, (s32) tag.r + (s32) col.tag.r);
        const u8 G = (u8) min<u32>(255, (s32) tag.g + (s32) col.tag.g);
        const u8 B = (u8) min<u32>(255, (s32) tag.b + (s32) col.tag.b);
        const u8 A = (u8) min<u32>(255, (s32) tag.a + (s32) col.tag.a);
        return fromRGBAi(R, G, B, A);
    }

    /**
     * 色の加算を行う
     */
    inline Color& operator+=(const Color &col) {
        tag.r = (u8) min<u32>(255, (s32) tag.r + (s32) col.tag.r);
        tag.g = (u8) min<u32>(255, (s32) tag.g + (s32) col.tag.g);
        tag.b = (u8) min<u32>(255, (s32) tag.b + (s32) col.tag.b);
        tag.a = (u8) min<u32>(255, (s32) tag.a + (s32) col.tag.a);
        return *this;
    }

    /**
     * 色の減算を行う
     */
    inline Color operator-(const Color &col) const {
        const u8 R = (u8) max<u32>(0, (s32) tag.r - (s32) col.tag.r);
        const u8 G = (u8) max<u32>(0, (s32) tag.g - (s32) col.tag.g);
        const u8 B = (u8) max<u32>(0, (s32) tag.b - (s32) col.tag.b);
        const u8 A = (u8) max<u32>(0, (s32) tag.a - (s32) col.tag.a);
        return fromRGBAi(R, G, B, A);
    }

    /**
     * 色の減算を行う
     */
    inline Color& operator-=(const Color &col) {
        tag.r = (u8) max<u32>(0, (s32) tag.r - (s32) col.tag.r);
        tag.g = (u8) max<u32>(0, (s32) tag.g - (s32) col.tag.g);
        tag.b = (u8) max<u32>(0, (s32) tag.b - (s32) col.tag.b);
        tag.a = (u8) max<u32>(0, (s32) tag.a - (s32) col.tag.a);
        return *this;
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
     * 色計算を行う。
     * dstColor * (1.0 - srcAlpha) + srcColor * srcAlpha
     */
    static Color blendColorSrcAlpha(const Color dstColor, const Color srcColor) {
        const float src_alpha = srcColor.af();
        const float dst_alpha = 1.0f - src_alpha;

        const float R = (dstColor.rf() * dst_alpha) + (srcColor.rf() * src_alpha);
        const float G = (dstColor.gf() * dst_alpha) + (srcColor.gf() * src_alpha);
        const float B = (dstColor.bf() * dst_alpha) + (srcColor.bf() * src_alpha);

        return fromRGBAf(jc::minmax<float>(0, 1, R), jc::minmax<float>(0, 1, G), jc::minmax<float>(0, 1, B), src_alpha);
    }

    /**
     * 色計算を行う。
     * dstColor * (1.0 - srcAlpha) + srcColor * srcAlpha
     */
    static Color blendColorSrcAlpha(const Color dstColor, const Color srcColor, const float srcAlpha) {
        const float dst_alpha = 1.0f - srcAlpha;

        const float R = (dstColor.rf() * dst_alpha) + (srcColor.rf() * srcAlpha);
        const float G = (dstColor.gf() * dst_alpha) + (srcColor.gf() * srcAlpha);
        const float B = (dstColor.bf() * dst_alpha) + (srcColor.bf() * srcAlpha);

        return fromRGBAf(jc::minmax<float>(0, 1, R), jc::minmax<float>(0, 1, G), jc::minmax<float>(0, 1, B), srcAlpha);
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
    static Color blendColor(const Color rgba0, const Color rgba1, float blend) {
        Color result;
        blend = jc::minmax<float>(0, 1.0f, blend);

        result.tag.r = (u8) ((((float) rgba1.tag.r) * blend) + (((float) rgba0.tag.r) * (1.0f - blend)));
        result.tag.g = (u8) ((((float) rgba1.tag.g) * blend) + (((float) rgba0.tag.g) * (1.0f - blend)));
        result.tag.b = (u8) ((((float) rgba1.tag.b) * blend) + (((float) rgba0.tag.b) * (1.0f - blend)));
        result.tag.a = (u8) ((((float) rgba1.tag.a) * blend) + (((float) rgba0.tag.a) * (1.0f - blend)));
        return result;
    }

    /**
     * 色を移動させて返す。
     * @param rgba0
     * @param rgba1
     * @param blend
     * @return
     */
    static Color moveColor(const Color now, const Color target, const u8 offset) {
        Color result;

        result.tag.r = targetMove(now.tag.r, target.tag.r, offset);
        result.tag.g = targetMove(now.tag.g, target.tag.g, offset);
        result.tag.b = targetMove(now.tag.b, target.tag.b, offset);
        result.tag.a = targetMove(now.tag.a, target.tag.a, offset);
        return result;
    }

    /**
     * α値を乗算する
     */
    inline Color multAlpha(const float a) const {
        return Color::fromRGBAi(tag.r, tag.g, tag.b, (u8) ((af() * a) * 255));
    }

    /**
     * 白を生成する
     */
    static Color white() {
        return fromRGBAi(0xFFFFFFFF);
    }

    /**
     * 黒を生成する
     */
    static Color black() {
        return fromRGBAi(0x000000FF);
    }
};

}

#endif /* JCCOLOR_H_ */
