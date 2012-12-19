/*
 * BinaryOutputStream.h
 *
 *  Created on: 2012/12/19
 */

#ifndef JCBINARYOUTPUTSTREAM_H_
#define JCBINARYOUTPUTSTREAM_H_

#include    "jc/io/OutputStream.h"

namespace jc {

enum {
    /**
     * OpenGL ESで利用する固定小数値
     */
    FIXED_BIT_GL = 16,

    /**
     * MascotCapsuleで利用する固定小数値
     */
    FIXED_BIT_MC = 12,
};

/**
 * バイナリデータの書き出しを行う。
 * 書き込んだデータは全てリトルエンディアンで格納される。
 */
class BinaryOutputStream {
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
        writeS8(const_cast<s8>(data));
    }

    /**
     * 16bit整数を書き出す
     */
    virtual void writeS16(const s16 data);

    /**
     * 16bit整数を書き出す
     */
    virtual void writeU16(const u16 data) {
        writeS16(const_cast<s16>(data));
    }

    /**
     * 32bit整数を書き出す
     */
    virtual void writeS32(const s32 data);

    /**
     * 32bit整数を書き出す
     */
    virtual void writeU32(const u32 data) {
        writeS32(const_cast<s32>(data));
    }

    /**
     * 64bit整数を書き出す
     */
    virtual void writeS64(const s64 data);

    /**
     * 64bit整数を書き出す
     */
    virtual void writeU64(const u64 data) {
        writeS64(const_cast<s64>(data));
    }

    /**
     * 固定小数として書き出す。
     *
     */
    virtual void writeFixed(const float data, const s32 bits = FIXED_BIT_GL);

    /**
     * 文字列として書き出す。
     * 頭2byteに文字数（＋NULL文字）、続くn byteに文字＋NULL終端が書き込まれる
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
     */
    virtual void writeFloatArray(const float *data, const u32 data_length, const s32 bits = FIXED_BIT_GL);
};

}

#endif /* BINARYOUTPUTSTREAM_H_ */
