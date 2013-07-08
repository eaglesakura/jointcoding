/*
 * FileArchiveOutputStream.cpp
 *
 *  Created on: 2013/01/17
 */

#include    "jc/data/FileArchiveOutputStream.h"

namespace jc {

/**
 *
 */
FileArchiveOutputStream::FileArchiveOutputStream(MOutputStream output) {
    this->stream.reset(new BinaryOutputStream(output));
    this->file_writing = jcfalse;
    this->write_pointer = 0;

    // ファイルヘッダを書き込む
    write_header();
}

/**
 *
 */
FileArchiveOutputStream::~FileArchiveOutputStream() {
    if (file_writing) {
        this->endFile();
    }
    close();
}

/**
 * ヘッダを書き込む
 */
void FileArchiveOutputStream::write_header() {
    stream->writeU32(ArchiveInfo::FILEVERSION);
    write_pointer = 4;
}

/**
 * フッダを書き込む
 */
void FileArchiveOutputStream::write_fooder() {
    for (int i = 0; i < archives.size(); ++i) {
        stream->writeRawByteArray(archives[i].file_name, sizeof(archives[i].file_name));
        stream->writeU32(archives[i].file_head);
        stream->writeU32(archives[i].file_length);
    }

    // ファイル数を書き込む
    stream->writeU32(archives.size());
}

/**
 * ファイルを開始する
 */
void FileArchiveOutputStream::beginFile(const String &file_name) {
    current_file = ArchiveInfo();
    strcpy(current_file.file_name, file_name.c_str());
    current_file.file_length = 0;
    current_file.file_head = write_pointer;

    file_writing = jctrue;
}

/**
 * ファイルを終了する
 */
void FileArchiveOutputStream::endFile() {
    archives.push_back(current_file);

    file_writing = jcfalse;
    jclogf("  archive(%s) %d -> %d", current_file.file_name, current_file.file_head, (current_file.file_head + current_file.file_length));
}

/**
 * 指定バイト数のデータを書き込む
 */
void FileArchiveOutputStream::write(const u8 *src, s32 length) {
    stream->writeRawByteArray(src, length);

// 書き込み位置を進める
    write_pointer += length;

// ファイルの大きさを進める
    current_file.file_length += length;
}

/**
 * ストリームを閉じる
 */
void FileArchiveOutputStream::close() {
    if (stream) {
        //フッダを書き込む
        write_fooder();

        stream.reset();
    }
}

}
