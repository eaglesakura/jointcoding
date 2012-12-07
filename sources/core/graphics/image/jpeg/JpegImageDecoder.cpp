/*
 * JpegImageDecoder.cpp
 *
 *  Created on: 2012/07/22

 */
#include    "jcException.h"
#include    "jcJpegImageDecoder.h"
#include    "jcMath.h"
#include "oss/jpeg/jpeglib.h"
#include "oss/jpeg/jerror.h"

namespace jc {

namespace {

static void _jpeg_error_handle(j_common_ptr cinfo) {
    cinfo->err->msg_code = 1;
}

}

JpegImageDecoder::JpegImageDecoder(MInputStream is) :
        ImageDecoder(is) {
    lines = 0;
    onceLine = 0;
    this->cInfo.err = jpeg_std_error(&eInfo);
    this->eInfo.error_exit = _jpeg_error_handle;

    jpeg_create_decompress(&cInfo);
    source.reset(JpegStreamSource::initialize(&cInfo, is.get(), 1024 * 32));
}

JpegImageDecoder::~JpegImageDecoder() {
    this->dispose();
}

/**
 * ヘッダを読み込む。
 */
void JpegImageDecoder::decodeHeader() {
    jpeg_read_header(&cInfo, TRUE);

    if (eInfo.msg_code == 1) {
        throw create_exception_mt(JpegDecodeException, JpegDecodeException_HeaderDecodeFailed, "eInfo.msg_code == 1");
    }

    // 幅と高さを記録する
    width = cInfo.image_width;
    height = cInfo.image_height;
    lines = height;

    if (height == 0 || width == 0) {
        throw create_exception_mt(JpegDecodeException, JpegDecodeException_HeaderDecodeFailed, "height == 0 || width == 0");
    }

    jclogf("jpeg decode header complete. size = %d x %d", getWidth(), getHeight());
}

/**
 * デコードの開始を行う。
 * @param format 読み込むピクセルフォーマット
 * @param line 1回に読み込むライン数
 */
void JpegImageDecoder::decodeBegin(PixelFormat_e format, s32 line) {
    // ピクセルフォーマットを設定する
    this->pixelFormat = format;

    onceLine = min(line, getHeight());
    s32 tempPixels = onceLine * getWidth();

    // ピクセルバッファを用意する
    this->pixels = Pixel::createPixelBuffer(format, tempPixels);

    // 一時的な読み込みポインタを作成する
    if (format != PixelFormat_RGB888) {
        this->tempPixelBuffer = Pixel::createPixelBuffer(PixelFormat_RGB888, tempPixels);
    } else {
        this->tempPixelBuffer = pixels;
    }

    // 各行ごとの読み込みポインタを指定する
    rowPointers.reset(new JSAMPROW[onceLine]);

    for (int i = 0; i < onceLine; ++i) {
        u8* head = (tempPixelBuffer.get() + (width * i * 3));
        rowPointers[i] = head;
    }

    jpeg_start_decompress(&cInfo);
    if (eInfo.msg_code == 1) {
        throw create_exception_t(JpegDecodeException, JpegDecodeException_PixelDecodeStartFailed);
    }
}

/**
 * 指定した数の水平ラインを取得する
 * 実際に読み込んだライン数を返す。
 * 0だったら読み込み終了
 */
s32 JpegImageDecoder::decode() {
    // 読み込み可能なラインがなければ何もしない
    if (!lines) {
        return 0;
    }

    // オーバーランしないように読み込み数を制限する
    onceLine = min(lines, onceLine);

    // スキャンラインを行う
    {
        // 読みこませる最終位置
        const s32 readEnd = cInfo.output_scanline + onceLine;
        JSAMPROW *rows = rowPointers.get();
        while (cInfo.output_scanline < readEnd) {
            s32 start = cInfo.output_scanline;
            if (!jpeg_read_scanlines(&cInfo, rows, readEnd - start)) {
                throw create_exception_t(JpegDecodeException, JpegDecodeException_PixelDecodeingFailed);
            }

            // 実際に読み込めたラインを数える
            s32 moveLine = cInfo.output_scanline - start;
            // 読み込めたライン分を進める
            rows += moveLine;
        }

    }
    // メモリの転送を行う
    {
        Pixel::copyRGB888Pixels(tempPixelBuffer.get(), pixelFormat, pixels.get(), onceLine * width);
    }

    // 残りラインを制御する
    lines -= onceLine;

    if (!lines) {
        jclog("jpeg decode finish");
        // ラインを読み終わったら開放する
        this->dispose();
    }

    return onceLine;
}

/**
 * デコーダーの持っているリソースを開放する
 */
void JpegImageDecoder::dispose() {
    if (source) {
        source.reset();
//        jpeg_finish_decompress(&cInfo);
        jpeg_destroy_decompress(&cInfo);
        rowPointers.reset();
//        pixels.reset();
        tempPixelBuffer.reset();
        stream.reset();
    }
    lines = 0;
}

}

