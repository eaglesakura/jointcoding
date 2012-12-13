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
 * GPUのステータスを格納する。
 * 実行環境で変動するステータスを格納する。
 */
class GPUCapacity {
    /**
     * 通常、newはできない。
     */
    GPUCapacity();
public:
    /**
     * レンダラー名を取得する
     */
    static String getRenderer();
    /**
     * ベンダー名を取得する
     */
    static String getVendor();

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
};

}
}

#endif /* JCGPU_H_ */
