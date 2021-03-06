/*
 * JpegStreamSource.cpp
 *
 *  Created on: 2012/05/13
 *      Author: Takeshi
 */
#include "jc/graphics/jpeg/JpegStreamSource.h"

namespace jc {

void init_source(jpeg_decompress_struct *cInfo) {
}

boolean fill_input_buffer(j_decompress_ptr cinfo) {

    JpegStreamSource *jStream = (JpegStreamSource*) cinfo->src;
    size_t nbytes;

    nbytes = jStream->stream->read(jStream->buffer.get(), jStream->bufferLength);

    // 読み込みに失敗したら
    if (nbytes <= 0) {
        WARNMS(cinfo, JWRN_JPEG_EOF);
        /* Insert a fake EOI marker */
        jStream->buffer[0] = (JOCTET) 0xFF;
        jStream->buffer[1] = (JOCTET) JPEG_EOI;
        nbytes = 2;
    }

    // 読み込んだ量を設定する
    jStream->pub.next_input_byte = jStream->buffer.get();
    jStream->pub.bytes_in_buffer = nbytes;

    return TRUE;
}

void skip_input_data(j_decompress_ptr cinfo, long num_bytes) {
    struct jpeg_source_mgr * src = cinfo->src;

    /* Just a dumb implementation for now.  Could use fseek() except
     * it doesn't work on pipes.  Not clear that being smart is worth
     * any trouble anyway --- large skips are infrequent.
     */
    if (num_bytes > 0) {
        while (num_bytes > (long) src->bytes_in_buffer) {
            num_bytes -= (long) src->bytes_in_buffer;
            (void) (*src->fill_input_buffer)(cinfo);
            /* note we assume that fill_input_buffer will never return FALSE,
             * so suspension need not be handled.
             */
        }
        src->next_input_byte += (size_t) num_bytes;
        src->bytes_in_buffer -= (size_t) num_bytes;
    }
}

void term_source(j_decompress_ptr cinfo) {
    JpegStreamSource *jStream = (JpegStreamSource*) cinfo->src;
    SAFE_DELETE(jStream);
}

void init(jpeg_decompress_struct *cInfo) {
//    JpegStreamSource *jStream = NULL;

}

/**
 * 通常のコンストラクタ
 */
JpegStreamSource::JpegStreamSource(InputStream *stream, s32 bufferLength) {
    this->stream = stream;
    this->bufferLength = bufferLength;
    this->buffer.reset(new u8[bufferLength]);

    pub.bytes_in_buffer = 0;
    pub.next_input_byte = NULL;

    // functions
    pub.init_source = init_source;
    pub.fill_input_buffer = fill_input_buffer;
    pub.skip_input_data = skip_input_data;
    pub.resync_to_restart = jpeg_resync_to_restart;
    pub.term_source = term_source;
}

JpegStreamSource* JpegStreamSource::initialize(jpeg_decompress_struct *cInfo, InputStream *stream, s32 bufferLength) {
    JpegStreamSource *result = new JpegStreamSource(stream, bufferLength);
    cInfo->src = (jpeg_source_mgr*) result;
    return result;
}

JpegStreamSource::~JpegStreamSource() {
    buffer.reset(NULL);
    jclog("delete stream source");
}

}
