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

#include    <android/bitmap.h>

using namespace jc;
using namespace jc::gl;
using namespace ndk;

namespace ndk {

class NDKPixelBuffer: public PixelBuffer {

    /**
     * NativeのBufferクラス
     */
    jc_sp<JObject> byteBuffer;
public:
    NDKPixelBuffer(jobject jByteBuffer, const s32 width, const s32 height) : PixelBuffer(PixelFormat_BGRA8888, width, height) {
        byteBuffer.reset(mark_new JObject(jByteBuffer));
        byteBuffer->addGlobalRef();

        CALL_JNIENV();
        this->header = (u8*)env->GetDirectBufferAddress(byteBuffer->getObject());
        assert(header);

    }

    virtual ~NDKPixelBuffer() {
        {
            CALL_JNIENV();
        }
        byteBuffer.reset();
    }
};

class JBitmapPixelBuffer: public PixelBuffer {
    jc_sp<JObject> bitmap;

    AndroidBitmapInfo info;
public:
    JBitmapPixelBuffer(jobject jBitmap, TextureLoadOption *option) : PixelBuffer(PixelFormat_NativeRGBA, 1, 1) {
        CALL_JNIENV();
        bitmap.reset(new JObject(jBitmap));
        bitmap->addGlobalRef();

        int result = AndroidBitmap_getInfo(env, bitmap->getObject(), &info);
        assert(result == ANDROID_BITMAP_RESULT_SUCCESS);

        this->width = info.width;
        this->height = info.height;
        jclogf("native bitmap size(%d x %d)", info.width, info.height);
        switch (info.format) {
            case ANDROID_BITMAP_FORMAT_A_8: {
                jclog("native bitmap format(ANDROID_BITMAP_FORMAT_A_8)");
                this->pixelFormat = PixelFormat_A8;
            }
            break;
            case ANDROID_BITMAP_FORMAT_RGBA_8888: {
                jclog("native bitmap format(ANDROID_BITMAP_FORMAT_RGBA_8888)");
                this->pixelFormat = PixelFormat_RGBA8888;
            }
            break;
            case ANDROID_BITMAP_FORMAT_RGB_565: {
                jclog("native bitmap format(ANDROID_BITMAP_FORMAT_RGB_565)");
                this->pixelFormat = PixelFormat_RGB565;
            }
            break;
            default: {
                jclog("native bitmap format(ANDROID_BITMAP_FORMAT_NONE/ANDROID_BITMAP_FORMAT_RGBA4)");
                // error format
                assert(false);
            }
            break;
        }

        {
            jctime locktimer = Timer::currentTime();
            result = AndroidBitmap_lockPixels(env, bitmap->getObject(), (void**)(&header));
            assert(result == ANDROID_BITMAP_RESULT_SUCCESS);

            // ロックピクセルの時間を打刻
            if(option) {
                option->result.raw_pixelconvert_time_ms = Timer::lapseTimeMs(locktimer);
            }
        }
        assert(header);

        bytesParPixel = Pixel::getPixelBytes(pixelFormat);
    }

    virtual ~JBitmapPixelBuffer() {
        CALL_JNIENV();
        AndroidBitmap_unlockPixels(env, bitmap->getObject());
    }
};

static jcboolean isConvertExistFormat(const int32_t format) {
    switch (format) {
        case ANDROID_BITMAP_FORMAT_A_8:
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
        case ANDROID_BITMAP_FORMAT_RGB_565:
            return jctrue;
        default:
            return jcfalse;
    }
}

MPixelBuffer createPixelBufferFromImageDecoder(jobject jImageDecoder, Vector2i *pImageSize) {
    MPixelBuffer result;

    // ラップして、必要な情報を取り出す
    jobject jPixelBuffer = ndk::ImageDecoder::getPixels_unsafe_(jImageDecoder);
    s32 imageWidth = ndk::ImageDecoder::getWidth_(jImageDecoder);
    s32 imageHeight = ndk::ImageDecoder::getHeight_(jImageDecoder);

    if (pImageSize) {
        pImageSize->x = imageWidth;
        pImageSize->y = imageHeight;
    }

    // バッファを作成する
    result.reset(mark_new NDKPixelBuffer(jPixelBuffer, imageWidth, imageHeight));

    return result;

}

}

namespace jc {
namespace gl {

/**
 * プラットフォームデコーダを利用してデコードを行う
 * iOS / AndroidであればJpeg / PNG / Bitmapが共通でデコードできる
 */
MPixelBuffer TextureImage::decodePixelsFromPlatformDecoder(MDevice device, const Uri &uri, TextureLoadOption *option) {
    MPixelBuffer result;
    CALL_JNIENV();

    const jctime decode_starttime = Timer::currentTime();
    jobject jBitmap = NULL;
    {
        CALL_JNIENV();
        jstring jUri = c2jstring(uri.getUri().c_str());
        jBitmap = ndk::ImageDecoder::decodeBitmapFromNativeUri_unsafe(jUri);
        env->DeleteLocalRef(jUri);
    }

    if (!jBitmap) {
        // デコード失敗した
        jclog("decode fail");
        return MPixelBuffer();
    }

//    assert(jBitmap);

// デコード時刻を打刻
    if (option) {
        option->result.raw_load_time_ms = Timer::lapseTimeMs(decode_starttime);
    }

    AndroidBitmapInfo info = { 0 };
    int bitmapResult = AndroidBitmap_getInfo(env, jBitmap, &info);
    assert(bitmapResult == ANDROID_BITMAP_RESUT_SUCCESS);
    jclogf("native bitmap size(%d x %d)", info.width, info.height);
    jclogf("native bitmap stride(%d)", info.stride);
    if (isConvertExistFormat(info.format)) {
        jclogf("native bitmap direct OK(%d)", info.format);
        // 直接デコードを行う
        result.reset(new JBitmapPixelBuffer(jBitmap, option));
        return result;
    } else {
        jclogf("native bitmap unknown format(%d)", info.format);
        jobject jImageDecoder = ndk::ImageDecoder::decodeFromBitmap_unsafe(jBitmap, jctrue);
        if (!jImageDecoder) {
            // デコード失敗した
            jclog("decode fail");
            return MPixelBuffer();
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
        result.reset(mark_new NDKPixelBuffer(jPixelBuffer, imageWidth, imageHeight));

// デコーダ本体を解放する
        env->DeleteLocalRef(jImageDecoder);

        return result;
    }

#if 0
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

    // デコーダーを作成する
    Vector2i size;
    result = createPixelBufferFromImageDecoder(jImageDecoder, &size);

#ifdef DEBUG
    {
        const s32 imageWidth = size.x;
        const s32 imageHeight = size.y;
        if (imageWidth > 256 && imageHeight > 256) {
            const jcboolean mipmap = (option && option->gen_mipmap);
            jcalertf("large size image[%d x %d]->[%d x %d] mipmap(%s) uri(%s)", imageWidth, imageHeight, TextureImage::toTextureSize(option, imageWidth), TextureImage::toTextureSize(option, imageHeight), mipmap ? "true" : "false", uri.getUri().c_str());
        }
    }
#endif

    // デコーダ本体を解放する
    env->DeleteLocalRef(jImageDecoder);

    return result;
#endif
}

}
}

