/*
 * FileArchiveOutputStream.h
 *
 *  Created on: 2013/01/17
 */

#ifndef FILEARCHIVEOUTPUTSTREAM_H_
#define FILEARCHIVEOUTPUTSTREAM_H_

#include    "jc/data/BinaryOutputStream.h"
#include    "jc/data/prv_FileArchive.h"

namespace jc {

/**
 * ファイルを簡易的に連結する。
 * 連結のみを提供して、圧縮はサポートしない。
 */
class FileArchiveOutputStream: public OutputStream {
    /**
     * 書き込みポインタ
     */
    u32 write_pointer;

    /**
     * アーカイブ済みのファイル一覧
     */
    std::vector<ArchiveInfo> archives;

    /**
     * 現在書込み中のファイル
     */
    ArchiveInfo current_file;

    /**
     * ファイルの書込み中の場合はtrue
     */
    jcboolean file_writing;

    /**
     * 書き込み対象のストリーム
     */
    MBinaryOutputStream stream;

    /**
     * ヘッダを書き込む
     */
    void write_header();

    /**
     * フッダを書き込む
     */
    void write_fooder();

public:
    /**
     *
     */
    FileArchiveOutputStream(MOutputStream output);

    /**
     *
     */
    virtual ~FileArchiveOutputStream();

    /**
     * ファイルを開始する
     */
    virtual void beginFile(const String &file_name);

    /**
     * ファイルを終了する
     */
    virtual void endFile();

    /**
     * 指定バイト数のデータを書き込む
     */
    virtual void write(const u8 *src, s32 length);

    /**
     * ストリームを閉じる
     */
    virtual void close();
};

/**
 * managed
 */
typedef jc_sp<FileArchiveOutputStream> MFileArchiveOutputStream;

}

#endif /* FILEARCHIVEOUTPUTSTREAM_H_ */
