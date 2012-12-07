/*
 * JCOutputStream.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCOUTPUTSTREAM_H_
#define JCOUTPUTSTREAM_H_

#include    "jcMacro.h"
#include    "jcObject.h"
#include    "jcSmartPtr.h"

namespace jc {

class OutputStream: public Object {
public:
    OutputStream() {
    }

    virtual ~OutputStream() {

    }

    /**
     * 1byteのデータを書き込む。
     */
    virtual void write(u8 data) {
        write(&data, 1);
    }

    /**
     * 指定バイト数のデータを書き込む
     */
    virtual void write(u8 *src, s32 length) = 0;

    /**
     * ストリームを閉じる
     */
    virtual void close() = 0;
};

/**
 * managed
 */
typedef jc_sp<OutputStream> MOutputStream;

}

#endif /* JCOUTPUTSTREAM_H_ */
