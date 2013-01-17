/*
 * FileInputStream.cpp
 *
 *  Created on: 2012/05/12
 *      Author: Takeshi
 */

#include "jointcoding.h"
#include "jc/io/FileInputStream.h"
#include "jc/math/Math.h"

namespace jc {

FileInputStream::FileInputStream(const String &fileName, jcboolean *completed) {
    file = fopen((char*) fileName.c_str(), "rb");
    if (completed) {
        (*completed) = (file != NULL);
    }
    setAutoClose(true);
    init();
}

FileInputStream::FileInputStream(const charactor *fileName, jcboolean *completed) {
    file = fopen((char*) fileName, "rb");
    if (completed) {
        (*completed) = (file != NULL);
    }
    setAutoClose(true);
    init();
}
FileInputStream::FileInputStream(FILE* fp) {
    file = fp;
    setAutoClose(false);
    init();
}

FileInputStream::~FileInputStream() {
    close();
}

void FileInputStream::init() {
    fpos_t current = 0;
    // 現在位置を保存
    fgetpos(file, &current);

    // ファイルの最後まで移動して位置を保存
    fseek(file, 0, SEEK_END);
    fpos_t size = 0;
    fgetpos(file, &size);

    // 現在位置から全体サイズを引いたら残量
    this->size = (size - current);

    //
    fseek(file, current, SEEK_SET);
}

s32 FileInputStream::read(u8 *result, s32 size) {
    s32 readed = fread(result, 1, size, file);
    return readed;
}

s32 FileInputStream::skip(s32 bytes) {
    bytes = min(size, bytes);
    s32 result = fseek(file, bytes, SEEK_CUR);
    return result;
}

void FileInputStream::close() {
    if (file != NULL && isAutoClose()) {
        fclose(file);
        file = NULL;
    }
}

}
