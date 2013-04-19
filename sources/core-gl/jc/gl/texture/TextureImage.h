/*
 * jcTextureImage.h
 *
 *  Created on: 2012/07/22

 */

#ifndef JCTEXTUREIMAGE_H_
#define JCTEXTUREIMAGE_H_

#include    "jc/io/Uri.h"
#include    "jc/gl/GL.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/graphics/TextureTable.hpp"

namespace jc {
namespace gl {

/**
 * ETC1テクスチャの拡張子（PKM形式)
 */
#define JC_TEXTURE_EXT_PKM "pkm"

struct TextureLoadOption {
    /**
     * trueの場合強制的にPowerOfTwo変換して読み込む
     */
    jcboolean convert_pot;

    /**
     * trueの場合、mipmapを生成する
     */
    jcboolean gen_mipmap;

    /**
     * キャンセルチェックを行う。
     * 読込中にtrueを検出した場合、その時点で読込を停止する
     */
    jcboolean load_cancel;

    /**
     * 読込優先度を下げる（他のデバイスがロックされていないときに読み込み動作を行わせる
     */
    jcboolean load_priority_down;

    /**
     * ラインごとに分割してデコードする
     * デバイスの占有時間を細かくすることでローディングスレッドの占有時間を小さくする
     */
    s32 slice_loading;

    /**
     * スライス読込時に一度sleepする時間
     */
    s32 slice_sleep_time_ms;

    struct {
        /**
         * RAW画像読み込み時間
         */
        s32 raw_load_time_ms;

        s32 raw_pixelconvert_time_ms;

        /**
         * glTexImageの所要時間を返却する
         */
        s32 teximage_time_ms;

        /**
         * テクスチャ領域のalloc時間
         */
        s32 alloc_time_ms;

        /**
         * デバイス占有時間を返却する
         */
        s32 devicelocked_time_ms;
    } result;

    /**
     *
     */
    TextureLoadOption() {
        convert_pot = gen_mipmap = load_cancel = load_priority_down = jcfalse;
        result.alloc_time_ms = result.teximage_time_ms = result.devicelocked_time_ms = result.raw_load_time_ms = result.raw_pixelconvert_time_ms = 0;
        slice_loading = 1;
        slice_sleep_time_ms = 0;
    }
};

/**
 * OpenGLで扱うテクスチャを管理する。
 * GLES2.0を基本とするため、テクスチャサイズは2^n以外も可能だが、パフォーマンスには影響する
 */
class TextureImage: public Object {

    struct {
        /**
         * テクスチャサイズとしての幅
         */
        u32 tex_width;
        /**
         * テクスチャサイズとしての高さ
         */
        u32 tex_height;

        /**
         * 画像としての幅
         */
        u32 img_width;

        /**
         * 画像としての高さ
         */
        u32 img_height;
    } size;

    /**
     * テクスチャバインドターゲット
     */
    GLenum target;

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

protected:
    virtual s32 getFreeTextureUnitIndex();

    static u32 toTextureSize(const TextureLoadOption *option, const u32 size) {
        if (option && option->convert_pot) {
            return jc::toPowerOfTwo(size);
        } else {
            return size;
        }
    }

public:
    TextureImage(const s32 width, const s32 height, MDevice device);

    TextureImage(const GLenum target, const s32 width, const s32 height, MDevice device);

    virtual ~TextureImage();

    /**
     * 画像としての幅を取得する。
     */
    virtual u32 getWidth() const {
        return size.img_width;
    }

    /**
     * 画像としての高さを取得する
     */
    virtual u32 getHeight() const {
        return size.img_height;
    }

    /**
     * テクスチャとしての幅を取得する。
     * POT変換を行う場合がある。
     */
    virtual u32 getTextureWidth() const {
        return size.tex_width;
    }

    /**
     * テクスチャとしての高さを取得する。
     * POT変換を行う場合がある。
     */
    virtual u32 getTextureHeight() const {
        return size.tex_height;
    }

    /**
     * NPOTテクスチャの場合trueを返す。
     * GPUに渡すステータスをチェックするため、getTextureWidth()とgetTextureHeight()をチェックする
     */
    virtual jcboolean isPowerOfTwoTexture();

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
     * mipmapを自動生成する
     */
    virtual void genMipmaps();

    /**
     * glBindTexgture(target, xxx)の内容を変更する
     */
    virtual void setBindTarget(GLenum target) {
        this->target = target;
    }

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
     * 名称は一意の32bit整数になる。
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
     * uriにはJpegテクスチャへのURIを指定する。
     *
     * 拡張子jpg -> Jpeg形式として、内蔵デコーダで画像のデコードを行う
     * 拡張子pkm -> PKM形式として認識し、自動的にdecodePKMを呼び出す。
     * それ以外  -> decodeFromPlatformDecoderを呼び出して、デコードを行う
     */
    static jc_sp<TextureImage> decode( MDevice device, const Uri &uri, const PixelFormat_e pixelFormat = PixelFormat_RGBA8888, TextureLoadOption *option = NULL);

    /**
     * PMKファイルのデコードを行う。
     */
    static jc_sp<TextureImage> decodePMK(MDevice device, const Uri &uri, TextureLoadOption *option = NULL);

    /**
     * Platformが実装しているデコーダーで画像をデコードする。
     * iOS / AndroidであればJpeg / PNG / Bitmapが共通でデコードできる
     */
    static jc_sp<TextureImage> decodeFromPlatformDecoder( MDevice device, const Uri &uri, const PixelFormat_e pixelFormat, TextureLoadOption *option = NULL);
};

/**
 * managed
 */
typedef jc_sp<TextureImage> MTextureImage;


/**
 * GLで利用するテクスチャテーブル
 */
typedef TextureTable<TextureImage> GLTextureTable;

/**
 * managed
 */
typedef jc_sp<GLTextureTable> MGLTextureTable;


}
}

#endif /* JCTEXTUREIMAGE_H_ */
