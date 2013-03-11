/*
 * TextureImage.cpp
 *
 *  Created on: 2012/07/22

 */
#include    "jc/gl/GL.h"
#include    "jc/gl/TextureImage.h"
#include    "jc/math/Math.h"
#include    "jc/platform/Platform.h"
#include    "jc/graphics/jpeg/JpegImageDecoder.h"
#include    "jc/gl/State.h"
#include    "jc/gl/GPUCapacity.h"

#include    "jc/gl/DeviceLock.h"
#include    "jc/gl/PKMHeader.h"

#define _BIND_CHECK(_this)      { if(_this->bindUnit < 0 ){ jclogf("texture not bind | this=( %x ) texture=( %d ) abort.", _this, _this->texture.get()); return; } }

namespace jc {
namespace gl {

/**
 * 強制的に持ち回りでテクスチャユニットを上書きする
 */
static s32 g_overrideTextureUnitIndex = 0;

TextureImage::TextureImage(const s32 width, const s32 height, MDevice device) {
    this->alloced = jcfalse;
    this->width = width;
    this->height = height;
    this->state = device->getState();
    this->target = GL_TEXTURE_2D;
    bindUnit = -1;
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    texture.alloc(device->getVRAM(), VRAM_Texture);

    {
        s32 index = state->getFreeTextureUnitIndex();
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
        }
        this->unbind();
    }
}

TextureImage::TextureImage(const GLenum target, const s32 width, const s32 height, MDevice device) {
    this->alloced = jcfalse;
    this->width = width;
    this->height = height;
    this->state = device->getState();
    this->target = target;
    bindUnit = -1;
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    texture.alloc(device->getVRAM(), VRAM_Texture);

    {
        s32 index = state->getFreeTextureUnitIndex();
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
        }
        this->unbind();
    }
}

TextureImage::~TextureImage() {
    this->dispose();
}

void TextureImage::copyPixelLine(const void* src, const GLenum srcPixelType, const GLenum srcPixelFormat, const s32 mipLevel, const s32 lineHeader, const s32 lineNum) {
    _BIND_CHECK(this);
    // 空いているユニットを取得する
    jcboolean finished = jcfalse;

    // 領域の確保が済んでいなければ確保する
    if (!this->alloced) {
        this->alloced = jctrue;

        if (lineHeader == 0 && lineNum == (s32) height) {
            // 一度に転送しきれる場合は全て転送してしまう
            glTexImage2D(GL_TEXTURE_2D, 0, srcPixelFormat, width, height, 0, srcPixelFormat, srcPixelType, src);
            finished = jctrue;
        } else {
            // まずは空の領域を確保する
            glTexImage2D(GL_TEXTURE_2D, 0, srcPixelFormat, width, height, 0, srcPixelFormat, srcPixelType, NULL);
            finished = jcfalse;
        }
    }

    // 部分転送が必要なら転送する
    if (!finished) {
        glTexSubImage2D(GL_TEXTURE_2D, mipLevel, 0, lineHeader, width, lineNum, srcPixelFormat, srcPixelType, src);
    }
}

/**
 * テクスチャピクセル用のメモリを確保する
 * @param srcPixelType GL_UNSIGNED_INT | GL_UNSIGNED_SHORT_5_6_5 | GL_UNSIGNED_SHORT_5_5_5_1 | GL_UNSIGNED_BYTE
 * @param srcPixelFormat GL_RGB | GL_RGBA
 */
void TextureImage::allocPixelMemory(const GLenum srcPixelType, const GLenum srcPixelFormat, const s32 miplevel) {
    _BIND_CHECK(this);

    if (!this->alloced) {
        this->alloced = jctrue;
        // まずは空の領域を確保する
        glTexImage2D(GL_TEXTURE_2D, 0, srcPixelFormat, width, height, 0, srcPixelFormat, srcPixelType, NULL);
    }
}

/**
 * Minフィルタを変更する
 */
void TextureImage::setMinFilter(GLint filter) {
    _BIND_CHECK(this);
    if (context.minFilter == filter) {
        return;
    }

    if (this->isNonPowerOfTwo()) {
        jclogf("texture is non power of two %d x %d", width, height);
    }
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
    context.minFilter = filter;
}

/**
 * Magフィルタを変更する
 */
void TextureImage::setMagFilter(GLint filter) {
    _BIND_CHECK(this);
    if (context.magFilter == filter) {
        return;
    }

    if (this->isNonPowerOfTwo()) {
        jclogf("texture is non power of two %d x %d", width, height);
    }
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
    context.magFilter = filter;
}

/**
 * ラップモードを設定する
 */
void TextureImage::setWrapS(GLint wrap) {
    _BIND_CHECK(this);
    if (context.wrapS == wrap) {
        return;
    }

    if (this->isNonPowerOfTwo()) {
        jclogf("texture is non power of two %d x %d", width, height);
    }

    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap);
}

