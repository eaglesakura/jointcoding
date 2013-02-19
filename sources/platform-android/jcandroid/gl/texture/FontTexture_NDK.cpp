/*
 * FontTexture_NDK.cpp
 *
 *  Created on: 2013/02/19
 */

#include    "jointcoding-android.h"
#include    "jc/gl/FontTexture.h"
#include    "jc/platform/Platform.h"
#include    "android-classes/ndkImageDecoder.h"
#include    "android-classes/ndkAppFont.h"

namespace jc {
namespace gl {

using namespace ndk;

/**
 * フォントテクスチャを作成する
 */
//
jc_sp<FontTexture> FontTexture::createInstance(const String &text, const u32 height, MDevice device) {
    CALL_JNIENV();

    jstring java_text = c2jstring(text.c_str());
    jobject ndkImageDecoder = AppFont::createRawImage_unsafe(java_text, height);
    {
        env->DeleteLocalRef(java_text);
    }

    // ラップして、必要な情報を取り出す
    jobject pixelBuffer = ndk::ImageDecoder::getPixels_unsafe_(ndkImageDecoder);
    s32 imageWidth = ndk::ImageDecoder::getWidth_(ndkImageDecoder);
    s32 imageHeight = ndk::ImageDecoder::getHeight_(ndkImageDecoder);

    jclogf("Font Image (%x) size(%d x %d)", pixelBuffer, imageWidth, imageHeight);

    // make texture
    jc_sp<FontTexture> result(new FontTexture(imageWidth, imageHeight, device, text));
    {
        void* raw_buffer = env->GetDirectBufferAddress(pixelBuffer);
        result->bind();
        {
            result->copyPixelLine(raw_buffer, GL_UNSIGNED_BYTE, GL_RGBA, 0, 0, imageHeight);
        }
        result->unbind();
    }

    env->DeleteLocalRef(ndkImageDecoder);
    env->DeleteLocalRef(pixelBuffer);
    return result;
}

}
}

