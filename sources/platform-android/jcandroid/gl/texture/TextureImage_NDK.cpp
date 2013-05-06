/*
 * TextureImage_NDK.cpp
 *
 *  Created on: 2013/02/15
 */

#include    "jointcoding-android.h"
#include    "jc/platform/Platform.h"
#include    "jc/platform/Timer.h"
#include    "jcandroid/io/JavaJointInputStream.h"
#include    "android-classes/ndkImageDecoder.h"

#include    "jc/gl/gpu/DeviceLock.hpp"
#include    "jc/gl/texture/TextureImage.h"

using namespace ndk;

namespace jc {
namespace gl {

class NDKPixelBuffer: public PixelBuffer {
    jc_sp<JObject> byteBuffer;
public:
    NDKPixelBuffer(jobject jByteBuffer, const s32 width, const s32 height) : PixelBuffer(PixelFormat_NativeRGBA, width, height) {
        byteBuffer.reset(new JObject(jByteBuffer));
        byteBuffer->addGlobalRef();

        CALL_JNIENV();
        this->header = (u8*)env->GetDirectBufferAddress(byteBuffer->getObject());
        assert(header);
    }

    virtual ~NDKPixelBuffer() {
        byteBuffer.reset();
    }
};

/**
 * プラットフォームデコーダを利用してデコードを行う
 * iOS / AndroidであればJpeg / PNG / Bitmapが共通でデコードできる
 */
MPixelBuffer TextureImage::decodePixelsFromPlatformDecoder(MDevice device, const Uri &uri, TextureLoadOption *option) {
    MPixelBuffer result;
    CALL_JNIENV();

    const jctime decode_starttime = Timer::currentTime();
    MInputStream stream = Platform::getFileSystem()->openInputStream(uri);

    jc_sp<JavaJointInputStream> jInputStream = downcast<JavaJointInputStream>(stream);
    if (!jInputStream) {
        jclogf("Uri(%s) not java joint", uri.getUri().c_str());
        return result;
    }

    jobject jImageDecoder = ndk::ImageDecoder::decodeFromStream_unsafe(jInputStream->getJniStream()->getObject());
    if (!jImageDecoder) {
        // デコード失敗した
        jclog("decode fail");
        return result;
    }

    if (option) {
        // ロード時間を保存する
        option->result.raw_load_time_ms = Timer::lapseTimeMs(decode_starttime);
    }

    // ラップして、必要な情報を取り出す
    jobject jPixelBuffer = ndk::ImageDecoder::getPixels_unsafe_(jImageDecoder);
    s32 imageWidth = ndk::ImageDecoder::getWidth_(jImageDecoder);
    s32 imageHeight = ndk::ImageDecoder::getHeight_(jImageDecoder);

    // バッファを作成する
    result.reset(new NDKPixelBuffer(jPixelBuffer, imageWidth, imageHeight));

    // デコーダ本体を解放する
    env->DeleteLocalRef(jImageDecoder);

    return result;
}

}
}

