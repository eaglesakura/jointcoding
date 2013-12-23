/*
 * IFileMapper.hpp
 *
 *  Created on: 2013/12/06
 */
#ifndef IFILEMAPPER_HPP_
#define IFILEMAPPER_HPP_

#include    "jointcoding.h"
#include    "jc/collection/SafeArray.hpp"
#include    "jc/io/Uri.h"

namespace jc {

typedef unsafe_array<u8> byte_array;

/**
 * ファイルとメモリの関連付けを行う。
 * 実装をラクにするため、mmapではなく単純にメモリ内にコピーする場合がある。
 */
class IFileMapper {
protected:
    IFileMapper() {
    }
public:
    virtual ~IFileMapper() {
    }

    /**
     * 長さを取得する
     */
    virtual s32 length() = 0;

    /**
     * 頭のポインタを取得する
     */
    virtual u8* getHead() = 0;

    /**
     * 元ファイルのUriを取得する
     * 不明な場合はfalseを返す。
     */
    virtual jcboolean getUri(Uri *result) = 0;

    /**
     * 書き込みの反映を行う。
     * ただし、一切ファイルへの書き戻しをサポートしない場合がある。
     */
    virtual void flush() {
    }
};

typedef jc_sp<IFileMapper> MFileMapper;

}

#endif /* IFILEMAPPER_HPP_ */