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

    jctime decode_starttime = Timer::currentTime();
    MInputStream stream = Platform::getFileSystem()->openInputStream(uri);
    JavaJointInputStream *pJJInputStream = dynamic_cast<JavaJointInputStream*>(stream.get());
    if (!pJJInputStream) {
        jclogf("Uri(%s) not java joint", uri.getUri().c_str());
        return result;
    }

    jobject jImageDecoder = ndk::ImageDecoder::decodeFromStream_unsafe(pJJInputStream->getJniStream()->getObject());
    if (!jImageDecoder) {
        // デコード失敗した
        jclog("decode fail...");
        return result;
    }

    if (option) {
        option->result.raw_load_time_ms = Timer::lapseTimeMs(decode_starttime);
    }

    // ラップして、必要な情報を取り出す
    jobject jPixelBuffer = ndk::ImageDecoder::getPixels_unsafe_(jImageDecoder);
    s32 imageWidth = ndk::ImageDecoder::getWidth_(jImageDecoder);
    s32 imageHeight = ndk::ImageDecoder::getHeight_(jImageDecoder);

    // 仮ポインタ
    jcboolean cancel_flag = jcfalse;
    jcboolean *cancel_ptr = option ? &option->load_cancel : &cancel_flag;

    // デバイスの制御待ちチェック
#define     device_wait(option) if(option && option->load_priority_down){ device->waitLockRequest(1, cancel_ptr); }

    const GLenum TEXTURE_PIXEL_FORMAT = PIXEL_FORMATS[pixelFormat];
    const GLenum TEXTURE_PIXEL_TYPE = PIXEL_TYPES[pixelFormat];

    {
        u8* raw_buffer = (u8*) env->GetDirectBufferAddress(jPixelBuffer);

        jc_sa<u8> temp_buffer;

        if (pixelFormat != PixelFormat_NativeRGBA) {
            jctime start_time = Timer::currentTime();

            jclogf("convert format(%d -> %d)", PixelFormat_NativeRGBA, pixelFormat);
            temp_buffer = Pixel::createPixelBuffer(pixelFormat, imageWidth * imageHeight);
            Pixel::copyBGRA8888Pixels((const u8*) raw_buffer, pixelFormat, temp_buffer.get(), imageWidth * imageHeight);

            raw_buffer = (u8*) temp_buffer.get();

            if (option) {
                option->result.raw_pixelconvert_time_ms = Timer::lapseTimeMs(start_time);
            }
        } else {
            if (option) {
                option->result.raw_pixelconvert_time_ms = 0;
            }
        }

        const u32 origin_width = imageWidth;
        const u32 origin_height = imageHeight;

        const u32 texture_width = TextureImage::toTextureSize(option, origin_width);
        const u32 texture_height = TextureImage::toTextureSize(option, origin_height);

        // テクスチャを生成する
        try {
            jctime lock_time = Timer::currentTime();

            // ロック優先度が低いなら、他のデバイスの制御待ちを行う
            device_wait(option);

            // lock
            DeviceLock lock(device, jctrue);

            // デバイスロック時間の記録
            if (option) {
                option->result.devicelocked_time_ms = Timer::lapseTimeMs(lock_time);
            }
            lock_time = Timer::currentTime();

            // テクスチャを作成する
            result.reset(new TextureImage(texture_width, texture_width, device));

            result->size.img_width = origin_width;
            result->size.img_height = origin_height;
            result->size.tex_width = texture_width;
            result->size.tex_height = texture_height;

            // テクスチャ用メモリを確保する
            result->bind();
            result->allocPixelMemory(TEXTURE_PIXEL_TYPE, TEXTURE_PIXEL_FORMAT, 0);

            // テクスチャalloc時間を記録する
            if (option) {
                option->result.alloc_time_ms = Timer::lapseTimeMs(lock_time);
                option->result.teximage_time_ms = 0;
            }
        } catch (Exception &e) {
            jcloge(e);
            // ref
            env->DeleteLocalRef(jImageDecoder);
            env->DeleteLocalRef(jPixelBuffer);
            throw;
        }

        // 分割読み込み数を設定する
        u32 slice_loading_num = 1;
        if (option && option->slice_loading > 0) {
            slice_loading_num = option->slice_loading;
        }
        s32 pixel_y = 0;
        s32 LOAD_HEIGHT = origin_height;

        if (option && option->slice_loading > 0) {
            LOAD_HEIGHT /= option->slice_loading;
            if (LOAD_HEIGHT == 0) {
                LOAD_HEIGHT = 1;
            }
        }

        while (pixel_y < origin_height && !(*cancel_ptr)) {
            try {
                jctime lock_time = Timer::currentTime();

                // ロック優先度が低いなら、他のデバイスの制御待ちを行う
                device_wait(option);

                // lock
                DeviceLock lock(device, jctrue);

                if (option) {
                    option->result.devicelocked_time_ms += Timer::lapseTimeMs(lock_time);
                }

                lock_time = Timer::currentTime();

                // 読み込むライン数を修正する
                if ((pixel_y + LOAD_HEIGHT) > origin_height) {
                    LOAD_HEIGHT = (origin_height - pixel_y);
                }

                assert(LOAD_HEIGHT > 0);
                assert((pixel_y + LOAD_HEIGHT) <= origin_height);

                result->bind();
                result->copyPixelLine(raw_buffer, TEXTURE_PIXEL_TYPE, TEXTURE_PIXEL_FORMAT, 0, pixel_y, LOAD_HEIGHT);
                if (option) {
                    option->result.teximage_time_ms += Timer::lapseTimeMs(lock_time);
                }
            } catch (EGLException &e) {
                jcloge(e);
                // ref
                env->DeleteLocalRef(jImageDecoder);
                env->DeleteLocalRef(jPixelBuffer);
                throw;
            }

            // 画像ヘッダを移動する
            raw_buffer += (LOAD_HEIGHT * origin_width * 4);
            pixel_y += LOAD_HEIGHT;

            if (option && option->slice_sleep_time_ms > 0 && pixel_y < origin_height) {
                Thread::sleep(option->slice_sleep_time_ms);
            }
        }

        // delete ref
        {
            env->DeleteLocalRef(jImageDecoder);
            env->DeleteLocalRef(jPixelBuffer);

            jImageDecoder = NULL;
            jPixelBuffer = NULL;
        }

        // 必要であればmipmapを生成する
        if (option && option->gen_mipmap && result->isPowerOfTwoTexture()) {
            try {
                jctime lock_time = Timer::currentTime();

                // ロック優先度が低いなら、他のデバイスの制御待ちを行う
                device_wait(option);

                // lock
                DeviceLock lock(device, jctrue);

                if (option) {
                    option->result.devicelocked_time_ms += Timer::lapseTimeMs(lock_time);
                }

                jclogf("gen mipmap %s", uri.getUri().c_str());
                result->genMipmaps();
                result->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
                result->setMagFilter(GL_LINEAR);

                if (option) {
                    option->result.teximage_time_ms += Timer::lapseTimeMs(lock_time);
                }
            } catch (Exception &e) {
                jcloge(e);
                throw;
            }

        }
    }

    return result;
}

}
}

