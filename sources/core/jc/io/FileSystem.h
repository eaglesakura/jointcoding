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

namespace jc {

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

protected:
    virtual MInputStream openAsset(const String fileName) = 0;
    virtual MInputStream openExternalStrageFile(const String fileName) = 0;
    virtual MInputStream openLocalStrageFile(const String fileName) = 0;
};

}
#endif /* IFILESYSTEM_H_ */
