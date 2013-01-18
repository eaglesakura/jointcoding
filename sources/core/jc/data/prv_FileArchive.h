/*
 * prv_FileArchive.h
 *
 *  Created on: 2013/01/17
 */

#ifndef PRV_FILEARCHIVE_H_
#define PRV_FILEARCHIVE_H_

#include    "jc/mem/MemoryUtil.h"
#include    <vector>

namespace jc {

/**
 * アーカイブ済みのファイル情報
 */
struct ArchiveInfo {

    enum {
        /**
         * ファイル名の文字数
         */
        FILENAME_SIZE = 42,

        /**
         * ファイルバージョン
         */
        FILEVERSION = 0x1,

        /**
         * sizeof値
         */
        SIZEOF = sizeof(charactor) * FILENAME_SIZE + sizeof(u32) + sizeof(u32),
    };

    /**
     * ファイル名
     * 41文字以内で登録する
     */
    charactor file_name[FILENAME_SIZE];

    /**
     * ヘッダとなるファイルポインタ位置
     */
    u32 file_head;

    /**
     * ファイルの長さ
     */
    u32 file_length;

    ArchiveInfo() {
        zeromemory(file_name, sizeof(file_name));
        file_head = file_length = 0;
    }
};

}

#endif /* PRV_FILEARCHIVE_H_ */
