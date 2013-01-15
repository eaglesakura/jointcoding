/*
 * BinaryInputStream.cpp
 *
 *  Created on: 2012/12/20
 */
#include    "jc/mem/MemoryUtil.h"
#include    "jc/data/BinaryInputStream.h"

namespace jc {

/**
 * 1byte read
 */
s8 BinaryInputStream::readS8() {
    s8 data = 0;
    stream->read((u8*) &data, sizeof(data));
    return data;
}

/**
 * 2byte read
 */
s16 BinaryInputStream::readS16() {
    s16 data = 0;
    stream->read((u8*) &data, sizeof(data));
    return data;
}

/**
 * 4byte read
 */
s32 BinaryInputStream::readS32() {
    s32 data = 0;
    stream->read((u8*) &data, sizeof(data));
    return data;
}

/**
 * 8byte read
 */
s64 BinaryInputStream::readS64() {
    s64 data = 0;
    stream->read((u8*) &data, sizeof(data));
    return data;
}

/**
 * 4byte fixed read
 */
float BinaryInputStream::readFixed32(const s32 bits) {
    fixed32 data = readS32();
    return fixed2real<fixed32, float>(data, bits);
}

/**
 * String read
 */
String BinaryInputStream::readString() {
    u16 length = readU16();
    jc_sa<charactor> temp( new charactor[length] );
    jc::zeromemory(temp.get(), length);

    stream->read((u8*) temp.get(), length);

    return String(temp.get());
}

/**
 * 生データを読み込んで返す。
 * @param result_data データの格納先
 * @param result_data_length データの長さの格納先。NULL可
 * @return データの長さ
 */
u32 BinaryInputStream::readByteArray(jc_sa<u8> *result_data, u32 *result_data_length ) {
    u32 length = readU32();
    u8* pData = new u8[length];

    // 指定された長さ分だけ読み込む
        stream->read(pData, length);

        // データを書き戻す
        result_data->reset(pData);

        // len?
        if(result_data_length) {
            (*result_data_length) = length;
        }

        return length;
    }

/**
 * 固定小数点として読み込んで返す。
 * @param result_data データの格納先
 * @param result_data_length データの長さの格納先。NULL可
 * @param bits 固定小数点のビット数
 */
u32 BinaryInputStream::readFixed32Array(jc_sa<float> *result_data, u32 *result_data_length, const s32 bits ) {
    u32 length = readU32();
    fixed32* pData = new fixed32[length];

    // 指定された長さ分だけ読み込む
    stream->read((u8*)pData, sizeof(fixed32) * length);

    // 戻り
    float *pResult = new float[length];
    for(u32 i = 0; i < length; ++i ) {
        pResult[i] = fixed2real<fixed32, float>( pData[i], bits );
    }

    // データを書き戻す
    result_data->reset(pResult);

    // len?
    if(result_data_length) {
        (*result_data_length) = length;
    }

    SAFE_DELETE_ARRAY(pData);
    return length;

}

}

