/*
 * RawImage.h
 *
 *  Created on: 2013/12/16
 */
#ifndef RAWIMAGE_H_
#define RAWIMAGE_H_

#include    "jointcoding.h"
#include    "jc/graphics/Pixel.h"
#include    "jc/collection/SafeArray.hpp"

namespace jc {

class RawImage: public Object {
    /**
     * 確保されたピクセルバッファ
     */
    safe_array<u8> pixels;

    /**
     * 直接アクセス用のバッファ
     */
    unsafe_array<u8> pBuffer;

    /**
     * ピクセルバッファフォーマット
     */
    PixelFormat_e format;

    /**
     * 画像幅
     */
    u32 width;

    /**
     * 画像高
     */
    u32 height;

    /**
     * y0行とy1行のピクセルを入れ替える
     */
    void swapLineY(const u32 y0, const u32 y1);
public:
    RawImage();

    virtual ~RawImage();

    /**
     * フォーマットを指定してピクセルバッファを生成する
     */
    virtual void alloc(const PixelFormat_e format, const u32 width, const u32 height);

    /**
     * ピクセルバッファの格納先を外部から指定する
     */
    virtual void setBuffer(const void* buffer, const s32 length);

    /**
     * 縦方向に反転する。
     * テクスチャからキャプチャした画素情報の天地反転用
     */
    void revertImageV();

    /**
     * ピクセルバッファの長さを取得する
     */
    u32 getPixelBufferLength() const {
        return pBuffer.length;
    }

    /**
     * ピクセルバッファのフォーマットを取得する
     */
    PixelFormat_e getPixelFormat() const {
        return format;
    }

    /**
     * 1ピクセルの容量を取得するう
     */
    u32 getPixelBytes() const {
        return Pixel::getPixelBytes(getPixelFormat());
    }

    void* getPixels() const {
        return pBuffer.ptr;
    }

    /**
     * 指定したY位置のピクセルバッファを取得する
     */
    void* getPixels(const u32 y) {
        return ((u8*) getPixels()) + (y * getWidth() * getPixelBytes());
    }

    /**
     * ピクセルバッファ幅を取得する
     */
    u32 getWidth() const {
        return width;
    }

    /**
     * ピクセルバッファ高を取得する
     */
    u32 getHeight() const {
        return height;
    }

};

/**
 * managed
 */
typedef typename jc_sp<RawImage> MRawImage;

}

#endif /* RAWIMAGE_H_ */
