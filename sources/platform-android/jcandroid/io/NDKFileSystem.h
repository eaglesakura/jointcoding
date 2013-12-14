/*
 * ndkFileSystem.h
 *
 *  Created on: 2012/07/11
 */

#ifndef NDKFILESYSTEM_H_
#define NDKFILESYSTEM_H_

#include    "jointcoding-android.h"
#include    "android-classes/ndkNativeContext.h"
#include    "jc/io/InputStream.h"
#include    "jc/io/FileSystem.h"

#include    "jcandroid/io/AssetFileMapper.hpp"

namespace ndk {

class NDKFileSystem: public FileSystem {
    jc_sp<NativeContext > context;
    jc_sp<JObject> assets;

    AAssetManager* pAssetManager;
public:
    NDKFileSystem(jc_sp< NativeContext > context);
    virtual ~NDKFileSystem();

    /**
     * ファイルを読み込む
     * ファイルのマッピング方法は実装により異なる。
     */
    virtual MFileMapper loadFile(const Uri &uri, const FileOpenHint *hint = NULL);

protected:
    virtual MInputStream openAsset(const String fileName);
    virtual MInputStream openExternalStrageFile(const String fileName);
    virtual MInputStream openLocalStrageFile(const String fileName);

};

}

#endif /* NDKFILESYSTEM_H_ */
