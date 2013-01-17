/*
 * ByteArrayInputStream.h
 *
 *  Created on: 2013/01/17
 */

#ifndef BYTEARRAYINPUTSTREAM_H_
#define BYTEARRAYINPUTSTREAM_H_

#include    "jc/io/InputStream.h"

namespace jc {

class ByteArrayInputStream: public InputStream {
    jc_sa<u8> raw_array;

    s32 head_position;
    s32 buffer_length;
    s32 readable;

    u8* current_ptr() {
        return raw_array.get() + head_position + (buffer_length - readable);
    }
public:

    ByteArrayInputStream( jc_sa<u8> raw_array, u32 length ) {
        this->raw_array = raw_array;
        this->readable = (s32)length;
        this->head_position = 0;
        this->buffer_length = length;
    }

    ByteArrayInputStream( jc_sa<u8> raw_array, u32 begin, u32 end) {
        this->raw_array = raw_array;
        this->readable = (s32)buffer_length;
        this->head_position = begin;
        this->buffer_length = end - begin;
    }

    /**
     * 指定バイト数読み取る
     * @return 読み込んだバイト数
     */
    virtual s32 read(u8* result, s32 size) {
        size = min(size, readable);
        memcpy(result, current_ptr(), size);
        readable -= size;

        return size;
    }

    /**
     * 指定バイト数読み取りヘッダを飛ばす
     */
    virtual s32 skip(s32 bytes) {
        readable -= bytes;
        return bytes;
    }

    /**
     * 読み取り可能な残容量を取得する。
     */
    virtual s32 available() {
        return readable;
    }

    /**
     * ストリームを閉じる。
     * この呼出以降、ストリームは無効になる。
     */
    virtual void close() {
    }
};

}

#endif /* BYTEARRAYINPUTSTREAM_H_ */
