/*
 * ActionTable.h
 *
 *  Created on: 2013/01/08
 */

#ifndef ANIMATION_CLIP_H_
#define ANIMATION_CLIP_H_

#include    "jointcoding.h"
#include    "jc/graphics/figure/animator/Animator.h"

namespace jc {

struct AnimationGroup {
    /**
     * アニメーション
     */
    jc_sa<Animator> animations;

    /**
     * アニメーション数
     */
    u32 animations_length;

    AnimationGroup(const u32 length) {
        animations_length = length;
        animations.reset(new Animator[length]);
    }
};

typedef jc_sp<AnimationGroup> MAnimationGroup;

/**
 * アニメーションデータを定義する
 * アニメーションを適当なフレームで区切って、別なアニメーションクリップを生成できるようにする。
 */
class AnimationClip: public Object {
    MAnimationGroup animation;

    struct {
        /**
         * 開始フレーム
         */
        u32 start;
        /**
         * 終了フレーム
         */
        u32 end;
    } range;

    /**
     * 現在のフレーム数。
     * 設定は0〜(end - start)の範囲で行う
     */
    float current;

    /**
     * アニメーションのラップタイプ
     */
    Animator::Wrap_e wrap;
public:
    /**
     *
     */
    AnimationClip(MAnimationGroup animation);

    /**
     * アニメーションレンジを指定してクリップを作成する
     */
    AnimationClip(MAnimationGroup animation, const u32 start, const u32 end);

    virtual ~AnimationClip() {
    }

    /**
     * 全体アニメーションを取得する。
     */
    virtual MAnimationGroup getAnimationGroup() const {
        return animation;
    }

    /**
     * 現在のアニメーションを更に分割して、新しいアニメーションにする。
     */
    virtual jc_sp<AnimationClip> slice(const u32 start, const u32 end) const {
        return jc_sp<AnimationClip>(new AnimationClip(animation, start, end));
    }

    /**
     * 現在のフレームを設定する
     */
    virtual void setCurrentFrame(const float set) {
        current = set;
    }

    /**
     * 現在のフレームを取得する
     */
    virtual float getCurrentFrame() const {
        return current;
    }

    /**
     * アニメーションのフレーム数を取得する
     */
    virtual float getAnimationLength() const {
        return range.end - range.start;
    }

    /**
     * 行列を取得する。
     */
    virtual void getMatrix(const u32 nodeNumber, Matrix4x4 *result) const;
};

/**
 * managed
 */
typedef jc_sp<AnimationClip> MAnimationClip;

}

#endif /* ACTIONTABLE_H_ */
