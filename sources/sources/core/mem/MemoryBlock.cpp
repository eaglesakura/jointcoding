/*
 * MemoryBlock.cpp
 *
 *  Created on: 2012/07/25

 */

#include    "jc/mem/MemoryBlock.h"
#include    "jc/system/Log.h"

namespace jc {

MemoryBlock::MemoryBlock(const u32 bufferLength) {
    this->block.reset(new u8[bufferLength + 1]);
    this->block_size = bufferLength + 1;
    this->length = 0;
}

MemoryBlock::~MemoryBlock() {

}

namespace {

static jc_sa<u8> request(const jc_sa<u8> buffer, const u32 buffer_size, u32 *new_buffersize, const u32 request_size ) {
    // バッファが足りている
    if( buffer_size >= request_size ) {
        (*new_buffersize) = buffer_size;
        return buffer;
    }

    // バッファが足りない
    (*new_buffersize) = request_size + 1024;
    jc_sa< u8 > result( new u8[(*new_buffersize)]);

    // バッファをコピーする
    memcpy(result.get(), buffer.get(), buffer_size );

    return result;
}

}

/**
 * メモリを後ろに追加する。
 */
void MemoryBlock::pushBack(const u8 *src, const u32 size) {
    block = request(block, block_size, &block_size, (size + length));

    // コピー
    u8* head = block.get() + length;
    memcpy((void*)head, src, size);

    // サイズを足し込み
    length += size;
}

/**
 * バッファサイズを必要最低限に切り詰める
 */
void MemoryBlock::compact() {
    if (getLength() == block_size) {
        // ちょうどのサイズだから必要ない
        return;
    }

    jc_sa<u8> nBuffer(new u8[getLength()]);
    memcpy(nBuffer.get(), this->block.get(), getLength());

    this->block = nBuffer;
}

/**
 * バッファ本体を取得する。
 */
u8* MemoryBlock::getBuffer() const {
    return (u8*) &(block[0]);
}

}

