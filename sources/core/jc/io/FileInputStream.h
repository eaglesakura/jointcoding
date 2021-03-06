/*
 * FileInputStream.h
 *
 *  Created on: 2012/05/12
 *      Author: Takeshi
 */

#ifndef FILEINPUTSTREAM_H_
#define FILEINPUTSTREAM_H_

#include "jointcoding.h"
#include "stdio.h"
#include "jc/io/InputStream.h"

namespace jc {

class FileInputStream: public InputStream {

    /**
     * 読み取り可能な残りサイズ
     */
    s32 size;

    /**
     * 読み取り対象のファイルポインタ
     */
    FILE *file;

    /**
     * 自動でファイルをcloseする場合はtrue
     */
    bool autoClose;

    void init();
public:
    FileInputStream(const String &fileName, jcboolean *completed);
    FileInputStream(const charactor* fileName, jcboolean *completed);
    FileInputStream(FILE* fp);
    virtual ~FileInputStream();

    /**
     * 自動的にfclose()する場合はtrue
     */
    virtual void setAutoClose(bool set) {
        autoClose = set;
    }

    /**
     * 自動でfclose()する場合はtrue
     */
    virtual bool isAutoClose() {
        return autoClose;
    }

    virtual s32 read(u8 *result, s32 size);

    /**
     * 指定バイト数読み取りヘッダを飛ばす
     */
    virtual s32 skip(s32 bytes);

    /**
     * 読み取り可能な残容量を取得する。
     */
    virtual s32 available() {
        return size;
    }

    /**
     * ストリームを閉じる
     */
    virtual void close();
};

}
#endif /* FILEINPUTSTREAM_H_ */
