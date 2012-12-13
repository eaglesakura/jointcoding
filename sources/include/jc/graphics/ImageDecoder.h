/*
 * ImageDecoder.h
 *
 *  Created on: 2012/07/22

 */

#ifndef IMAGEDECODER_H_
#define IMAGEDECODER_H_

#include    "jointcoding.h"
#include    "jc/io/InputStream.h"
#include    "jc/graphics/Pixel.h"

namespace jc {

/**
 * 画像のデコードを行う抽象クラス。
 * 画像は逐次デコードを行う。
 * 現在対応しているのはlibjpegのみ。
 */
class ImageDecoder: public Object {
protected:
    /**
     * 画像へのストリーム
     */
    MInputStream stream;

    /**
     * 画像の幅
     */
    s32 width;

    /**
     * 画像の高さ
     */
    s32 height;

    /**
     * 記録するピクセルフォーマット
     */
    PixelFormat_e pixelFormat;

    /**
     * 読み込んだピクセル
     */
    jc_sa<u8> pixels;
public:
    ImageDecoder(MInputStream is) {
        this->stream = is;
        pixelFormat = PixelFormat_RGB888;
        width = 0;
        height = 0;
    }

    virtual ~ImageDecoder() {

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
     * ピクセルバッファを取得する
     */
    virtual jc_sa<u8> getPixels() {
        return pixels;
    }

    /**
     * ヘッダを読み込む。
     */
    virtual void decodeHeader() = 0;

    /**
     * デコードの開始を行う。
     * @param format 読み込むピクセルフォーマット
     * @param line 1回に読み込むライン数
     */
    virtual void decodeBegin( PixelFormat_e fomat, s32 line ) = 0;

    /**
     * 指定した数の水平ラインを取得する
     * 実際に読み込んだライン数を返す。
     * 0だったら読み込み終了
     */
    virtual s32 decode() = 0;

    /**
     * デコーダーの持っているリソースを開放する
     */
    virtual void dispose() = 0;
};

/**
 * managed
 */
typedef jc_sp<ImageDecoder> MImageDecoder;

}

#endif /* IMAGEDECODER_H_ */
