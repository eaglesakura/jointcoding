/*
 * TextureImage_NDK.cpp
 *
 *  Created on: 2013/02/15
 */

#include    "jointcoding-android.h"
#include    "jc/gl/TextureImage.h"
#include    "jc/platform/Platform.h"
#include    "android-classes/ndkImageDecoder.h"
#include    "jcandroid/io/JavaJointInputStream.h"

namespace jc {
namespace gl {

using namespace ndk;

namespace {
static u32 PIXEL_TYPES[4] = {
//
        GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
//
        };
static u32 PIXEL_FORMATS[] = {
//
        GL_RGB, GL_RGBA, GL_RGB, GL_RGBA,
//
        };
}

/**
 * Platformが実装しているデコーダーで画像をデコードする。
 * iOS / AndroidであればJpeg / PNG / Bitmapが共通でデコードできる
 */
MTextureImage TextureImage::decodeFromPlatformDecoder(MDevice device, const Uri &uri, const PixelFormat_e pixelFormat) {
    MTextureImage result;
    CALL_JNIENV();

    MInputStream stream = Platform::getFileSystem()->openInputStream(uri);
    JavaJointInputStream *pJJInputStream = dynamic_cast<JavaJointInputStream*>(stream.get());
    if (!pJJInputStream) {
        jclogf("Uri(%s) not java joint", uri.getUri().c_str());
        return result;
    }

    jobject ndkImageDecoder = ndk::ImageDecoder::decodeFromStream_unsafe(pJJInputStream->getJniStream()->getObject());
    if (!ndkImageDecoder) {
        // デコード失敗した
        jclog("decode fail...");
        return result;
    }

    // ラップして、必要な情報を取り出す
    jobject pixelBuffer = ndk::ImageDecoder::getPixels_unsafe_(ndkImageDecoder);
    s32 imageWidth = ndk::ImageDecoder::getWidth_(ndkImageDecoder);
    s32 imageHeight = ndk::ImageDecoder::getHeight_(ndkImageDecoder);

    jclogf("Image (%x) size(%d x %d)", pixelBuffer, imageWidth, imageHeight);

    {
        void* raw_buffer = env->GetDirectBufferAddress(pixelBuffer);

        jc_sa<u8> temp_buffer;

        if (pixelFormat != PixelFormat_RGBA8888) {
            jclogf("convert format(%d -> %d)", PixelFormat_RGBA8888, pixelFormat);
            temp_buffer = Pixel::createPixelBuffer(pixelFormat, imageWidth * imageHeight);
            Pixel::copyRGBA8888Pixels((const u8*)raw_buffer, pixelFormat, temp_buffer.get(), imageWidth * imageHeight);

            raw_buffer = (void*)temp_buffer.get();
        }

        // make texture
        result.reset(new TextureImage(imageWidth, imageHeight, device));

        result->bind();
        {
            CLEAR_GL_ERROR
            ;
            result->copyPixelLine(raw_buffer, PIXEL_TYPES[pixelFormat], PIXEL_FORMATS[pixelFormat], 0, 0, imageHeight);

            PRINT_GL_ERROR;
        }
        result->unbind();
    }

    env->DeleteLocalRef(ndkImageDecoder);
    env->DeleteLocalRef(pixelBuffer);
    return result;
}

}
}

