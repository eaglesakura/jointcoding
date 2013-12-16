/*
 * RawImage.cpp
 *
 *  Created on: 2013/12/16
 */

#include    "jc/graphics/RawImage.h"

namespace jc {

RawImage::RawImage() {
    format = PixelFormat_NativeRGBA;
    width = height = 0;
}

RawImage::~RawImage() {
}

/**
 * フォーマットを指定してピクセルバッファを生成する
 */
void RawImage::alloc(const PixelFormat_e format, const u32 width, const u32 height) {
    this->format = format;
    this->width = width;
    this->height = height;
    // ピクセルバッファの取得を行う
    {
        const s32 size = getPixelBytes() * width * height;
        if (this->pixels.length < size) {
            this->pixels.alloc(size);
        }
        pBuffer = pixels.iterator();
    }
}

/**
 * y0行とy1行のピクセルを入れ替える
 */
void RawImage::swapLineY(const u32 y0, const u32 y1) {

}

/**
 * 縦方向に反転する。
 * テクスチャからキャプチャした画素情報の天地反転用
 */
void RawImage::revertImageV() {

}

}
