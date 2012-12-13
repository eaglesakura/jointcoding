/*
 * jcMemoryBlock.h
 *
 *  Created on: 2012/07/25

 */

#ifndef JCMEMORYBLOCK_H_
#define JCMEMORYBLOCK_H_

#include    "jcMacro.h"
#include    "jcObject.h"
#include    "jcSmartPtr.h"
#include    <vector>

namespace jc {

/**
 * メモリの塊を扱う。
 */
class MemoryBlock: public Object {
    /**
     * 確保したメモリブロック
     */
    jc_sa<u8> block;

    /**
     * 確保したブロックのサイズ
     */
    u32 block_size;

    /**
     * ブロックの長さ
     */
    u32 length;
public:
    MemoryBlock(const u32 bufferLength);

    virtual ~MemoryBlock();

    /**
     * メモリを後ろに追加する。
     */
    virtual void pushBack(const u8 *src, const u32 size);

    /**
     * 0x00のデータを1バイトだけ流しこむ。
     * テキストファイルのNULL終端用。
     */
    virtual void pushBackZero( ) {
        u8 zero = 0;
        pushBack(&zero, 1);
    }

    /**
     * バッファ本体を取得する。
     */
    virtual u8* getBuffer() const;

    /**
     * 共有用のバッファを取得する
     */
    virtual jc_sa<u8> getSharedBuffer() const {
        return block;
    }

    /**
     * 長さを取得する
     */
    virtual u32 getLength() const {
        return length;
    }

    /**
     * バッファサイズを必要最低限に切り詰める
     */
    virtual void compact();
};

/**
 * managed
 */
typedef jc_sp<MemoryBlock> MMemoryBlock;

}

#endif /* JCMEMORYBLOCK_H_ */
