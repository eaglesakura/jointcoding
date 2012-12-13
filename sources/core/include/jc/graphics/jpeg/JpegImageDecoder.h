/*
 * JpegImageDecoder.h
 *
 *  Created on: 2012/07/22

 */

#ifndef JPEGIMAGEDECODER_H_
#define JPEGIMAGEDECODER_H_

#include    "jcImageDecoder.h"
#include    "jcJpegStreamSource.h"

namespace jc {

class JpegImageDecoder: public ImageDecoder {
    jpeg_decompress_struct cInfo;
    jpeg_error_mgr eInfo;

    /*
     * JPEG読み込み関数テーブル
     */
    jc_sp<JpegStreamSource> source;

    /**
     * 一時的に読み込むピクセルバッファ
     */
    jc_sa<u8> tempPixelBuffer;

    /**
     * 一度に読み込むライン数
     */
    s32 onceLine;

    /**
     * 読み込み可能な残りライン
     */
    s32 lines;

    /**
     * 各行ごとの読み込み先ポインタ
     */
    jc_sa<JSAMPROW> rowPointers;
public:
    JpegImageDecoder(MInputStream is);
    virtual ~JpegImageDecoder();

    /**
     * ヘッダを読み込む。
     */
    virtual void decodeHeader();

    /**
     * デコードの開始を行う。
     * @param format 読み込むピクセルフォーマット
     * @param line 1回に読み込むライン数
     */
    virtual void decodeBegin( PixelFormat_e fomat, s32 line );

    /**
     * 指定した数の水平ラインをデコードしてピクセルバッファに格納する
     * 実際に読み込んだライン数を返す。
     * 0だったら読み込み終了
     */
    virtual s32 decode();

    /**
     * デコーダーの持っているリソースを開放する
     */
    virtual void dispose();

};

}

#endif /* JPEGIMAGEDECODER_H_ */
