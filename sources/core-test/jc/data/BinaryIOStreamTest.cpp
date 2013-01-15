/*
 * BinaryIOStreamTest.cpp
 *
 *  Created on: 2012/12/20
 */

#include    "jointcoding.h"
#include    "jointcoding-testsupport.h"
#include    "jc/data/BinaryInputStream.h"
#include    "jc/data/BinaryOutputStream.h"
#include    "jc/math/Math.h"

#include    "jc/io/FileInputStream.h"
#include    "jc/io/FileOutputStream.h"

#define FILE_NAME "test_bin_io.dat"

using namespace jc;

/**
 * BinaryInput/OutputStreamを利用してファイル読み書きを行い、正しいデータが入出力出来る事を確認する。
 */
void test_binaryOutput() {
    const s8 s8_data = 1;
    const s16 s16_data = 0xFF00 | 2;
    const s32 s32_data = 0xFFEEDD | 4;
    const s64 s64_data = 0xFF00EE00DD00CC | 8;
    const String string_data = "じゅげむじゅげむABCDごこうの012すりきれ";

    const u8 byte_array_data[] = { 0xFF, 0xEE, 0xDD, 0xCC, 0xAB, 0x01 };
    const u32 byte_array_data_length = sizeof(byte_array_data);

    const float float_data = 0.5f;
    const float float_array_data[] = { 0.12f, 1.23f, 2.34f, 3.45f, 4.56f, 5.67f };
    const u32 float_array_data_length = sizeof(float_array_data) / sizeof(float);

    {
        jcboolean complete;
        MOutputStream os(new FileOutputStream(FILE_NAME, &complete));

        _assertTrue(complete);

        jc_sp<BinaryOutputStream> stream(new BinaryOutputStream(os));

        stream->writeS8(s8_data);
        stream->writeS16(s16_data);
        stream->writeS32(s32_data);
        stream->writeS64(s64_data);
        stream->writeString(string_data);
        stream->writeByteArray(byte_array_data, byte_array_data_length);
        stream->writeFixed32(float_data);
        stream->writeFixed32Array(float_array_data, float_array_data_length);
    }

    {
        jcboolean complete;
        MInputStream is(new FileInputStream(FILE_NAME, &complete));
        _assertTrue(complete);

        jc_sp<BinaryInputStream> stream(new BinaryInputStream(is));

        {
            s8 data = stream->readS8();
            _assertEquals(data, s8_data);
        }
        {
            s16 data = stream->readS16();
            _assertEquals(data, s16_data);
        }
        {
            s32 data = stream->readS32();
            _assertEquals(data, s32_data);
        }
        {
            s64 data = stream->readS64();
            _assertEquals(data, s64_data);
        }
        {
            String data = stream->readString();
//            jclogf("readed data = %s", data.c_str());
            _assertEquals(data, string_data);
        }
        {
            jc_sa<u8> data;
            u32 data_length;
            stream->readByteArray(&data, &data_length);

            _assertEquals(data_length, data_length);
            for( u32 i = 0; i < data_length; ++i ) {
                _assertEquals(data[i], byte_array_data[i]);
            }
        }
        {
            float data = stream->readFixed32();
            jclogf(" readFixed32 %f -> %f", float_data, data);
            real2fixed<float, fixed32>(0, 0);

            const fixed32 before = real2fixed<float, fixed32>(float_data, FIXED_BIT_GL);
            const fixed32 after = real2fixed<float, fixed32>(data, FIXED_BIT_GL);
            _assertEquals( before, after);
//            _assertTrue(::jc::equals(data, float_data));
        }
        {
            jc_sa<float> data;
            u32 data_length;
            stream->readFixed32Array(&data, &data_length);

            _assertEquals(data_length, float_array_data_length);
            for( u32 i = 0; i < data_length; ++i ) {
                jclogf(" readFixed32 %f -> %f", float_array_data[i], data[i]);

                const fixed32 before = real2fixed<float, fixed32>(float_array_data[i], FIXED_BIT_GL);
                const fixed32 after = real2fixed<float, fixed32>(data[i], FIXED_BIT_GL);

                _assertEquals( before, after);
//                _assertTrue(::jc::equals(data[i], float_array_data[i]));
            }
        }
    }
}
