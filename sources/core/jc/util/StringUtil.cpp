/*
 * StringUtil.cpp
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */
#include    "jointcoding.h"
#include    <string>
#include    "jc/system/StringUtil.h"
#include    "boost/algorithm/string.hpp"

/**
 * 可変長配列
 * 通常はstd::vector
 */
#define jc_varray   ::std::vector

/**
 * 可変長リスト
 * 通常はstd::list
 */
#define jc_vlist    ::std::list

namespace jc {

/**
 * ストリングを特定の文字をセパレータにして分割する。

 * @param origin 分割元のURL
 * @param delim 分割用の文字列
 * @param result 格納先のベクター
 */
::jc::s32 split(const String &origin, const String &delim, jc_varray<String> *result) {
    const std::string &str = origin.get_native<std::string>();
    const std::string &dlm = delim.get_native<std::string>();
    jc_varray<std::string> temp;

    // 分割はboostを利用する
        boost::split(temp, str, boost::is_any_of(dlm));

        for (int i = 0; i < (int)temp.size(); ++i) {
            result->push_back(temp[i].c_str());
        }

        return temp.size();
    }

/**
 * パスを分解し、ファイル名を取り出す。
 * 取り出せなかった場合はそのまま帰す。
 */
const charactor* getFileName(const charactor *path) {
    if (strchr(path, '/')) {
        return strrchr(path, '/') + 1;
    } else if (strchr(path, '\\')) {
        return strrchr(path, '\\') + 1;
    } else {
        return path;
    }
}

/**
 * パスを分解し、ファイル名を取り出す。
 * 取り出せなかった場合はそのまま帰す。
 */
const charactor* getFileExt(const charactor *path) {
    if (strchr(path, '.')) {
        return strrchr(path, '.') + 1;
    }
    return NULL;
}

}
