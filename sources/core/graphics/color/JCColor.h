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
struct Color {
    /**
     * 色管理用の構造体
     */
    union {
        /**
         * 1要素ごとの情報
         */
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        } tag;

        /**
         * RGBA情報
         */
        rgba32 rgba;
    } colors;

    Color() {
        RGBAi(255, 255, 255, 255);
    }

    Color(const Color &origin) {
        colors = origin.colors;
    }

    inline
    float rf() const {
        return (float) colors.tag.r / 255.0f;
    }

    inline
    float gf() const {
        return (float) colors.tag.g / 255.0f;
    }

    inline
    float bf() const {
        return (float) colors.tag.b / 255.0f;
    }

    inline
    float af() const {
        return (float) colors.tag.a / 255.0f;
    }

    inline u8 r() const {
        return colors.tag.r;
    }

    inline u8 g() const {
        return colors.tag.g;
    }

    inline u8 b() const {
        return colors.tag.b;
    }

    inline u8 a() const {
        return colors.tag.a;
    }
    /**
     * RGBAを0〜255で指定する
     */
    inline
    void RGBAi(u8 r, u8 g, u8 b, u8 a) {
        colors.tag.r = r;
        colors.tag.g = g;
        colors.tag.b = b;
        colors.tag.a = a;
    }

    /**
     * RGBAを0〜1.0fで指定する
     */
    inline
    void RGBAf(float r, float g, float b, float a) {
        colors.tag.r = (u8) (r * 255.0f);
        colors.tag.g = (u8) (g * 255.0f);
        colors.tag.b = (u8) (b * 255.0f);
        colors.tag.a = (u8) (a * 255.0f);
    }

    /**
     * RGBA色に変換する
     */
    inline rgba32 rgba() const {
        return colors.rgba;
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
        _argb.a = colors.tag.a;
        _argb.r = colors.tag.r;
        _argb.g = colors.tag.g;
        _argb.b = colors.tag.b;

        return (*((argb32*) &_argb));
    }

    /**
     * 同一ならtrue
     */
    inline
    bool operator==(const Color &col) const {
        return rgba() == col.rgba();
    }
    /**
     * 同一以外ならtrue
     */
    inline
    bool operator!=(const Color &col) const {
        return colors.rgba != col.colors.rgba;
    }

    /**
     * RGBA32形式へはキャストを提供する
     */
    inline operator rgba32() const {
        return colors.rgba;
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
};

}

#endif /* JCCOLOR_H_ */
