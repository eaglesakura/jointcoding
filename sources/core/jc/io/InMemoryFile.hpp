/*
 * InMemoryFile.hpp
 *
 *  Created on: 2013/12/05
 */
#ifndef INMEMORYFILE_HPP_
#define INMEMORYFILE_HPP_

#include    "jc/io/IFileMapper.hpp"

namespace jc {

/**
 * ファイルをシンプルに扱うためのクラス
 * 物理メモリにロードするため、巨大なファイルは扱わない方がいい
 */
class InMemoryFile: public Object, public IFileMapper {
protected:
    // 長さ
    s32 _length;
    // データ本体
    jc_sa<u8> data;
    // アクセスUri
    Uri uri;
public:
    InMemoryFile(jc_sa<u8> data, const u32 data_length) {
        this->data = data;
        this->_length = data_length;
    }

    virtual ~InMemoryFile() {
    }

    /**
     * 長さを取得する
     */
    virtual s32 length() {
        return _length;
    }

    /**
     * 頭のポインタを取得する
     */
    virtual u8* getHead() {
        return data.get();
    }

    virtual void setUri(const Uri set) {
        this->uri = set;
    }

    /**
     * 元ファイルのUriを取得する
     * 不明な場合はfalseを返す。
     */
    virtual jcboolean getUri(Uri *result) {
        if(uri.getUri().length()) {
            *result = uri;
            return jctrue;
        } else {
            return jcfalse;
        }
    }
};

}

#endif /* INMEMORYFILE_HPP_ */
