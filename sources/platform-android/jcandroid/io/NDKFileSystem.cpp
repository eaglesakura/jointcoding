/*
 * FileSystem.cpp
 *
 *  Created on: 2012/07/11
 */
#include    "jcandroid/io/NDKFileSystem.h"
#include    "android-classes/ndkNativeIOUtil.h"
#include    "jcandroid/io/JavaJointInputStream.h"
#include    "jc/system/Exception.h"

namespace ndk {

NDKFileSystem::NDKFileSystem(jc_sp< NativeContext > context) {
    this->context = context;
    this->assets.reset(new JObject(context->getAssetManager_unsafe()));
    this->assets->addGlobalRef();

    CALL_JNIENV();
    this->pAssetManager = AAssetManager_fromJava(env, assets->getObject());
}

NDKFileSystem::~NDKFileSystem() {
}

/**
 * ファイルを読み込む
 * ファイルのマッピング方法は実装により異なる。
 */
MFileMapper NDKFileSystem::loadFile(const Uri &uri, const FileOpenHint *hint) {
    const UriSchemeType_e scheme = uri.getSchemeType();
    if (scheme == UriSchemeType_Assets) {
        // load assets
        return  AssetFileMapper::createInstance(pAssetManager, uri, hint);
    }

    jclogf("unknown scheme :: %s", uri.getScheme().c_str());
    return MFileMapper();
}

}
