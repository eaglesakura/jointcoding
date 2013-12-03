/*
 * TextureImage.cpp
 *
 *  Created on: 2012/07/22

 */
#include    "jc/math/Math.h"
#include    "jc/platform/Platform.h"
#include    "jc/gl/texture/PKMHeader.h"
#include    "jc/gl/texture/TextureImage.h"
#include    "jc/gl/gpu/DeviceLock.hpp"
#include    "jc/platform/Timer.h"

namespace jc {
namespace gl {

namespace {
static u32 PIXEL_TYPES[] = {
//
        GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
//
        };
static u32 PIXEL_FORMATS[] = {
//
#ifdef BUILD_Android
        GL_RGB, GL_RGBA, GL_RGB, GL_RGBA, GL_BGRA_EXT,
#else
        GL_RGB, GL_RGBA, GL_RGB, GL_RGBA, GL_RGBA,
#endif
//
    };
}

TextureImage::TextureImage(const s32 width, const s32 height, MDevice device) {
    this->alloced = jcfalse;
    size.img_width = size.tex_width = width;
    size.img_height = size.tex_height = height;
    this->state = device->getState();
    this->target = GL_TEXTURE_2D;
    bindUnit = -1;
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    texture.alloc(device->getVRAM(), VRAM_Texture);

    {
        const s32 index = GPUCapacity::getMaxTextureUnits() - 1;
        this->bind(index);
        {
            // npotでは GL_CLAMP_TO_EDGE を指定する必要がある
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            context.wrapS = GL_CLAMP_TO_EDGE;
            context.wrapT = GL_CLAMP_TO_EDGE;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            context.magFilter = GL_NEAREST;
            context.minFilter = GL_NEAREST;

            assert_gl();
        }
        this->unbind();
    }
}

TextureImage::TextureImage(const GLenum target, const s32 width, const s32 height, MDevice device) {
    this->alloced = jcfalse;
    size.img_width = size.tex_width = width;
    size.img_height = size.tex_height = height;
    this->state = device->getState();
    this->target = target;
    bindUnit = -1;
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    texture.alloc(device->getVRAM(), VRAM_Texture);

    {
        const s32 index = GPUCapacity::getMaxTextureUnits() - 1;
        this->bind(index);
        {
            // npotでは GL_CLAMP_TO_EDGE を指定する必要がある
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            context.wrapS = GL_CLAMP_TO_EDGE;
            context.wrapT = GL_CLAMP_TO_EDGE;

            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            context.magFilter = GL_NEAREST;
            context.minFilter = GL_NEAREST;

            assert_gl();
        }
        this->unbind();
    }
}

TextureImage::~TextureImage() {
    this->dispose();
}

s32 TextureImage::getFreeTextureUnitIndex() {
    return state->getFreeTextureUnitIndex(jctrue);
}

void TextureImage::copyPixelLine(const void* src, const PixelFormat_e pixelFormat, const s32 mipLevel, const s32 lineHeader, const s32 lineNum) {
    assert(isBinded(NULL) == jctrue);
    assert(state->getActiveTextureIndex() == bindUnit);

    // 領域の確保が済んでいなければ確保する
    if (!this->alloced) {
        allocPixelMemory(pixelFormat, mipLevel);
    }

    // 部分転送を行う
    glTexSubImage2D(GL_TEXTURE_2D, mipLevel, 0, lineHeader, size.img_width, lineNum, PIXEL_FORMATS[pixelFormat], PIXEL_TYPES[pixelFormat], src);
    assert_gl();
}

/**
 * テクスチャピクセル用のメモリを確保する
 * @param srcPixelType GL_UNSIGNED_INT | GL_UNSIGNED_SHORT_5_6_5 | GL_UNSIGNED_SHORT_5_5_5_1 | GL_UNSIGNED_BYTE
 * @param srcPixelFormat GL_RGB | GL_RGBA
 */
void TextureImage::allocPixelMemory(const PixelFormat_e pixelFormat, const s32 miplevel) {
    assert(isBinded(NULL) == jctrue);
    assert(state->getActiveTextureIndex() == bindUnit);

    if (!this->alloced) {
        this->alloced = jctrue;
        // まずは空の領域を確保する
        glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMATS[pixelFormat], size.tex_width, size.tex_height, 0, PIXEL_FORMATS[pixelFormat], PIXEL_TYPES[pixelFormat], NULL);
        assert_gl();
    }
}

/**
 * Minフィルタを変更する
 */
void TextureImage::setMinFilter(GLint filter) {
    assert(isBinded(NULL) == jctrue);

    if (context.minFilter == filter) {
        return;
    }

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
    assert_gl();
    context.minFilter = filter;
}

/**
 * Magフィルタを変更する
 */
void TextureImage::setMagFilter(GLint filter) {
    assert(isBinded(NULL) == jctrue);

    if (context.magFilter == filter) {
        return;
    }

    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
    assert_gl();
    context.magFilter = filter;
}

/**
 * ラップモードを設定する
 */
void TextureImage::setWrapS(GLint wrap) {
    assert(isBinded(NULL) == jctrue);
    if (context.wrapS == wrap) {
        return;
    }

    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
    context.wrapS = wrap;
}

/**
 * ラップモードを設定する
 */
void TextureImage::setWrapT(GLint wrap) {
    assert(isBinded(NULL) == jctrue);
    if (context.wrapT == wrap) {
        return;
    }

    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
    assert_gl();
    context.wrapT = wrap;

}

/**
 * mipmapを自動生成する
 */
void TextureImage::genMipmaps() {
    if (!isPowerOfTwoTexture()) {
        // mipmapが生成できなくても表示上は問題ないため、npotログだけ吐き出して終了
        jclogf("texture is non power of two %d x %d", size.tex_width, size.tex_height);
        return;
    }
    assert(isBinded(NULL) == jctrue);
    glGenerateMipmap(target);
}

/**
 * NPOTテクスチャの場合trueを返す。
 */
jcboolean TextureImage::isPowerOfTwoTexture() {
    if (jc::isPowerOfTwo(size.tex_width) && jc::isPowerOfTwo(size.tex_height)) {
        return jctrue;
    } else {
        return jcfalse;
    }
}

s32 TextureImage::bind() {
    if (bindUnit >= 0) {
        assert(bindUnit >= 0 && bindUnit < GPUCapacity::getMaxTextureUnits());

        // バインドキャッシュがある場合は、まだバインドが有効かをチェックする
        if (state->isBindedTexture(bindUnit, target, texture.get())) {
            // まだバインドされていたらactiveへ切り替える
            state->activeTexture(bindUnit);
            return bindUnit;
        }
        bindUnit = -1;
    }

    s32 unitIndex = getFreeTextureUnitIndex();
//    unitIndex = 0;
    this->bind(unitIndex);
    return unitIndex;
}

/**
 * テクスチャをindex番のユニットに関連付ける
 */
void TextureImage::bind(s32 index) {
    if (!texture.exist()) {
        jclogf("texture is not exist(%x)", this);
        return;
    }

    assert(index >= 0 && index < GPUCapacity::getMaxTextureUnits());

    if (bindUnit == index) {
        if (state->isBindedTexture(bindUnit, target, texture.get())) {
            state->activeTexture(bindUnit);
            return;
        }
    } else if (bindUnit >= 0) {
        unbind();
    }
    bindUnit = index;
    this->state->activeTexture(index);
    this->state->bindTexture(target, texture.get());
}

/**
 * テクスチャをユニットから切り離す
 */
void TextureImage::unbind() {
    this->state->unbindTexture(texture.get());
    bindUnit = -1;
}

/**
 * バインド済みかどうかを確認する。
 * バインドされている場合jctrueを返す。
 * resultIndexが設定されている場合、bindedならインデックスを返す。
 */
jcboolean TextureImage::isBinded(s32 *resultIndex) {
    if (bindUnit < 0) {
        // テクスチャがバインドされていない
        return jcfalse;
    }

    if (resultIndex) {
        (*resultIndex) = bindUnit;
    }
    return jctrue;
}

/**
 * 管理している資源を開放する
 */
void TextureImage::dispose() {
    if (texture.exist()) {
        texture.release();
    }
}

/**
 * 1x1dotのシンプルなテクスチャを生成する
 */
jc_sp<TextureImage> TextureImage::createDotTexture2D(MDevice device, const Color color) {

    MTextureImage result(mark_new TextureImage(1, 1, device));
    result->bind();
    {
        if(color.a() == 0xFF) {
            // alphaが不要
            // 頭3byte=RGBだけを使ってもらう
            result->copyPixelLine(&color, PixelFormat_RGB888, 0, 0, 1);
        } else {
            // alphaが必要
            result->copyPixelLine(&color, PixelFormat_RGBA8888, 0, 0, 1);
        }
    }
    result->unbind();

    return result;
}

/**
 * テクスチャへのデコードを行う。
 * uriにはJpegテクスチャへのURIを指定する。
 */ //
MTextureImage TextureImage::decode(MDevice device, const Uri &uri, const PixelFormat_e pixelFormat, TextureLoadOption *option) {
    const String ext = uri.getFileExt();
    if (ext == JC_TEXTURE_EXT_PKM) {
        jclogf("pkm texture(%s)", uri.getUri().c_str());
        return decodePMK(device, uri, option);
    } else {
        jclogf("platform texture(%s)", uri.getUri().c_str());
        MPixelBuffer buffer = TextureImage::decodePixelsFromPlatformDecoder(device, uri, option);
        if (buffer) {
            return decode(device, buffer, pixelFormat, option);
        }
        return MTextureImage();
    }
}

/**
 * ピクセルバッファからデコードを行う
 * pixelBufferはpixelFormatのフォーマットへ変換を行った上で転送を行う
 */ //
jc_sp<TextureImage> TextureImage::decode(MDevice device, MPixelBuffer pixelBuffer, const PixelFormat_e pixelFormat, TextureLoadOption *option) {
    jc_sp<TextureImage> result;

    // ピクセルコンバートを行う
    pixelBuffer->convert(pixelFormat);

    // キャンセルチェックポインタを作成
    jcboolean cancel_flag = jcfalse;
    jcboolean *cancel_ptr = option ? &option->load_cancel : &cancel_flag;

    const s32 texture_unit = GPUCapacity::getMaxTextureUnits() - 1;
    assert(texture_unit >= 0);

    // デバイスの制御待ちチェック
#define     device_wait(option) if(option && option->load_priority_down){ device->waitLockRequest(1, cancel_ptr); }

    // bytes/pixをチェックする
    const s32 ONCE_PIXEL_BYTES = Pixel::getPixelBytes(pixelFormat);
    {
        const u32 origin_width = pixelBuffer->getWidth();
        const u32 origin_height = pixelBuffer->getHeight();

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
            result.reset(mark_new TextureImage(texture_width, texture_width, device));

            result->size.img_width = origin_width;
            result->size.img_height = origin_height;
            result->size.tex_width = texture_width;
            result->size.tex_height = texture_height;

            // mipmapを生成する場合は正方形に整列する
            if (option && option->gen_mipmap) {
                result->size.tex_width = jc::max(texture_width, texture_height);
                result->size.tex_height = result->size.tex_width;
            }

            // テクスチャ用メモリを確保する
            result->bind(texture_unit);
            result->allocPixelMemory(pixelFormat, 0);

            // glTexImage2D用にパッキングを行う
            // この呼出を行わない場合、テクセル境界が4byteとなってしまう
            // 例えば2byte RGB565テクスチャの転送で4byte境界にされてしまい、テクスチャがうまいこと読み込めなくなってしまう
            if (ONCE_PIXEL_BYTES == 4 || ONCE_PIXEL_BYTES == 2) {
                glPixelStorei(GL_UNPACK_ALIGNMENT, ONCE_PIXEL_BYTES);
                glPixelStorei(GL_PACK_ALIGNMENT, ONCE_PIXEL_BYTES);
            } else {
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
            }

            // テクスチャalloc時間を記録する
            if (option) {
                option->result.alloc_time_ms = Timer::lapseTimeMs(lock_time);
                option->result.teximage_time_ms = 0;
            }

            // テクスチャロードを開始する
            result->unbind();
            glFinish();
        } catch (Exception &e) {
            jcloge(e);
            throw;
        }

        // 分割読み込み数を設定する
        s32 pixel_y = 0;
        s32 LOAD_HEIGHT = origin_height;

        // １回ごとに読み込むピクセル数を設定する
        if (option && option->slice_loading_pixel > 0) {
            LOAD_HEIGHT = jc::max<s32>(option->slice_loading_pixel / origin_width, 32);
        }

        // 読込先Y位置が画像高さよりも小さければロード続行する
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
                LOAD_HEIGHT = jc::min<s32>(LOAD_HEIGHT, origin_height - pixel_y);
                assert(LOAD_HEIGHT > 0);
                assert((pixel_y + LOAD_HEIGHT) <= origin_height);

                result->bind(texture_unit);
                result->copyPixelLine(pixelBuffer->getPixelHeader(), pixelFormat, 0, pixel_y, LOAD_HEIGHT);
                result->unbind();
                // テクスチャロードはfinish待ちを行う
                glFinish();

                if (option) {
                    option->result.teximage_time_ms += Timer::lapseTimeMs(lock_time);
                }
            } catch (EGLException &e) {
                jcloge(e);
                throw;
            }

