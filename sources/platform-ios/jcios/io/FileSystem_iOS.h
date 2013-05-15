/*
 * ndkFileSystem.h
 *
 *  Created on: 2012/07/11
 */

#ifndef NDKFILESYSTEM_H_
#define NDKFILESYSTEM_H_

#include    "jointcoding-ios.h"
#include    "jc/io/InputStream.h"
#include    "jc/io/FileSystem.h"

namespace ndk {

class iOSFileSystem: public FileSystem {
public:
    iOSFileSystem();
    virtual ~iOSFileSystem();

protected:
    virtual MInputStream openAsset(const String fileName);
    virtual MInputStream openExternalStrageFile(const String fileName);
    virtual MInputStream openLocalStrageFile(const String fileName);

};

}

#endif /* NDKFILESYSTEM_H_ */
