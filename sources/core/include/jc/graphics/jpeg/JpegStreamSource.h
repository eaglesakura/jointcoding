/*
 * JpegStreamSource.h
 *
 *  Created on: 2012/05/13
 *      Author: Takeshi
 */

#ifndef JPEGSTREAMSOURCE_H_
#define JPEGSTREAMSOURCE_H_

#include "jointcoding.h"
#include "JCInputStream.h"
#include "jpeg/jpeglib.h"
#include "jpeg/jerror.h"

namespace jc {

struct JpegStreamSource {
    jpeg_source_mgr pub; /* public fields */

    /**
     * 読み込みストリーム
     */
    InputStream *stream;

    /**
     * 保持するバッファの長さ
     */
    s32 bufferLength;

    /**
     * 一時的に保持しておくバッファ
     */
    jc_sa<u8> buffer;

private:
    /**
     * 通常のコンストラクタ
     */
    JpegStreamSource(InputStream *stream, s32 bufferlength);
public:
    ~JpegStreamSource();

    static JpegStreamSource* initialize(jpeg_decompress_struct *cInfo, InputStream *stream, s32 bufferLength);
};

}
#endif /* JPEGSTREAMSOURCE_H_ */
