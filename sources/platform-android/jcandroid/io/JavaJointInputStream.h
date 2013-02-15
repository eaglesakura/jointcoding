/*
 * JavaJointInputStream.h
 *
 *  Created on: 2012/05/16
 *      Author: Takeshi
 */

#ifndef JAVAJOINTINPUTSTREAM_H_
#define JAVAJOINTINPUTSTREAM_H_

#include "jointcoding-android.h"
#include "jc/io/InputStream.h"
#include "android-classes/ndkJniInputStream.h"

namespace ndk {

/**
 * Java部分との接続を行うInputStream
 */
class JavaJointInputStream: public InputStream {
    /**
     * 読み込み元のストリーム
     */
    jc_sp<JniInputStream> stream;

    /**
     * 一時領域
     */
    jc_sp<JByteArray> tempBuffer;
public:
    JavaJointInputStream(jobject inputstream);
    virtual ~JavaJointInputStream();

    /**
     * 指定バイト数読み取る
     * @return 読み込んだバイト数
     */
    virtual s32 read(u8* result, s32 size);

    /**
     * 指定バイト数読み取りヘッダを飛ばす
     */
    virtual s32 skip(s32 bytes);

    /**
     * 読み取り可能な残容量を取得する。
     */
    virtual s32 available();

    /**
     * ストリームを閉じる
     */
    virtual void close();

    /**
     * 元のストリームを取得する
     */
    virtual jc_sp<JniInputStream> getJniStream() const {
        return stream;
    }
};

}
#endif /* JAVAJOINTINPUTSTREAM_H_ */
