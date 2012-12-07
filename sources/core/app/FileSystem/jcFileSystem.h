/*
 * IFileSystem.h
 *
 *  Created on: 2012/05/20
 *      Author: Takeshi
 */

#ifndef IFILESYSTEM_H_
#define IFILESYSTEM_H_

#include	"jointcoding.h"
#include	"jcUri.h"
#include    "jcInputStream.h"

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

protected:
    virtual MInputStream openAsset(const String fileName) = 0;
    virtual MInputStream openExternalStrageFile(const String fileName) = 0;
};

}
#endif /* IFILESYSTEM_H_ */
