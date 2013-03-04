/*
 * jcTextureImage.h
 *
 *  Created on: 2012/07/22

 */

#ifndef JCTEXTUREIMAGE_H_
#define JCTEXTUREIMAGE_H_

#include    "jc/gl/GL.h"
#include    "jc/graphics/ImageDecoder.h"
#include    "jc/gl/Device.h"
#include    "jc/io/Uri.h"

namespace jc {
namespace gl {

/**
 * ETC1テクスチャの拡張子（PKM形式)
 */
#define JC_TEXTURE_EXT_ETC1 "pkm"

/**
 * Jpegテクスチャの拡張子
 */
#define JC_TEXTURE_EXT_JPEG "jpg"

/**
 * OpenGLで扱うテクスチャを管理する。
 * GLES2.0を基本とするため、テクスチャサイズは2^n以外も可能だが、パフォーマンスには影響する
 */
class TextureImage: public Object {
    /**
     * テクスチャ幅
     */
    u32 width;

    /**
     * テクスチャ高さ
     */
    u32 height;

    /**
     * テクスチャユニット本体
     */
    SharedResource texture;

    /**
     * GLのステート保持
     */
    MGLState state;

    /**
     * バインドしたユニット番号
     */
    s32 bindUnit;

    /**
     * テクスチャ領域を確保済みならtrue
     */
    jcboolean alloced;

    /**
     * 状態管理
     */
    struct {
        /**
         * GL_TEXTURE_MAG_FILTER
         * default = GL_NEAREST
         */
        GLint minFilter;

        /**
         * GL_TEXTURE_MIN_FILTER
         * default = GL_NEAREST
         */
        GLint magFilter;

        /**
         * GL_TEXTURE_WRAP_S
         * default = GL_CLAMP_TO_EDGE
         */
        GLint wrapS;

        /**
         * GL_TEXTURE_WRAP_T
         * default = GL_CLAMP_TO_EDGE
         */
        GLint wrapT;
    } context;

public:
    TextureImage(const s32 width, const s32 height, MDevice device);

    TextureImage(const GLenum target, const width, const height, MDevice device);

    virtual ~TextureImage();

    /**
     * テクスチャ幅
     */
    virtual u32 getWidth() const {
        return width;
    }

    /**
     * テクスチャ高さ
     */
    virtual u32 getHeight() const {
        return height;
    }

    /**
     * NPOTテクスチャの場合trueを返す。
     */
    virtual jcboolean isNonPowerOfTwo();

    /**
     * Minフィルタを変更する
     */
    virtual void setMinFilter(GLint filter);

    /**
     * Magフィルタを変更する
     */
    virtual void setMagFilter(GLint filter);

    /**
     * ラップモードを設定する
     */
    virtual void setWrapS(GLint wrap);

    /**
     * ラップモードを設定する
     */
    virtual void setWrapT(GLint wrap);

    /**
     * 水平ラインごとにピクセルをコピーする
     * @param src コピー元のピクセル情報
     * @param srcPixelType GL_UNSIGNED_INT | GL_UNSIGNED_SHORT_5_6_5 | GL_UNSIGNED_SHORT_5_5_5_1 | GL_UNSIGNED_BYTE
     * @param srcPixelFormat GL_RGB | GL_RGBA
     */
    virtual void copyPixelLine(const void* src, const GLenum srcPixelType, const GLenum srcPixelFormat, const s32 mipLevel, const s32 lineHeader, const s32 lineNum);

    /**
     * テクスチャピクセル用のメモリを確保する
     * @param srcPixelType GL_UNSIGNED_INT | GL_UNSIGNED_SHORT_5_6_5 | GL_UNSIGNED_SHORT_5_5_5_1 | GL_UNSIGNED_BYTE
     * @param srcPixelFormat GL_RGB | GL_RGBA
     */
    virtual void allocPixelMemory(const GLenum srcPixelType, const GLenum srcPixelFormat, const s32 miplevel);

    /**
     * テクスチャをindex番のユニットに関連付ける
     */
    virtual void bind(s32 index);

    /**
     * 空いているテクスチャユニットにバインドする。
     */
    virtual s32 bind();

    /**
     * バインドされたテクスチャユニットの番号を取得する。
     */
    virtual s32 getBindTextureUnitIndex() {
        return bindUnit;
    }

    /**
     * バインド済みかどうかを確認する。
     * バインドされている場合jctrueを返す。
     * resultIndexが設定されている場合、bindedならインデックスを返す。
     */
    virtual jcboolean isBinded(s32 *resultIndex);

    /**
     * VRAMの情報を取得する
     */
    virtual vram_id getVramId() const {
        return texture.getId();
    }

    /**
     * テクスチャ名を取得する
     */
    virtual u32 getName() const {
        return texture.get();
    }

    /**
     * テクスチャをユニットから切り離す
     */
    virtual void unbind();

    /**
     * 管理している資源を開放する
     */
    virtual void dispose();

    /**
     * テクスチャへのデコードを行う。
     */
    static jc_sp<TextureImage> decode( MDevice device, MImageDecoder decoder, const PixelFormat_e pixelFormat);

    /**
     * テクスチャへのデコードを行う。
     * uriにはJpegテクスチャへのURIを指定する。
     *
     * 拡張子jpg -> Jpeg形式として、内蔵デコーダで画像のデコードを行う
     * 拡張子pkm -> PKM形式として認識し、自動的にdecodePKMを呼び出す。
     * それ以外  -> decodeFromPlatformDecoderを呼び出して、デコードを行う
     */
    static jc_sp<TextureImage> decode( MDevice device, const Uri &uri, const PixelFormat_e pixelFormat = PixelFormat_RGBA8888);

    /**
     * PMKファイルのデコードを行う。
     */
    static jc_sp<TextureImage> decodePMK(MDevice device, const Uri &uri);

    /**
     * Platformが実装しているデコーダーで画像をデコードする。
     * iOS / AndroidであればJpeg / PNG / Bitmapが共通でデコードできる
     */
    static jc_sp<TextureImage> decodeFromPlatformDecoder( MDevice device, const Uri &uri, const PixelFormat_e pixelFormat);
};

/**
 * managed
 */
typedef jc_sp<TextureImage> MTextureImage;

}
}

#endif /* JCTEXTUREIMAGE_H_ */
