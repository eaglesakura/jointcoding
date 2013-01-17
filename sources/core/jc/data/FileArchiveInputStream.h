/*
 * FileArchiveInputStream.h
 *
 *  Created on: 2013/01/17
 */

#ifndef FILEARCHIVEINPUTSTREAM_H_
#define FILEARCHIVEINPUTSTREAM_H_

#include    "jc/data/BinaryInputStream.h"
#include    "jc/data/prv_FileArchive.h"

namespace jc {

class FileArchiveInputStream: public Object {
    /**
     * アーカイブ済みのファイル一覧
     */
    std::vector<ArchiveInfo> archives;

public:
    FileArchiveInputStream();
    virtual ~FileArchiveInputStream();

    /**
     * 初期化を行う
     */
    virtual void initialize(MInputStream stream);

    /**
     * ファイルの読み込みを開始する
     */
    virtual jcboolean findFile(const String &file_name, ArchiveInfo *result);

};
}

#endif /* FILEARCHIVEINPUTSTREAM_H_ */
