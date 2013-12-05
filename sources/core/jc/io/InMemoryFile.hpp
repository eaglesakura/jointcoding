/*
 * InMemoryFile.hpp
 *
 *  Created on: 2013/12/05
 */
#ifndef INMEMORYFILE_HPP_
#define INMEMORYFILE_HPP_

#include    "jointcoding.h"
#include    "jc/collection/SafeArray.hpp"

namespace jc {

typedef unsafe_array<u8> byte_array;

/**
 * ファイルをシンプルに扱うためのクラス
 * メモリ空間にマッピング（load | mmap）するため、巨大なファイルは扱わない方がいい
 */
class InMemoryFile: public Object {
protected:
    InMemoryFile() {
    }
public:

    virtual ~InMemoryFile() {
    }

    /**
     * ファイルの長さを取得する
     */
    virtual s32 length() = 0;

    /**
     * 先頭ポインタを取得する
     */
    virtual u8* getHeadPointer() = 0;

    /**
     * アクセス用イテレータを返す
     */
    virtual byte_array iterator() {
        return byte_array(getHeadPointer(), length());
    }
};

}

#endif /* INMEMORYFILE_HPP_ */
