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
     */
    GPUExtension_Texture_ETC1,

    /**
     * PVRTC対応
     */
    GPUExtension_Texture_PVRTC,

    /**
     * BGRAテクスチャ対応
     */
    GPUExtension_Texture_BGRA8888,

    /**
     * external画像
     * for Android（support SurfaceTexture）
     */
    GPUExtension_OES_EGL_image_external,



    /**
     * num flags
     */
    GPUExtension_Num,
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
};

}
}

#endif /* JCGPU_H_ */
