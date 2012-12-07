/*
 * jcByteArrayOutputStream.h
 *
 *  Created on: 2012/07/25

 */

#ifndef JCBYTEARRAYOUTPUTSTREAM_H_
#define JCBYTEARRAYOUTPUTSTREAM_H_

#include    "jcMacro.h"
#include    "jcSmartPtr.h"
#include    "jcOutputStream.h"
#include    "jcMemoryBlock.h"

namespace jc {
class ByteArrayOutputStream: public OutputStream {
    /**
     * 一時バッファ
     */
    MMemoryBlock buffer;
public:
    /**
     * バッファの初期サイズを指定して作成する
     */
    ByteArrayOutputStream(const s32 bufferSize = 1024) {
        buffer.reset(new MemoryBlock(bufferSize));
    }
    virtual ~ByteArrayOutputStream() {
        buffer.reset();
    }

    /**
     * 指定バイト数のデータを書き込む
     */
    virtual void write(u8 *src, s32 length) {
        buffer->pushBack(src, length);
    }

    /**
     * ストリームを閉じる。
     * 実際は何もしない。
     */
    virtual void close() {
    }

    /**
     * バイト配列に変換する。
     */
    virtual MMemoryBlock getBuffer() const {
        return buffer;
    }
};

}

#endif /* JCBYTEARRAYOUTPUTSTREAM_H_ */
