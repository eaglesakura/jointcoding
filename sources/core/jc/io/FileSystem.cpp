/*
 * FileSystem.cpp
 *
 *  Created on: 2012/07/22

 */

#include    "jc/io/FileSystem.h"
#include    "protocol/jcUriProtocol.h"
#include    "jc/system/Exception.h"

namespace jc {

/**
 * 読み込み用のストリームを開く。
 */
MInputStream FileSystem::openInputStream(const Uri &uri) {
    String scheme = uri.getScheme();
    String path = uri.getPath();

    jclogf("scheme = %s", scheme.c_str());
    jclogf("path = %s", path.c_str());

    // アセットから読み込む
    if (scheme == UriProtocol::SCHEME_APPLI_ASSETS) {
        return this->openAsset(path);
    } else if (scheme == UriProtocol::SCHEME_EXTERNALSTRAGE) {
        return this->openExternalStrageFile(path);
    }

    jclogf("scheme error = %s", scheme.c_str());
    throw create_exception(FileNotFoundException, "scheme error");
}

}

