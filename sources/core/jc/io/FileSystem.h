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
};

/**
 * ファイル読み込み時のヒントを設定する
 * 但し、実装によっては呼び出し側に不都合が無い程度にヒントに従わない場合もある。
 */
struct FileOpenHint {
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
     * 読み込み用のストリームを開く。
     */
    virtual MInputStream openInputStream(const Uri &uri);

    /**
     * 特定ディレクトリ内のファイルを開く
     */
    virtual u32 listFiles(const Uri base_uri, std::vector<File> *result);

    /**
     * ファイルを読み込む
     * ファイルのマッピング方法は実装により異なる。
     */
    virtual MFileMapper loadFile(const Uri &uri, const FileOpenHint *hint = NULL) = 0;

protected:
    virtual MInputStream openAsset(const String fileName) = 0;
    virtual MInputStream openExternalStrageFile(const String fileName) = 0;
    virtual MInputStream openLocalStrageFile(const String fileName) = 0;
};

}
#endif /* IFILESYSTEM_H_ */
