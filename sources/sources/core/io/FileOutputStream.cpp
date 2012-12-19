/*
 * FileOutputStream.cpp
 *
 *  Created on: 2012/12/19
 */
#include    "jointcoding.h"
#include    "jc/io/FileOutputStream.h"

namespace jc {

FileOutputStream::FileOutputStream(const charactor *fileName) {
    fp = NULL;
    initialize(fileName);
}

FileOutputStream::FileOutputStream(const String &fileName) {
    fp = NULL;
}

FileOutputStream::~FileOutputStream() {
    close();
}

void FileOutputStream::initialize(const charactor *name) {

}

/**
 * 指定バイト数のデータを書き込む
 */
void FileOutputStream::write(u8 *src, s32 length) {
    if (fwrite((void*) src, (size_t) length, 1, fp) != 0) {
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

