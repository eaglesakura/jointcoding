/*
 * BoneWeight.h
 *
 *  Created on: 2012/12/17
 */

#ifndef FBXBONEWEIGHT_H_
#define FBXBONEWEIGHT_H_

#include "jointcoding.h"

namespace jc {

enum {
    /**
     * 未使用のボーン
     */
    UNUSED_BONE = 0xFF,

    /**
     * シンプルに利用する場合のボーン数
     */
    SIMPLE_BONE_NUM = 4,
};

/**
 * ボーンの重み情報を管理する。
 * １つのメッシュグループでボーンは256本(u8)に制限する
 */
template<int WEIGHT_NUM>
class _BoneWeight {
public:
    /**
     * 参照するボーンのインデックス.
     */
    u8 indices[WEIGHT_NUM];

    /**
     * ボーンの重み係数.
     * 合計値は必ず1.0fになる。
     */
    float weights[WEIGHT_NUM];

    _BoneWeight() {
        for (int i = 0; i < WEIGHT_NUM; ++i) {
            indices[i] = (u8) UNUSED_BONE;
            weights[i] = 0;
        }
    }

    /**
     * ウェイト値を書き込む
     */
    inline void registerWegight(s32 boneIndex, float weight) {
        float lowerWeight = 99999.0f; // 最も小さいウェイト値
        s32 lowerWeightIndex = 0; // 最も小さいウェイト値のインデックス
        for (int i = 0; i < WEIGHT_NUM; ++i) {
            // 登録されていないインデックスがあればそこへ書き込む
            if (indices[i] == (u8) UNUSED_BONE) {
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
        for (int i = 0; i < WEIGHT_NUM; ++i) {
            sum += weights[i];
        }

        // 合計値が0の場合は何もしない
        if (sum == 0) {
            return;
        }

        for (int i = 0; i < WEIGHT_NUM; ++i) {
            weights[i] /= sum;
            if (indices[i] == (u8) UNUSED_BONE) {
                indices[i] = 0;
            }
        }

        for (int i = 0; i < (WEIGHT_NUM - 1); ++i) {
            for (int k = (i + 1); k < WEIGHT_NUM; ++k) {
                if (weights[i] < weights[k]) {
                    // 値が大きい方が若いインデックスに来て欲しい
                    jc::swap(&weights[i], &weights[k]);
                    jc::swap(&indices[i], &indices[k]);
                }
            }
        }
    }
};

/**
 * シンプルなボーンウェイト情報
 */
typedef _BoneWeight<SIMPLE_BONE_NUM> SimpleBoneWeight;

}

#endif /* BONEWEIGHT_H_ */
