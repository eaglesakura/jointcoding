/*
 * Animator.h
 *
 *  Created on: 2012/12/27
 */

#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include "jc/math/Math.h"
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

    KeyFrame() {
        frame = 0;
    }

    KeyFrame(const u32 frame, const Type &value) {
        this->frame = frame;
        this->value = value;
    }
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
     * アニメーションが一切設定されていない場合はtrue
     */
    virtual jcboolean empty() {
        return translates.empty() && rotates.empty() && scales.empty();
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
     * 位置アニメーション一覧を取得する
     */
    const std::vector<TranslateKey>& getTranslates() const {
        return translates;
    }

    /**
     * 回転アニメーション一覧を取得する
     */
    const std::vector<RotateKey>& getRotates() const {
        return rotates;
    }

    /**
     * スケーリングキー一覧を取得する
     */
    const std::vector<ScaleKey>& getScales() const {
        return scales;
    }

    /**
     * アニメーションの現在の状態を取得する
     */
    inline void getTransform(const Wrap_e wrap, Transform *result) const {
        result->translate = translates[0].value;
        result->rotate = rotates[0].value;
        result->scale = scales[0].value;
    }

    /**
     * 移動キーを挿入する
     */
    virtual jcboolean addTranslateAnimation(const TranslateKey &key) {
        // ラストキーと同じ？
        if (translates.size() > 1) {
            if (translates[translates.size() - 1].value == key.value) {
                translates[translates.size() - 1] = key;
//                jclogf("    some translate key(%d)", key.frame);
                return jcfalse;
            }
        }

//        jclogf("    new translate key(%d)(%f, %f, %f)", key.frame, key.value.x, key.value.y, key.value.z);
        translates.push_back(key);
        return jctrue;
    }

    /**
     * 回転キーを挿入する
     */
    virtual jcboolean addRotateAnimation(const RotateKey &key) {
        // ラストキーと同じ？
        if (rotates.size() > 1) {
            if (rotates[rotates.size() - 1].value == key.value) {
                rotates[rotates.size() - 1] = key;
//                jclogf("    some rotate key(%d)", key.frame);
                return jcfalse;
            }
        }

//        jclogf("    new rotate key(%d)(%f, %f, %f)", key.frame, key.value.x, key.value.y, key.value.z);
        rotates.push_back(key);
        return jctrue;
    }

    /**
     * スケーリングキーを挿入する
     */
    virtual jcboolean addScaleAnimation(const ScaleKey &key) {
        // ラストキーと同じ？
        if (scales.size() > 1) {
            if (scales[scales.size() - 1].value == key.value) {
                scales[scales.size() - 1] = key;
//                jclogf("    some scale key(%d)", key.frame);
                return jcfalse;
            }
        }
//        jclogf("    new scale key(%d)(%f, %f, %f)", key.frame, key.value.x, key.value.y, key.value.z);
        scales.push_back(key);
        return jctrue;
    }
};

/**
 * managed
 */
typedef jc_sp<Animator> MAnimator;

}

#endif /* ANIMATOR_H_ */
