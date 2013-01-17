/*
 * File.h
 *
 *  Created on: 2013/01/17
 */

#ifndef FILE_H_
#define FILE_H_

#include    "jc/system/Macro.h"
#include    "jc/io/Uri.h"

namespace jc {

/**
 * １ファイル | １ディレクトリを示すクラス
 */
class File {
private:
    enum Type_e {
        /**
         * ディレクトリを示す
         */
        Type_Directory,

        /**
         * ファイルを示す
         */
        Type_File,

        /**
         * 不明
         */
        Type_Unknown,
    };

    /**
     * ファイルへのURI
     * フルパスで格納される
     */
    Uri uri;

    /**
     * ファイルタイプ
     */
    Type_e type;
public:
    File() {
        type = Type_Unknown;
    }

    File(const File &origin) {
        uri = origin.uri;
        type = origin.type;
    }

    /**
     * ファイルである場合、trueを返す。
     */
    jcboolean isFile() const {
        return type == Type_File;
    }

    /**
     * ディレクトリである場合、trueを返す。
     */
    jcboolean isDirectory() const {
        return type == Type_Directory;
    }

    String getName() const {
        return uri.getFileName();
    }

    String getExt() const {
        return uri.getFileExt();
    }

    const Uri& getUril() const {
        return uri;
    }

    /**
     * ファイルURIからパースする
     */
    static File fromFile(const Uri &uri) {
        File result;
        result.uri = uri;
        result.type = Type_File;
        return result;
    }

    /**
     * ディレクトリURIからパースする
     */
    static File fromDirectory(const Uri &uri) {
        File result;
        result.uri = uri;
        result.type = Type_Directory;
        return result;
    }
};

}

#endif /* FILE_H_ */
