/*
 * FileArchiveInputStream.cpp
 *
 *  Created on: 2013/01/17
 */

#include    "jc/data/FileArchiveImporter.h"

namespace jc {

FileArchiveImporter::FileArchiveImporter() {

}

FileArchiveImporter::~FileArchiveImporter() {

}

/**
 * 初期化を行う
 */
void FileArchiveImporter::initialize(MInputStream stream) {
    const s32 file_size = stream->available();
    MBinaryInputStream reader(new BinaryInputStream(stream));

    // まずはヘッダを読み込む
    {
        const u32 file_version = reader->readU32();
        if (file_version != ArchiveInfo::FILEVERSION) {
            throw create_exception(IOException, "FileVersion Not Support");
        }
    }

    // ファイルの末尾に移動して、ファイル数を読む
    stream->skip(file_size - 4 - 4);

    // ファイル数
    const u32 file_num = reader->readU32();

    jclogf("archive files(%d)", file_num);

    // ファイルのインフォメーションを読み込む
    stream->skip(-4 - ArchiveInfo::SIZEOF * file_num);

    for (int i = 0; i < file_num; ++i) {
        ArchiveInfo info;

        stream->read((u8*) info.file_name, sizeof(charactor) * ArchiveInfo::FILENAME_SIZE);
        info.file_head = reader->readU32();
        info.file_length = reader->readU32();

//        jclogf("archived[%d] (%s) offset[%d] -> length[%d]", i, info.file_name, info.file_head, info.file_length);
        archives.push_back(info);
    }
}

/**
 * ファイルの読み込みを開始する
 */
jcboolean FileArchiveImporter::findFile(const String &file_name, ArchiveInfo *result) {

    for (int i = 0; i < archives.size(); ++i) {
        if (strcmp(archives[i].file_name, file_name.c_str()) == 0) {
            (*result) = archives[i];
            return jctrue;
        }
    }

    return jcfalse;
}
}
