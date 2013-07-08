/*
 * BinaryOutputStream.cpp
 *
 *  Created on: 2012/12/19
 */

#include    "jc/data/BinaryOutputStream.h"
#include    "jc/math/Math.h"

namespace jc {

BinaryOutputStream::BinaryOutputStream(MOutputStream os) {
    this->stream = os;
}

BinaryOutputStream::~BinaryOutputStream() {

}

/**
 * 8bit整数を書き出す
 */
void BinaryOutputStream::writeS8(const s8 data) {
    stream->write((u8*) &data, sizeof(data));
}

/**
 * 16bit整数を書き出す
 */
void BinaryOutputStream::writeS16(const s16 data) {
    stream->write((u8*) &data, sizeof(data));
}

/**
 * 32bit整数を書き出す
 */
void BinaryOutputStream::writeS32(const s32 data) {
    stream->write((u8*) &data, sizeof(data));
}

/**
 * 64bit整数を書き出す
 */
void BinaryOutputStream::writeS64(const s64 data) {
    stream->write((u8*) &data, sizeof(data));
}
/**
 * 固定小数として書き出す。
 *
 */
void BinaryOutputStream::writeFixed32(const float data, const s32 bits) {
    writeS32(real2fixed<float, s32>(data, bits));
}

/**
 * 文字列として書き出す。
 * 頭2byteに文字数（＋NULL文字）、続くn byteに文字＋NULL終端が書き込まれる
 * | (u16)data.char_length + 1 | (charactor)string + NULL |
 */
void BinaryOutputStream::writeString(const String data) {
    const u16 data_lenght = (data.length() + 1);
    writeU16(data_lenght);
    stream->write((u8*) data.c_str(), data_lenght);
}

/**
 * データを直接書き込む。
 * | s32 bytes | data array ... |
 */
void BinaryOutputStream::writeByteArray(const void* data, const u32 bytes) {
    writeU32(bytes);
    stream->write((const u8*) data, bytes);
}

/**
 * データを直接、長さ保持せずに書き込む
 * | data array ... |
 */
void BinaryOutputStream::writeRawByteArray(const void* data, const u32 bytes) {
    stream->write((const u8*) data, bytes);
}

/**
 * データを書き込む。
 * float -> fixed変換を行なって書き込む。
 */
void BinaryOutputStream::writeFixed32Array(const float *data, const u32 data_length, const s32 bits) {
    fixed32 *data_array = new s32[data_length];

    // 全てfixed変換を行う
    for (int i = 0; i < data_length; ++i) {
        data_array[i] = real2fixed<float, fixed32>(data[i], bits);
    }

    writeU32(data_length);
    stream->write((u8*) data_array, sizeof(fixed32) * data_length);

    SAFE_DELETE_ARRAY(data_array);
}
}
