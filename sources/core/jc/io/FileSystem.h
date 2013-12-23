/*
 * IFileSystem.h
 *
 *  Created on: 2012/05/20
 *      Author: Takeshi
 */

#ifndef IFILESYSTEM_H_
#define IFILESYSTEM_H_

#include	"jointcoding.h"
#include	"jc/io/Uri.h"
#include    "jc/io/InputStream.h"
#include    "jc/io/File.h"
#include    "jc/io/IFileMapper.hpp"

namespace jc {

enum FileOpenMode_e {
    /**
     * 読み込み専用
     */
    FileOpenMode_Read,

    /**
     * 読み書きを行う
     */
    FileOpenMode_ReadWrite,

    /**
     * 書き込み専用
     */
    FileOpenMode_Write,
};

/**
 * ファイル読み込み時のヒントを設定する
 * 但し、実装によっては呼び出し側に不都合が無い程度にヒントに従わない場合もある。
 */
struct FileOpenHint {
    FileOpenMode_e mode;

    FileOpenHint() {
        mode = FileOpenMode_Read;
    }
};

/**
 * 端末固有のファイルシステムを抽象化する。
 */
class FileSystem {
protected:
    FileSystem() {
    }

public:
    virtual ~FileSystem() {
    }

    /**
     * 特定ディレクトリ内のファイルを開く
     */
    virtual u32 listFiles(const Uri base_uri, std::vector<File> *result);

    /**
     * ファイルを読み込む
     * ファイルのマッピング方法は実装により異なる。
     */
    virtual MFileMapper loadFile(const Uri &uri, const FileOpenHint *hint = NULL) = 0;
};

}
#endif /* IFILESYSTEM_H_ */
