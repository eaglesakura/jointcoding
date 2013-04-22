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

namespace jc {
namespace gl {

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
        s32 index = getFreeTextureUnitIndex();
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
        s32 index = getFreeTextureUnitIndex();
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

void TextureImage::copyPixelLine(const void* src, const GLenum srcPixelType, const GLenum srcPixelFormat, const s32 mipLevel, const s32 lineHeader, const s32 lineNum) {
    assert(isBinded(NULL) == jctrue);

    // 領域の確保が済んでいなければ確保する
    if (!this->alloced) {
        allocPixelMemory(srcPixelType, srcPixelFormat, mipLevel);
    }

    // 部分転送を行う
    glTexSubImage2D(GL_TEXTURE_2D, mipLevel, 0, lineHeader, size.img_width, lineNum, srcPixelFormat, srcPixelType, src);
    assert_gl();
}

/**
 * テクスチャピクセル用のメモリを確保する
 * @param srcPixelType GL_UNSIGNED_INT | GL_UNSIGNED_SHORT_5_6_5 | GL_UNSIGNED_SHORT_5_5_5_1 | GL_UNSIGNED_BYTE
 * @param srcPixelFormat GL_RGB | GL_RGBA
 */
void TextureImage::allocPixelMemory(const GLenum srcPixelType, const GLenum srcPixelFormat, const s32 miplevel) {
    assert(isBinded(NULL) == jctrue);

    if (!this->alloced) {
        this->alloced = jctrue;
        // まずは空の領域を確保する
        glTexImage2D(GL_TEXTURE_2D, 0, srcPixelFormat, size.tex_width, size.tex_height, 0, srcPixelFormat, srcPixelType, NULL);
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
        if (state->isBindedTexture(bindUnit, target, texture.get())) {
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
        this->unbind();
        texture.release();
    }
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
        return decodeFromPlatformDecoder(device, uri, pixelFormat, option);
    }
}

/**
 * PMKファイルのデコードを行う。
 */
MTextureImage TextureImage::decodePMK(MDevice device, const Uri &uri, TextureLoadOption *option) {
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
            assert_gl();
        }
        result->unbind();

        // オリジナルサイズに合わせて補正する
        result->size.img_width = header->getOriginalWidth();
        result->size.img_height = header->getOriginalHeight();
        result->alloced = jctrue;
        return result;
    }
}

}
}

#undef _BIND_CHECK
