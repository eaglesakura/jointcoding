/*
 * FileOutputStream.cpp
 *
 *  Created on: 2012/12/19
 */
#include    "jointcoding.h"
#include    "jc/io/FileOutputStream.h"

namespace jc {

FileOutputStream::FileOutputStream(const charactor *fileName, jcboolean *completed) {
    fp = NULL;
    initialize(fileName);

    if (completed) {
        (*completed) = (fp != NULL);
    }
}

FileOutputStream::FileOutputStream(const String &fileName, jcboolean *completed) {
    fp = NULL;
    initialize(fileName.c_str());

    if (completed) {
        (*completed) = (fp != NULL);
    }

}

FileOutputStream::~FileOutputStream() {
    close();
}

void FileOutputStream::initialize(const charactor *name) {
    fp = fopen(name, "wb");
}

/**
 * 指定バイト数のデータを書き込む
 */
void FileOutputStream::write(const u8 *src, s32 length) {
    if (!fp) {
        throw create_exception(IOException, "FilePointer open failed");
    }
    if (length <= 0) {
        throw create_exception(IOException, "Write Length Error");
    }

    if (fwrite((void*) src, (size_t) length, 1, fp) == 0) {
        throw create_exception(IOException, "fwrite error");
    }
}

/**
 * ストリームを閉じる
 */
void FileOutputStream::close() {
    if (fp) {
        fclose(fp);
        fp = NULL;
    }
}

}