/**
 * ラップモードを設定する
 */
void TextureImage::setWrapT(GLint wrap) {
    _BIND_CHECK(this);
    if (context.wrapT == wrap) {
        return;
    }

    if (this->isNonPowerOfTwo()) {
        jclogf("texture is non power of two %d x %d", width, height);
    }

    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap);
}

/**
 * NPOTテクスチャの場合trueを返す。
 */
jcboolean TextureImage::isNonPowerOfTwo() {
    if (jc::isPowerOfTwo(width) && jc::isPowerOfTwo(height)) {
        return jcfalse;
    } else {
        return jctrue;
    }
}

s32 TextureImage::bind() {
    if (bindUnit >= 0) {
        if (state->isBindedTexture(bindUnit, target, texture.get())) {
            state->activeTexture(bindUnit);
            return bindUnit;
        }
        bindUnit = -1;
    }

    s32 unitIndex = state->getFreeTextureUnitIndex();

    if (unitIndex < 0) {
        unitIndex = (++g_overrideTextureUnitIndex % (s32) GPUCapacity::getMaxTextureUnits());
        jclogf("texture unit = %d", unitIndex);
    }
    this->bind(unitIndex);
    return unitIndex;
}

/**
 * テクスチャをindex番のユニットに関連付ける
 */
void TextureImage::bind(s32 index) {
    if (bindUnit == index) {
        if (state->isBindedTexture(bindUnit, target, texture.get())) {
            state->activeTexture(index);
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
        this->unbind();
        texture.release();
    }
}

#if 0
/**
 * 2byte
 * GL_UNSIGNED_SHORT_5_6_5
 */
PixelFormat_RGB565,

/**
 * 2byte
 * GL_UNSIGNED_SHORT_5_5_5_1
 */
PixelFormat_RGBA5551,

/**
 * 3byte
 * GL_UNSIGNED_BYTE
 */
PixelFormat_RGB888,

/**
 * 4byte
 * GL_UNSIGNED_BYTE
 */
PixelFormat_RGBA8888,
#endif

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
 * デコーダーを通して、テクスチャ化を行う。
 */
MTextureImage TextureImage::decode(MDevice device, MImageDecoder decoder, const PixelFormat_e pixelFormat) {
// 適当なラインずつ設定を行う。
    const s32 width = decoder->getWidth();
    const s32 height = decoder->getHeight();

    if (width <= 0 || height <= 0) {
        jcalertf("texture size error(%d x %d)", width, height);
        return MTextureImage();
    }

    MTextureImage result(new TextureImage(width, height, device));

    result->bind();
    // 読み込んだ合計ライン数
    s32 readed = 0;
    // カレントのライン数
    s32 line = 0;
    // 可能な限り読み込み続ける
    CLEAR_GL_ERROR
    ;
    while ((line = decoder->decode()) > 0) {
        result->copyPixelLine(decoder->getPixels().get(), PIXEL_TYPES[pixelFormat], PIXEL_FORMATS[pixelFormat], 0, readed, line);
        readed += line;
    }
    PRINT_GL_ERROR;
    result->unbind();

    return result;
}

/**
 * テクスチャへのデコードを行う。
 * uriにはJpegテクスチャへのURIを指定する。
 */ //
MTextureImage TextureImage::decode(MDevice device, const Uri &uri, const PixelFormat_e pixelFormat) {

    {
        const String ext = uri.getFileExt();
        if (ext == JC_TEXTURE_EXT_ETC1) {
            jclogf("pkm texture(%s)", uri.getUri().c_str());
            return decodePMK(device, uri);
        } else if (ext != JC_TEXTURE_EXT_JPEG) {
            jclogf("platform texture(%s)", uri.getUri().c_str());
            return decodeFromPlatformDecoder(device, uri, pixelFormat);
        }
    }

    MInputStream is = Platform::getFileSystem()->openInputStream(uri);
    if (!is) {
        jcalertf("file not found %s", uri.getUri().c_str());
        return MTextureImage();
    }

    MImageDecoder decoder(new JpegImageDecoder(is));
    // ヘッダを解析する
    decoder->decodeHeader();
    // 適当なライン数だけデコードする
    decoder->decodeBegin(pixelFormat, 32);

    // 実際のデコードを行う
    MTextureImage result = TextureImage::decode(device, decoder, pixelFormat);

    // デコーダーを削除
    decoder->dispose();

    return result;
}

/**
 * PMKファイルのデコードを行う。
 */
MTextureImage TextureImage::decodePMK(MDevice device, const Uri &uri) {
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

        MTextureImage result(new TextureImage(width, height, device));
        result->bind();
        {
            u32 length = 0;
            jc_sa<u8> temp = InputStream::toByteArray(is, &length);

            jclogf("etc1 size(%d x %d)", width, height);
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, header->getWidth(), header->getHeight(), 0, length, (void*) temp.get());
        }
        result->unbind();

        return result;
    }
}

}
}
