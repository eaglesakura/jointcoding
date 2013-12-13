/*
 * PixelBuffer.hpp
 *
 *  Created on: 2013/05/06
 */
#ifndef __PIXELBUFFER_HPP__
#define __PIXELBUFFER_HPP__

#include    "jointcoding.h"
#include    "jc/graphics/Pixel.h"

namespace jc {
namespace gl {

/**
 * 単純なピクセル配列を定義したクラス
 *
 * 基本的には継承し、Nativeクラスを保持する=Pixel変換を最小限にするように実装する
 */
class PixelBuffer: public Object {
protected:
    /**
     * 格納されているフォーマット
     */
    PixelFormat_e pixelFormat;

    /**
     * １ピクセルごとのバイト数
     */
    s32 bytesParPixel;

    /**
     * ピクセル配列
     */
    jc_sa<u8> pixels;

    /**
     * 読込ヘッダ
     */
    u8* header;

    /**
     * 幅
     */
    s32 width;

    /**
     * 高さ
     */
    s32 height;
public:
    PixelBuffer( const PixelFormat_e pixelFormat, const s32 width, const s32 height ) {
        assert(width > 0);
        assert(height > 0);

        this->pixelFormat = pixelFormat;
        this->width = width;
        this->height = height;
        bytesParPixel = Pixel::getPixelBytes(pixelFormat);
        header = NULL;
    }

    virtual ~PixelBuffer() {
    }

    /**
     * 読み込みヘッダを取得する
     */
    virtual GLvoid* getPixelHeader() const {
        return (GLvoid*)header;
    }

    /**
     * 次のラインに移動する
     */
    virtual void nextLine(const s32 skipPixelLines) {
        header += ( width * skipPixelLines ) * bytesParPixel;
    }

    /**
     * ピクセルフォーマットを取得する
     */
    PixelFormat_e getPixelFormat() const {
        return pixelFormat;
    }

    /**
     * 幅を取得する
     */
    virtual s32 getWidth() const {
        return width;
    }

    /**
     * 高さを取得する
     */
    virtual s32 getHeight() const {
        return height;
    }

    /**
     * ピクセルフォーマットを別フォーマットに変更する
     */
    virtual void convert(const PixelFormat_e newFormat) {
        // 同一フォーマットの場合は何もしない
        if(pixelFormat == newFormat) {
            return;
        }

        jclogf("convert pixelformat(%d) -> (%d)", pixelFormat, newFormat);
        bytesParPixel = Pixel::getPixelBytes(newFormat);
        jc_sa<u8> newBuffer = Pixel::createPixelBuffer(newFormat, width * height);

        // 旧フォーマットをチェックし、新フォーマットに変換する
        switch(pixelFormat) {
            case PixelFormat_RGBA8888:
            Pixel::copyRGBA8888Pixels(header, newFormat, newBuffer.get(), width * height);
            break;
            case PixelFormat_RGB888:
            Pixel::copyRGB888Pixels(header, newFormat, newBuffer.get(), width * height);
            break;
            case PixelFormat_NativeRGBA:
#if defined(BUILD_Android)
            // AndroidはBGRA -> 新フォーマット
            Pixel::copyBGRA8888Pixels(header, newFormat, newBuffer.get(), width * height);
#endif
#if defined(BUILD_iOS)
            // iOSはRGBA -> 新フォーマット
            Pixel::copyRGBA8888Pixels(header, newFormat, newBuffer.get(), width * height);
#endif
            break;
            default:
            // その他のフォーマットは相互変換出来ない
            jclogf("not support format(%d)", newFormat);
            assert(false);
            break;
        }

        pixelFormat = newFormat;
        bytesParPixel = Pixel::getPixelBytes(newFormat);
        pixels = newBuffer;
        header = newBuffer.get();
    }
};

/**
 * managed
 */
typedef jc_sp<PixelBuffer> MPixelBuffer;

}
}

#endif  //__PIXELBUFFER_HPP__
