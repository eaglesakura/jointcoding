/*
 * Animator.h
 *
 *  Created on: 2012/12/27
 */

#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include "jc/math/Transform.h"
#include <vector>

namespace jc {

template<typename Type>
struct KeyFrame {

    /**
     * nフレーム目。
     * 必ず整数になる
     */
    u32 frame;

    /**
     * キーフレームに打ち込むパラメータ
     */
    Type value;
};

/**
 * 位置情報のキー
 */
typedef KeyFrame<Vector3f> TranslateKey;

/**
 * 開店情報のキー
 */
typedef KeyFrame<Vector4f> RotateKey;

/**
 * スケーリング情報のキー
 */
typedef KeyFrame<Vector3f> ScaleKey;

/**
 * 各キーフレーム毎にアニメーションを管理する。
 * アニメーションが必要な各オブジェクトに対して1つを管理する。
 */
class Animator: public Object {
    /**
     * 位置情報
     */
    std::vector<TranslateKey> translates;

    /**
     * 回転情報
     */
    std::vector<RotateKey> rotates;

    /**
     * スケーリング情報
     */
    std::vector<ScaleKey> scales;
public:
    enum Wrap_e {
        /**
         * アニメーションはループさせる
         */
        Wrap_Loop,

        /**
         * アニメーション末尾は停止させる
         */
        Wrap_Stop,
    };

    Animator() {
    }

    virtual ~Animator() {
    }

    /**
     * アニメーションの最大フレーム数を取得する
     * 各パラメータはNULLチェックして返す。
     */
    inline void getAnimationRange(u32 *result_translate, u32 *result_rotate, u32 *result_scale, u32 *result_all) {
        const u32 max_translate = translates.empty() ? 0 : (translates[translates.size() - 1].frame);
        const u32 max_rotate = rotates.empty() ? 0 : (rotates[rotates.size() - 1].frame);
        const u32 max_scale = scales.empty() ? 0 : (scales[scales.size() - 1].frame);

        // 最大フレーム数
        const u32 max_frame = jc::max(jc::max(max_translate, max_rotate), max_scale);

        if (result_translate) {
            (*result_translate) = max_translate;
        }

        if (result_rotate) {
            (*result_rotate) = max_rotate;
        }

        if (result_scale) {
            (*result_scale) = max_scale;
        }

        if (result_all) {
            (*result_all) = max_frame;
        }
    }

    /**
     * アニメーションの現在の状態を取得する
     */
    inline void getTransform(const Wrap_e wrap, Transform *result) const {
        result->translate = translates[0];
        result->rotate = rotates[0];
        result->scale = scales[0];
    }

    /**
     * 移動キーを挿入する
     */
    virtual void addTranslateAnimation(const TranslateKey &key) {
        translates.push_back(key);
    }

    /**
     * 回転キーを挿入する
     */
    virtual void addRotateAnimation(const RotateKey &key) {
        rotates.push_back(key);
    }


    /**
     * スケーリングキーを挿入する
     */
    virtual void addScaleAnimation(const ScaleKey &key) {
        scales.push_back(key);
    }
};

}

#endif /* ANIMATOR_H_ */
