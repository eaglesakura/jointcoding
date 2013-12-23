/*
 * ByteArrayInputStream.h
 *
 *  Created on: 2013/01/17
 */

#ifndef BYTEARRAYINPUTSTREAM_H_
#define BYTEARRAYINPUTSTREAM_H_

#include    "jc/io/IFileMapper.hpp"
#include    "jc/io/InputStream.h"

namespace jc {

class ByteArrayInputStream: public InputStream {
    jc_sa<u8> raw_array;
    MFileMapper file;

    s32 readable;
    u8* current_ptr;

public:

    ByteArrayInputStream( jc_sa<u8> raw_array, u32 length ) {
        this->raw_array = raw_array;
        this->readable = (s32)length;
        this->current_ptr = raw_array.get();
    }

    ByteArrayInputStream( jc_sa<u8> raw_array, const u32 begin, const u32 length) {
        this->raw_array = raw_array;
        this->readable = (s32)length;
        this->current_ptr = raw_array.get() + begin;
    }

    ByteArrayInputStream( MFileMapper file ) {
        this->file = file;
        this->readable = (s32)file->length();
        this->current_ptr = file->getHead();
    }

    ByteArrayInputStream( MFileMapper file, const u32 begin, const u32 length) {
        this->file = file;
        this->readable = length;
        this->current_ptr = file->getHead() + begin;
    }

    virtual ~ByteArrayInputStream() {

    }

    /**
     * 指定バイト数読み取る
     * @return 読み込んだバイト数
     */
    virtual s32 read(u8* result, s32 size) {
        size = min(size, readable);
//        jclogf(" read size(%d)", size);
        memcpy(result, current_ptr, size);
        readable -= size;
        current_ptr += size;

        return size;
    }

    /**
     * 指定バイト数読み取りヘッダを飛ばす
     */
    virtual s32 skip(s32 bytes) {
        readable -= bytes;
        current_ptr += bytes;
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
