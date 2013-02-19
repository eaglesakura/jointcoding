/*
 * jcPixel.h
 *
 *  Created on: 2012/07/29
 */

#ifndef JCPIXEL_H_
#define JCPIXEL_H_

#include    "jc/system/Macro.h"

/**
 * ピクセルフォーマットについての定義を設定する
 */
namespace jc {

/**
 * ピクセルフォーマットを指定する
 */
enum PixelFormat_e {
    /**
     * 2byte
     * GL_UNSIGNED_SHORT_5_6_5
     */
    PixelFormat_RGB565,

    /**
     * 2byte
     * GL_UNSIGNED_SHORT_5_5_5_1
     */
    PixelFormat_RGBA5551,

    /**
     * 3byte
     * GL_UNSIGNED_BYTE
     */
    PixelFormat_RGB888,

    /**
     * 4byte
     * GL_UNSIGNED_BYTE
     */
    PixelFormat_RGBA8888,
};

class Pixel {
public:

    /**
     * フォーマットごとの1ピクセルサイズを取得する
     */
    static inline const u32 getPixelBytes(const PixelFormat_e fmt) {
        const u32 size[] = {
        //
                2,// RGB565
                2, // RGBA5551
                3, // RGB888
                4, // RGBA8888
                };
        return size[fmt];
    }

    /**
     * 指定したピクセルフォーマットで規定のピクセル数分のバッファを作成する
     */
    static jc_sa<u8> createPixelBuffer(const PixelFormat_e fmt, const s32 pixels) {
        s32 bytes = getPixelBytes(fmt);
        return jc_sa < u8 > (new u8[bytes * pixels]);
    }

    /**
     * RGB888のポインタをdstへピクセル情報をコピーする。
     * 同じポインタの場合は、何も行わない。
     */
    inline
    static void copyRGB888Pixels(const u8 *src_rgb888, const PixelFormat_e dstFormat, u8 *dst, const s32 _pixels) {
        if (src_rgb888 == dst) {
            return;
        }

        // 残ピクセル数
        s32 pixels = _pixels;

        switch (dstFormat) {
            case PixelFormat_RGB565: {
                u16 *p = (u16*) dst;
                while (pixels) {

                    s32 r = src_rgb888[0] & 0xff;
                    s32 g = src_rgb888[1] & 0xff;
                    s32 b = src_rgb888[2] & 0xff;

                    (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                    src_rgb888 += 3;
                    ++p;
                    --pixels;
                }
            }
                break;
            case PixelFormat_RGBA5551: {
                u16 *p = (u16*) dst;
                while (pixels) {

                    s32 r = src_rgb888[0] & 0xff;
                    s32 g = src_rgb888[1] & 0xff;
                    s32 b = src_rgb888[2] & 0xff;
                    (*p) = ((r >> 3) << 11) | ((g >> 3) << 5) | ((b >> 3) << 1) | 1;
                    src_rgb888 += 3;
                    ++p;
                    --pixels;
                }
            }
                break;
            case PixelFormat_RGB888: {
                memcpy(dst, src_rgb888, pixels * 3);
            }
                break;
            case PixelFormat_RGBA8888: {
                while (pixels) {

                    dst[0] = src_rgb888[0];
                    dst[1] = src_rgb888[1];
                    dst[2] = src_rgb888[2];
                    dst[3] = 0xFF;

                    src_rgb888 += 3;
                    dst += 4;
                    --pixels;
                }
            }
                break;
        }
    }

    /**
     * RGB888のポインタをdstへピクセル情報をコピーする。
     * 同じポインタの場合は、何も行わない。
     */
    inline
    static void copyRGBA8888Pixels(const u8 *src_rgba8888, const PixelFormat_e dstFormat, u8 *dst, const s32 _pixels) {
        if (src_rgba8888 == dst) {
            return;
        }

        // 残ピクセル数
        s32 pixels = _pixels;

        const int pixel_size = 4;

        switch (dstFormat) {
            case PixelFormat_RGB565: {
                u16 *p = (u16*) dst;
                while (pixels) {

                    const s32 r = src_rgba8888[0] & 0xff;
                    const s32 g = src_rgba8888[1] & 0xff;
                    const s32 b = src_rgba8888[2] & 0xff;

                    (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                    src_rgba8888 += pixel_size;
                    ++p;
                    --pixels;
                }
            }
                break;
            case PixelFormat_RGBA5551: {
                u16 *p = (u16*) dst;
                while (pixels) {

                    const s32 r = src_rgba8888[0] & 0xff;
                    const s32 g = src_rgba8888[1] & 0xff;
                    const s32 b = src_rgba8888[2] & 0xff;
                    const s32 a = (src_rgba8888[3] & 0xff) > 0 ? 1 : 0;
                    (*p) = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | a;
                    src_rgba8888 += pixel_size;
                    ++p;
                    --pixels;
                }
            }
                break;
            case PixelFormat_RGB888: {
                while (pixels) {

                    dst[0] = src_rgba8888[0];
                    dst[1] = src_rgba8888[1];
                    dst[2] = src_rgba8888[2];

                    src_rgba8888 += pixel_size;
                    dst += 3;
                    --pixels;
                }
            }
                break;
            case PixelFormat_RGBA8888: {
                memcpy(dst, src_rgba8888, pixels * pixel_size);
            }
                break;
        }
    }
};

}

#endif /* JCPIXEL_H_ */
