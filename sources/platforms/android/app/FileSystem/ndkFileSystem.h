/*
 * ndkFileSystem.h
 *
 *  Created on: 2012/07/11
 */

#ifndef NDKFILESYSTEM_H_
#define NDKFILESYSTEM_H_

#include    "jointcoding-android.h"
#include    "ndkNativeContext.h"
#include    "jcInputStream.h"
#include    "jcFileSystem.h"

namespace ndk {

class NDKFileSystem: public FileSystem {
    jc_sp< NativeContext > context;
public:
    NDKFileSystem(jc_sp< NativeContext > context);
    virtual ~NDKFileSystem();

protected:
    virtual MInputStream openAsset(const String fileName);
    virtual MInputStream openExternalStrageFile(const String fileName);
};

}

#endif /* NDKFILESYSTEM_H_ */
