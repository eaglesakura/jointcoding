/*
 * InputStream.cpp
 *
 *  Created on: 2012/07/25

 */

#include    "jointcoding.h"
#include    "jc/io/InputStream.h"
#include    "jc/system/Log.h"

namespace jc {
/**
 * バイト配列に変換する。
 */ //
jc_sa<u8> InputStream::toByteArray(jc_sp<InputStream> is) {

    MMemoryBlock buffer(new MemoryBlock(is->available()));
    const u32 BUFFER_LENGTH = 1024 * 8;
    jc_sa<u8> temp(new u8[BUFFER_LENGTH]);
    s32 readed = 0;

    // 読み込めるだけ読み込む
    while ((readed = is->read(temp.get(), BUFFER_LENGTH)) > 0) {
        buffer->pushBack(temp.get(), readed);
    }

    jclogf("stream size = %d", buffer->getLength());

    // バッファを切り詰める
    buffer->compact();

    return buffer->getSharedBuffer();
}

/**
 * バイト配列に変換する。
 */ //
String InputStream::toText(MInputStream is) {
    const u32 BUFFER_LENGTH = 1024 * 8;
    jc_sa<u8> temp(new u8[BUFFER_LENGTH]);
    s32 readed = 0;
    MMemoryBlock buffer(new MemoryBlock(BUFFER_LENGTH));


    // 読み込めるだけ読み込む
    while ((readed = is->read(temp.get(), BUFFER_LENGTH)) > 0) {
        buffer->pushBack(temp.get(), readed);
    }

    is->close();
    jclogf("stream size = %d", buffer->getLength());

    // 1文字詰める
    buffer->pushBackZero();

    String result = (const charactor*) buffer->getSharedBuffer().get();
    return result;
}

}
