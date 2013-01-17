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
    /**
     * ファイル名
     * 41文字以内で登録する
     */
    charactor file_name[42];

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

/**
 * ファイルのアーカイブバージョン
 */
const u32 FILEARCHIVE_VERSION = 0x1;

}

#endif /* PRV_FILEARCHIVE_H_ */
