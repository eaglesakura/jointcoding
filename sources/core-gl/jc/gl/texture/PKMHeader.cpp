/*
 * PKMHeader.cpp
 *
 *  Created on: 2013/02/25
 */

#include    "jc/gl/PKMHeader.h"
#include    "jc/data/BinaryInputStream.h"
#include    "jc/mem/MemoryUtil.h"

namespace jc {
namespace gl {

PKMHeader::PKMHeader() {
    width = height = originalHeight = originalWidth = dataType = 0;
    zeromemory(version, 2);
}

PKMHeader::~PKMHeader() {

}

/**
 * ヘッダを作成する.
 * 読込に失敗した場合はNULLを返す。
 */ //
jc_sp<PKMHeader> PKMHeader::createInstance(MInputStream stream) {
    jc_sp<PKMHeader> result;

    {
        // check header
        charactor magic[4] = {0};
        stream->read((u8*)magic, 4);

        if(magic[0] != 'P' || magic[1] != 'K' || magic[2] != 'M') {
            return MPKMHeader();
        }
    }

    result.reset(new PKMHeader());
    {
        // check version
        stream->read((u8*)result->version, 2);

        if(result->version[0] != '1' || result->version[1] != '0') {
            return MPKMHeader();
        }
    }

    {
        BinaryInputStream binStream(stream);

        // read data type
        result->dataType = binStream.readU16BE();

        // read compressed width & height
        result->width = binStream.readU16BE();
        result->height = binStream.readU16BE();

        // read original width & height
        result->originalWidth = binStream.readU16BE();
        result->originalHeight = binStream.readU16BE();
    }

// texture size check
    assert(result->width >= result->originalWidth);
    assert(result->height >= result->originalHeight);

    return result;
}

}
}
