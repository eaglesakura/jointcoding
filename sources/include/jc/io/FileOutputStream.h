/*
 * FileOutputStream.h
 *
 *  Created on: 2012/12/19
 */

#ifndef FILEOUTPUTSTREAM_H_
#define FILEOUTPUTSTREAM_H_

#include    "jc/io/OutputStream.h"
#include    <stdio.h>

namespace jc {

class FileOutputStream: public OutputStream {
    FILE *fp;

    void initialize(const charactor *name);
public:
    FileOutputStream(const charactor *fileName);
    FileOutputStream(const String &fileName);

    virtual ~FileOutputStream();

    /**
     * 指定バイト数のデータを書き込む
     */
    virtual void write(u8 *src, s32 length);

    /**
     * ストリームを閉じる
     */
    virtual void close();
};

}

#endif /* FILEOUTPUTSTREAM_H_ */
