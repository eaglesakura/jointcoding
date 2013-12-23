/*
 * FileSystem.cpp
 *
 *  Created on: 2012/07/22

 */

#include    "jc/io/FileSystem.h"
#include    "protocol/jcUriProtocol.h"
#include    "jc/system/Exception.h"

#include    <dirent.h>

namespace jc {


/**
 * 特定ディレクトリ内のファイルを開く
 */
u32 FileSystem::listFiles(const Uri base_uri, std::vector<File> *result) {
    DIR *dir = opendir(base_uri.getPath().c_str());

    if (!dir) {
        return 0;
    }

    u32 finded = 0;
    dirent *dp;

    while ((dp = readdir(dir)) != NULL) {
//        jclogf("path = %s[%d]", dp->d_name, dp->d_type);
        Uri dp_uri = Uri::parse(base_uri.getUri() + dp->d_name);
        if (dp->d_type == DT_DIR) {
            result->push_back(File::fromDirectory(dp_uri));
        } else {
            result->push_back(File::fromFile(dp_uri));
        }
        ++finded;
    }
    closedir(dir);

    return finded;
}


}