            // 画像ヘッダを移動する
            pixelBuffer->nextLine(LOAD_HEIGHT);
            pixel_y += LOAD_HEIGHT;

            if (option && option->slice_sleep_time_ms > 0 && pixel_y < origin_height) {
                Thread::sleep(option->slice_sleep_time_ms);
            }
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

                jclog("gen mipmap");
                result->bind(texture_unit);
                result->genMipmaps();
                result->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
                result->setMagFilter(GL_LINEAR);
                result->unbind();

                // テクスチャロードはfinish待ちを行う
                glFinish();
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

#undef  device_wait
    return result;
}

/**
 * PMKファイルのデコードを行う。
 */
MTextureImage TextureImage::decodePMK(MDevice device, const Uri &uri, TextureLoadOption *option) {
#ifndef GL_ETC1_RGB8_OES
    // not support ETC1
    return MTextureImage();
#else
    MInputStream is = Platform::getFileSystem()->openInputStream(uri);
    if (!is) {
        jcalertf("file not found(%s)", uri.getUri().c_str());
        return MTextureImage();
    }

    MPKMHeader header = PKMHeader::createInstance(is);

    if (!header) {
        jcalertf("not pkm file(%s)", uri.getUri().c_str());
        return MTextureImage();
    }

    if (!header->isPlatformSupportTexture()) {
        jclogf("unsupported Pixel Format(%s) Type(%d)", uri.getUri().c_str(), header->getDataType());
        return MTextureImage();
    }

    const s32 width = header->getWidth();
    const s32 height = header->getHeight();

    {
        DeviceLock lock(device, jctrue);

        MTextureImage result(mark_new TextureImage(width, height, device));
        result->bind();
        {
            u32 length = 0;
            jc_sa<u8> temp = InputStream::toByteArray(is, &length);

            jclogf("etc1 size(%d x %d)", width, height);
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, header->getWidth(), header->getHeight(), 0, length, (void*) temp.get());
            assert_gl();
        }
        result->unbind();

        // オリジナルサイズに合わせて補正する
        result->size.img_width = header->getOriginalWidth();
        result->size.img_height = header->getOriginalHeight();
        result->alloced = jctrue;
        return result;
    }
#endif
}

}
}

#undef _BIND_CHECK
