/*
 * Uri.h
 *
 *  Created on: 2012/05/20
 *      Author: Takeshi
 */

#ifndef URI_H_
#define URI_H_

#include	"jointcoding.h"
#include    "jc/system/jcString.h"

namespace jc {

/**
 * 特定のファイルを示すURIを構築する。
 * 内部的には独自スキーマかプラットフォームごとに決められたスキーマが投入される。
 */
class Uri {
    /**
     * URL本体
     */
    String uri;
public:

    /**
     * 直接的な作成は避けるほうが無難
     */
    Uri(String uriString) {
        this->uri = uriString;
    }

    /**
     *
     */
    Uri() {

    }

    /**
     * 代入演算は許可する
     */
    Uri(const Uri &origin) {
        this->uri = origin.uri;
    }
    virtual ~Uri() {

    }

    /**
     * 代入を行う
     */
    Uri& operator=(const Uri &origin) {
        this->uri = origin.uri;
        return *this;
    }

    /**
     * 比較を行う
     */
    bool operator==(const Uri &origin) const {
        return uri == origin.uri;
    }

    /**
     * 比較を行う
     */
    bool operator!=(const Uri &origin) const {
        return uri == origin.uri;
    }

    /**
     * スキーマを取得する
     */
    virtual String getScheme() const;

    /**
     * パス部分を取得する
     */
    virtual String getPath() const;

    /**
     * ファイル名を取得する
     */
    virtual String getFileName() const;

    /**
     * ファイル拡張子を取得する
     */
    virtual String getFileExt() const;

    /**
     * URI全文を取得する
     */
    virtual const String& getUri() const {
        return uri;
    }

    /**
     * パースを行う。
     */
    static Uri parse(const String &uri);

    /**
     * アセットからURIを生成する
     */
    static Uri fromAssets(const String &path);

    /**
     * URLからURIを生成する
     */
    static Uri fromWeb(const String &url);

    /**
     * ローカルストレージからURIを生成する
     */
    static Uri fromLocalStrage(const String &path);

    /**
     * 外部ストレージからURIを生成する
     */
    static Uri fromExternalStrage(const String &path);

#ifdef  BUILD_Android
    /**
     * アプリのrawから取得する
     */
    static Uri fromApkRaw(const s32 rawId);
#endif
};

}
#endif /* URI_H_ */
