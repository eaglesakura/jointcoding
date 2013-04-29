/*
 * JCGPU.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCGPU_H_
#define JCGPU_H_

#include    "jointcoding.h"
#include    "jc/model/Object.h"

namespace jc {
namespace gl {

/**
 * 対応している拡張機能
 */
enum GPUExtension_e {
    /**
     * ETC1に対応している
     * GL_OES_compressed_ETC1_RGB8_texture
     */
    GPUExtension_Texture_ETC1,

    /**
     * PVRTC対応
     * GL_IMG_texture_compression_pvrtc
     */
    GPUExtension_Texture_PVRTC,

    /**
     * S3TC対応
     * GL_EXT_texture_compression_s3tc
     */
    GPUExtension_Texture_S3TC,

    /**
     * BGRAテクスチャ対応
     * GL_EXT_texture_format_BGRA8888
     */
    GPUExtension_Texture_BGRA8888,

    /**
     * external画像
     * GL_OES_EGL_image_external
     * for Android（support SurfaceTexture）
     */
    GPUExtension_OES_EGL_image_external,

    /**
     * 深度テクスチャが利用できる
     * GL_OES_depth_texture
     */
    GPUExtension_Texture_Depth,

    /**
     * タイルレンダリング対応
     * GL_EXT_discard_framebuffer
     * PowerVR(iOS, GalaxyNexus, etc...)
     */
    GPUExtension_TileBasedDeferredRendering,

    /**
     * half floatテクスチャが利用できる
     */
    GPUExtension_Texture_HalfFloat,

    /**
     * num flags
     */
    GPUExtension_Num,
};

/**
 * サポートする代表的な系列機を列挙する
 */
enum GPUFamily_e {
    /**
     * Mali GPU
     * GalaxyS2, GalaxyS3, Nexus10
     */
    GPUFamily_Mali,

    /**
     * PowerVR GPU
     * iOS, GalaxyNexus
     */
    GPUFamily_PowerVR,

    /**
     * Adreno GPU
     * XperiaGX
     */
    GPUFamily_Adreno,

    /**
     * Tegra GPU
     * OptimusPad, GalaxyTab10.1, Nexus7
     */
    GPUFamily_Tegra,

    /**
     * not found...
     */
    GPUFamily_Unknown,
};

/**
 * GPUのステータスを格納する。
 * 実行環境で変動するステータスを格納する。
 */
class GPUCapacity {
    friend class GLState;

    /**
     * 通常、newはできない。
     */
    GPUCapacity();
    /**
     * 初期化を行う
     */
    static void initialize();
public:

    /**
     * レンダラー名を取得する
     */
    static const String& getRenderer();
    /**
     * ベンダー名を取得する
     */
    static const String& getVendor();

    /**
     * 頂点属性の最大数を取得する。
     */
    static u32 getMaxVertexAttributes();

    /**
     * テクスチャユニットの最大数を取得する
     * この枚数を超えるテクスチャ処理は行えない。
     */
    static u32 getMaxTextureUnits();

    /**
     * テクスチャの一辺のサイズの最大値
     */
    static u32 getMaxTextureSize();

    /**
     * 頂点シェーダのUniformベクトル最大数を取得する
     */
    static u32 getMaxUniformVectorsVs();

    /**
     * フラグメントシェーダでのUniformベクトル最大数を取得する
     */
    static u32 getMaxUniformVectorsFs();

    /**
     * GPU拡張機能をサポートするかを調べる
     */
    static jcboolean isSupport(const GPUExtension_e extension);

    /**
     * GPU系列のチェックを行う
     * 代表的な系列機以外はUnknownになる（エミュレーター等）
     */
    static GPUFamily_e getGPUFamily();

    /**
     * 実行されているGPUが同一系統のGPUである場合、trueを返す
     */
    static jboolean isFamily(const GPUFamily_e family) {
        return getGPUFamily() == family;
    }
};

}
}

#endif /* JCGPU_H_ */
