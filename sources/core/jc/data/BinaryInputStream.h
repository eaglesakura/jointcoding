/*
 * BinaryInputStream.h
 *
 *  Created on: 2012/12/20
 */

#ifndef JCBINARYINPUTSTREAM_H_
#define JCBINARYINPUTSTREAM_H_

#include    "jc/io/InputStream.h"
#include    "jc/io/ByteArrayInputStream.h"
#include    "jc/math/Math.h"

namespace jc {

/**
 * バイナリデータの読み込みを行う。
 * データは全てリトルエンディアンで格納されることが前提。
 * BinaryOutputStreamと対になる。
 */
class BinaryInputStream: public Object {
    /**
     * 読み込み用ストリーム
     */
    MInputStream stream;
public:
    BinaryInputStream(MInputStream is) {
        this->stream = is;
    }

    BinaryInputStream(MFileMapper file) {
        stream.reset(mark_new ByteArrayInputStream(file));
    }

    virtual ~BinaryInputStream() {
    }

    /**
     * 1byte read
     */
    virtual s8 readS8();

    /**
     * 1byte read
     */
    virtual u8 readU8() {
        return static_cast<u8>(readS8());
    }

    /**
     * 2byte read
     */
    virtual s16 readS16();

    /**
     * 2byte read
     * Big Endian
     */
    virtual s16 readS16BE();

    /**
     * 2byte read
     */
    virtual u16 readU16() {
        return static_cast<u16>(readS16());
    }

    virtual u16 readU16BE() {
        return static_cast<u16>(readS16BE());
    }

    /**
     * 4byte read
     */
    virtual s32 readS32();

    /**
     * 4byte read
     */
    virtual u32 readU32() {
        return static_cast<u32>(readS32());
    }

    /**
     * 8byte read
     */
    virtual s64 readS64();

    /**
     * 8byte read
     */
    virtual u64 readU64() {
        return static_cast<u64>(readS64());
    }

    /**
     * 4byte fixed read
     */
    virtual float readFixed32(const s32 bits = FIXED_BIT_GL);

    /**
     * String read
     */
    virtual String readString();

    /**
     * 生データを読み込んで返す。
     * @param result_data データの格納先
     * @param result_data_length データの長さの格納先。NULL可
     * @return データの長さ
     */
    virtual u32 readByteArray(jc_sa<u8> *result_data, u32 *result_data_length );

    /**
     * 固定小数点として読み込んで返す。
     * @param result_data データの格納先
     * @param result_data_length データの長さの格納先。NULL可
     * @param bits 固定小数点のビット数
     */
    virtual u32 readFixed32Array(jc_sa<float> *result_data, u32 *result_data_length, const s32 bits = FIXED_BIT_GL );
};

/**
 * managed
 */
typedef jc_sp<BinaryInputStream> MBinaryInputStream;

}

#endif /* BINARYINPUTSTREAM_H_ */
