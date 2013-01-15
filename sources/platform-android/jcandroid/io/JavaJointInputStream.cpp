/*
 * JavaJointInputStream.cpp
 *
 *  Created on: 2012/05/16
 *      Author: Takeshi
 */

#include    "jcandroid/io/JavaJointInputStream.h"
#include    "jc/system/Exception.h"

namespace ndk {

JavaJointInputStream::JavaJointInputStream(jobject inputstream) {
    CALL_JNIENV();
    this->stream = JniInputStream::global(inputstream);
//    env->DeleteLocalRef(inputstream);
    // 一時領域を作成する。デフォルトは32kb
    this->tempBuffer = JByteArray::global(env->NewByteArray(1024 * 32));
}

JavaJointInputStream::~JavaJointInputStream() {
    try {
        jclog("java inputstream close");
        this->close();
        jclog("java inputstream close finish");
    } catch (const Exception &e) {
        jcloge(e);
    }

    try {
        this->tempBuffer.reset();
    } catch (const Exception &e) {
        jcloge(e);
    }
}

/**
 * 指定バイト数読み取る
 * @return 読み込んだバイト数
 */
s32 JavaJointInputStream::read(u8* result, s32 size) {
    s32 readed = stream->read(tempBuffer->getArrayObject(), 0, size);
    stream->throwHasException();

    // 書き戻すべきバッファがあるなら書き戻す
    if (readed > 0) {
        tempBuffer->get(0, readed, (jbyte*) result);
    }
    // テンポラリ領域を書きなおす
//    tempBuffer->get()
    /**
     jclogf("readed = %d", readed);
     jclogf("size = %d", size);
     {
     jbyte *p = tempBuffer->lock();
     memcpy(result, p, size);
     tempBuffer->unlock(JniArrayAbort);
     }
     {
     }
     */
    // 読み込んだ長さを返す
    return readed;
}

/**
 * 指定バイト数読み取りヘッダを飛ばす
 */
s32 JavaJointInputStream::skip(s32 bytes) {
    s32 result = stream->skip(bytes);
    stream->throwHasException();
    return result;
}

/**
 * 読み取り可能な残容量を取得する。
 */
s32 JavaJointInputStream::available() {
    s32 result = stream->available();

    stream->throwHasException();
    return result;
}

void JavaJointInputStream::close() {
    if (stream) {
        stream->close();

        try {
            stream->throwHasException();
            stream.reset();
        } catch (const Exception &e) {
            stream.reset();
            jcloge(e);
            throw;
        }
    }
}

}
