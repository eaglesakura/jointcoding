/*
 * Uri.cpp
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#include    "jc/io/Uri.h"
#include    "protocol/jcUriProtocol.h"

namespace jc {
/*
 inline s32 indexOf(String &main, String &sub) {

 charactor *p = strchr(main.c_str(), (int) (*sub.c_str()));
 // 頭文字が見つかった
 if (p) {
 s32 result = (s32) p - (s32) main.c_str();

 charactor *head = sub.c_str();

 while ((*head) && (*p)) {

 // 一致しない文字があった
 if ((*head) != (*p)) {
 return -1;
 }

 ++head;
 ++p;
 }

 // 全て一致した
 return result;
 }
 return -1;
 }
 */

/**
 * スキーマを取得する
 */
String Uri::getScheme() const {
    s32 index = uri.indexOf("://");
    if (index >= 0) {
        index += 3;
        return uri.substring(0, index);
    }

    return "";
}

/**
 * パス部分を取得する
 */
String Uri::getPath() const {
    s32 index = uri.indexOf("://");
    // スキーマが見つかった
    if (index >= 0) {
        index += 3;
        return uri.substring(index, uri.length() - index);
    }
    return "";
}

/**
 * アセットからURIを生成する
 */
Uri Uri::fromAssets(const String &path) {
    return Uri(String(UriProtocol::SCHEME_APPLI_ASSETS) + path);
}

/**
 * URLからURIを生成する
 */
Uri Uri::fromWeb(const String &url) {
    return Uri(url);
}

/**
 * ローカルストレージからURIを生成する
 */
Uri Uri::fromLocalStrage(const String &path) {
    return Uri(String(UriProtocol::SCHEME_LOCALSTRAGE) + path);
}

/**
 * 外部ストレージからURIを生成する
 */
Uri Uri::fromExternalStrage(const String &path) {
    return Uri(String(UriProtocol::SCHEME_EXTERNALSTRAGE) + path);
}
}
