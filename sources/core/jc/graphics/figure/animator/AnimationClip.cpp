/*
 * AnimationClip.cpp
 *
 *  Created on: 2013/01/08
 */

#include    "jc/graphics/figure/animator/AnimationClip.h"

namespace jc {

/**
 *
 */
AnimationClip::AnimationClip(MAnimationGroup animation) {
    this->animation = animation;
    this->wrap = Animator::Wrap_Loop;
    current = 0;
    range.start = 0;
    range.end = 0;

    for (u32 i = 0; i < animation->animations_length; ++i) {
        u32 end = 0;
        animation->animations[i].getAnimationRange(NULL, NULL, NULL, &end);
//        jclogf("range-end(%d)", range.end);
        range.end = jc::max(range.end, end);
    }
}

/**
 * アニメーションレンジを指定してクリップを作成する
 */
AnimationClip::AnimationClip(MAnimationGroup animation, const u32 start, const u32 end) {
    this->animation = animation;
    this->wrap = Animator::Wrap_Loop;
    range.start = start;
    current = 0;
    range.end = end;
}

/**
 * 行列を取得する。
 */
void AnimationClip::getMatrix(const u32 nodeNumber, Matrix4x4 *result) const {

    const float range_length = (float) (range.end - range.start);
    float normalized_frame = 0;
    if (wrap == Animator::Wrap_Stop) {
        normalized_frame = jc::minmax<float>((float) range.start, (float) range.end - 1, current + range.start);
    } else /* if (wrap == Wrap_Loop) */{
        normalized_frame = current + range.start;

        while (normalized_frame < range.start) {
            normalized_frame += range_length;
        }
        while (normalized_frame >= range.end) {
            normalized_frame -= range_length;
        }
    }

//    jclogf("normalized_frame = %f (%d -> %d)", normalized_frame, range.start, range.end);
    Transform temp;
    animation->animations[nodeNumber].getTransform(normalized_frame, &temp);
    temp.getMatrix(result);
}

}
