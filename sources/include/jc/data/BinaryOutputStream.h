/*
 * BinaryOutputStream.h
 *
 *  Created on: 2012/12/19
 */

#ifndef JCBINARYOUTPUTSTREAM_H_
#define JCBINARYOUTPUTSTREAM_H_

#include    "jointcoding.h"
#include    "jc/io/OutputStream.h"
#include    "jc/math/Math.h"

namespace jc {

/**
 * バイナリデータの書き出しを行う。
 * 書き込んだデータは全てリトルエンディアンで格納される。
 * BinaryInputStreamと対になる。
 */
class BinaryOutputStream: public Object {
    /**
     * 書き込み用の実ストリーム
     */
    MOutputStream stream;
public:
    BinaryOutputStream(MOutputStream os);
    virtual ~BinaryOutputStream();

    /**
     * 8bit整数を書き出す
     */
    virtual void writeS8(const s8 data);

    /**
     * 8bit整数を書き出す
     */
    virtual void writeU8(const u8 data) {
        writeS8(static_cast<s8>(data));
    }

    /**
     * 16bit整数を書き出す
     */
    virtual void writeS16(const s16 data);

    /**
     * 16bit整数を書き出す
     */
    virtual void writeU16(const u16 data) {
        writeS16(static_cast<s16>(data));
    }

    /**
     * 32bit整数を書き出す
     */
    virtual void writeS32(const s32 data);

    /**
     * 32bit整数を書き出す
     */
    virtual void writeU32(const u32 data) {
        writeS32(static_cast<s32>(data));
    }

    /**
     * 64bit整数を書き出す
     */
    virtual void writeS64(const s64 data);

    /**
     * 64bit整数を書き出す
     */
    virtual void writeU64(const u64 data) {
        writeS64(static_cast<s64>(data));
    }

    /**
     * 固定小数として書き出す。
     */
    virtual void writeFixed32(const float data, const s32 bits = FIXED_BIT_GL);

    /**
     * 文字列として書き出す。
     * 頭2byteに文字数（＋NULL文字）、続く n byteに文字＋NULL終端が書き込まれる
     * | (u16)data.char_length + 1 | (charactor)string + NULL |
     */
    virtual void writeString(const String data);

    /**
     * データを直接書き込む。
     * | s32 bytes | data array ... |
     */
    virtual void writeByteArray(const void* data, const u32 bytes);

    /**
     * データを書き込む。
     * float -> fixed変換を行なって書き込む。
     * | s32 bytes | fixed data array ... |
     */
    virtual void writeFixed32Array(const float *data, const u32 data_length, const s32 bits = FIXED_BIT_GL);
};

/**
 * managed
 */
typedef jc_sp<BinaryOutputStream> MBinaryOutputStream;

}

#endif /* BINARYOUTPUTSTREAM_H_ */
