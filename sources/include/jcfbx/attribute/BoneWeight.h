/*
 * BoneWeight.h
 *
 *  Created on: 2012/12/17
 */

#ifndef FBXBONEWEIGHT_H_
#define FBXBONEWEIGHT_H_

#include "jointcoding.h"

namespace jc {
namespace fbx {

/**
 * ボーンの重み情報を管理する。
 */
template<int WEIGHT_NUM>
class BoneWeight {
public:
    /**
     * 参照するボーンのインデックス.
     */
    s8 indices[WEIGHT_NUM];

    /**
     * ボーンの重み係数.
     * 合計値は必ず1.0fになる。
     */
    float weights[WEIGHT_NUM];

    BoneWeight() {
        for (int i = 0; i < WEIGHT_NUM; ++i) {
            indices[i] = -1;
            weights[i] = 0;
        }
    }

    /**
     * ウェイト値を書き込む
     */
    inline void registerWegight(s32 boneIndex, float weight) {
        float lowerWeight = 99999.0f; // 最も小さいウェイト値
        s32 lowerWeightIndex = 0; // 最も小さいウェイト値のインデックス
        for (s32 i = 0; i < 3; ++i) {
            // 登録されていないインデックスがあればそこへ書き込む
            if (indices[i] < 0) {
                indices[i] = boneIndex;
                weights[i] = weight;
                return;
            }

            if (weights[i] < lowerWeight) {
                lowerWeight = weights[i];
                lowerWeightIndex = i;
            }
        }

        //! もっとも低いウェイトを切り捨てる。
        if (weight > lowerWeight) {
            indices[lowerWeightIndex] = boneIndex;
            weights[lowerWeightIndex] = weight;
        }
    }

    /**
     * 頂点ウェイト値を最適化する
     */
    inline void normalize() {
        float sum = 0.0f;
        for (s32 i = 0; i < 3; ++i) {
            sum += weights[i];
        }

        // 合計値が0の場合は何もしない
        if (sum == 0) {
            return;
        }

        for (s32 i = 0; i < 3; ++i) {
            weights[i] /= sum;
            if (indices[i] < 0) {
                indices[i] = 0;
            }
        }
    }
};

/**
 * シンプルなボーンウェイト情報
 */
typedef BoneWeight<4> SimpleBoneWeight;

}
}

#endif /* BONEWEIGHT_H_ */
