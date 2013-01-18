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

    KeyFrame(const KeyFrame &origin) {
        frame = origin.frame;
        value = origin.value;
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

private:

    template<typename T>
    inline jcboolean add_key(std::vector<T> *key_list, const T &key) {
        // ラストキーと同じ？
        if (key_list->size() > 1) {
            if ((*key_list)[key_list->size() - 1].value == key.value) {
                (*key_list)[key_list->size() - 1] = key;
                return jcfalse;
            }
        }
        key_list->push_back(key);
        return jctrue;
    }

    /**
     * フレーム数が low_ptr <= frame <= high_ptr となるフレームを探し出す。
     */
    template<typename T, typename VectorType>
    static inline void findFrames(const std::vector<T> &key_list, const u32 frame, KeyFrame<VectorType> **low_ptr, KeyFrame<VectorType> **high_ptr) {
        (*low_ptr) = (KeyFrame<VectorType>*)&(key_list[0]);
        for (u32 i = 0; i < key_list.size(); ++i) {
            if (key_list[i].frame <= frame) {
                // フレームが欲しいフレームを超えてないなら、次をチェックする
                (*low_ptr) = (KeyFrame<VectorType>*)&(key_list[i]);
            } else {
                // フレームが欲しいフレームを超えたら、最後にチェックしたフレームを返す
                (*high_ptr) = (KeyFrame<VectorType>*)&(key_list[i]);
                return;
            }
        }

        (*high_ptr) = (KeyFrame<VectorType>*)&(key_list[key_list.size() - 1]);
    }

    template<typename T, typename VectorType>
    static inline void calc_frame(const std::vector<T> &key_list, const float frame, VectorType* result) {
        KeyFrame<VectorType> *low_frame = NULL;
        KeyFrame<VectorType> *high_frame = NULL;

        findFrames(key_list, (u32) frame, &low_frame, &high_frame);

        // ２つのフレームの差分
        const float frame_length = high_frame->frame - low_frame->frame;
        // 現在のフレームの差
        const float current_frame_position = frame - low_frame->frame;

        // フレームの長さと、経過時間からウェイトを取得する
        const float frame_weight = frame_length > 0.0f ? current_frame_position / frame_length : 0;

        // ２つのフレームを補間する
        lerp(low_frame->value, high_frame->value, frame_weight, result);
    }

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
    inline void getAnimationRange(u32 *result_translate, u32 *result_rotate, u32 *result_scale, u32 *result_all) const {
        const u32 max_translate = translates.empty() ? 0 : (translates[translates.size() - 1].frame);
        const u32 max_rotate = rotates.empty() ? 0 : (rotates[rotates.size() - 1].frame);
        const u32 max_scale = scales.empty() ? 0 : (scales[scales.size() - 1].frame);

//        jclogf("max(%d, %d, %d)", max_translate, max_rotate, max_scale);
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
    inline void getTransform(const float frame, Transform *result) const {
        calc_frame<TranslateKey, Vector3f>(translates, frame, &result->translate);
        calc_frame<RotateKey, Vector4f>(rotates, frame, &result->rotate);
        calc_frame<ScaleKey, Vector3f>(scales, frame, &result->scale);
    }

    /**
     * 移動キーを挿入する
     */
    virtual jcboolean addTranslateAnimation(const TranslateKey &key) {
        return add_key(&translates, key);
    }

    /**
     * 回転キーを挿入する
     */
    virtual jcboolean addRotateAnimation(const RotateKey &key) {
        return add_key(&rotates, key);
    }

    /**
     * スケーリングキーを挿入する
     */
    virtual jcboolean addScaleAnimation(const ScaleKey &key) {
        return add_key(&scales, key);
    }
};

/**
 * managed
 */
typedef jc_sp<Animator> MAnimator;

}

#endif /* ANIMATOR_H_ */
