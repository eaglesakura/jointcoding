/*
 * StringUtil.cpp
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */
#include    "jc/system/StringUtil.h"
#include    <string>
#include    <boost/algorithm/string.hpp>


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
s32 split(String origin, String delim, jc_varray<String> *result) {
    const std::string &str = origin.get_native<std::string>();
    const std::string &dlm = delim.get_native<std::string>();
    jc_varray<std::string> temp;

    // 分割はboostを利用する
    boost::split(temp, str, boost::is_any_of(dlm));

    for (s32 i = 0; i < temp.size(); ++i) {
        result->push_back(temp[i].c_str());
    }

    return temp.size();
}

}
