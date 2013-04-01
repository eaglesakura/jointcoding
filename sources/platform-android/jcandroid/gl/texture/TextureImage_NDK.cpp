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

#include    "jc/gl/DeviceLock.h"
#include    "jc/platform/Timer.h"

namespace jc {
namespace gl {

using namespace ndk;

namespace {
static u32 PIXEL_TYPES[] = {
//
        GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
//
        };
static u32 PIXEL_FORMATS[] = {
//
        GL_RGB, GL_RGBA, GL_RGB, GL_RGBA, GL_BGRA_EXT,
//
        };
}

/**
 * Platformが実装しているデコーダーで画像をデコードする。
 * iOS / AndroidであればJpeg / PNG / Bitmapが共通でデコードできる
 */
MTextureImage TextureImage::decodeFromPlatformDecoder(MDevice device, const Uri &uri, const PixelFormat_e pixelFormat, TextureLoadOption *option) {
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

    {
        void* raw_buffer = env->GetDirectBufferAddress(pixelBuffer);

        jc_sa<u8> temp_buffer;

        if (pixelFormat != PixelFormat_BGRA8888) {
            jclogf("convert format(%d -> %d)", PixelFormat_BGRA8888, pixelFormat);
            temp_buffer = Pixel::createPixelBuffer(pixelFormat, imageWidth * imageHeight);
            Pixel::copyBGRA8888Pixels((const u8*) raw_buffer, pixelFormat, temp_buffer.get(), imageWidth * imageHeight);

            raw_buffer = (void*) temp_buffer.get();
        }

        jctime lock_start_time = Timer::currentTime();
        try {

            // lock
            DeviceLock lock(device, jctrue);

            const u32 origin_width = imageWidth;
            const u32 origin_height = imageHeight;

            const u32 texture_width = TextureImage::toTextureSize(option, origin_width);
            const u32 texture_height = TextureImage::toTextureSize(option, origin_height);

            // make texture
            result.reset(new TextureImage(origin_width, origin_height, device));
            result->bind();
            {
                // 同一サイズなら一括転送
                if (origin_width == texture_width && origin_height == texture_height) {
                    glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMATS[pixelFormat], texture_width, texture_height, 0, PIXEL_FORMATS[pixelFormat], PIXEL_TYPES[pixelFormat], raw_buffer);
                    jclogf("texture load(%d x %d)", imageWidth, imageHeight);
                } else {
                    glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMATS[pixelFormat], texture_width, texture_height, 0, PIXEL_FORMATS[pixelFormat], PIXEL_TYPES[pixelFormat], NULL);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, origin_width, origin_height, PIXEL_FORMATS[pixelFormat], PIXEL_TYPES[pixelFormat], raw_buffer);
                    result->size.tex_width = texture_width;
                    result->size.tex_height = texture_height;
                    jclogf("texture load(%dx%d) -> tex(%dx%d) %s", result->getWidth(), result->getHeight(), result->getTextureWidth(), result->getTextureHeight(), uri.getUri().c_str());
                }
            }

            // 必要であればmipmapを生成する
            if (result->isPowerOfTwoTexture() && option && option->gen_mipmap) {
                jclogf("gen mipmap %s", uri.getUri().c_str());
                result->genMipmaps();
                result->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
                result->setMagFilter(GL_LINEAR);
            }

            result->unbind();
            result->alloced = jctrue;
        } catch (EGLException &e) {
            // ref
            env->DeleteLocalRef(ndkImageDecoder);
            env->DeleteLocalRef(pixelBuffer);
            throw;
        }

        if (option) {
            option->result_devicelocked_time_ms = Timer::lapseTimeMs(lock_start_time);
        }
    }

    env->DeleteLocalRef(ndkImageDecoder);
    env->DeleteLocalRef(pixelBuffer);
    return result;
}

}
}

