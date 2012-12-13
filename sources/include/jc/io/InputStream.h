/*
 * InputStream.h
 *
 *  Created on: 2012/05/12
 *      Author: Takeshi
 */

#ifndef INPUTSTREAM_H_
#define INPUTSTREAM_H_

#include    "jointcoding.h"
#include    "jc/mem/SmartPtr.h"
#include    "jc/mem/MemoryBlock.h"

namespace jc {

class InputStream {
public:
    virtual ~InputStream() {

    }

    /**
     * 1byte読み取る
     * @return 読み取ったデータ
     */
    virtual u8 read() {
        u8 result = 0;
        if (read(&result, 1) == 1) {
            return result;
        }
        return -1;
    }

    /**
     * 指定バイト数読み取る
     * @return 読み込んだバイト数
     */
    virtual s32 read(u8* result, s32 size) = 0;

    /**
     * 指定バイト数読み取りヘッダを飛ばす
     */
    virtual s32 skip(s32 bytes) = 0;

    /**
     * 読み取り可能な残容量を取得する。
     */
    virtual s32 available() = 0;

    /**
     * ストリームを閉じる。
     * この呼出以降、ストリームは無効になる。
     */
    virtual void close() = 0;

    /**
     * バイト配列に変換する。
     */
    static jc_sa<u8> toByteArray(jc_sp<InputStream> is);

    /**
     * テキストに変換する
     */
    static String toText(jc_sp<InputStream> is);
};

    /**
     * ManagedPointer
     */
typedef jc_sp<InputStream> MInputStream;

}
#endif /* INPUTSTREAM_H_ */
