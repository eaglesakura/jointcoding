/*
 * AnimationClip.cpp
 *
 *  Created on: 2013/01/08
 */

#include    "jc/graphics/figure/animator/AnimationClip.h"

namespace jc {

/**
 * 行列を取得する。
 */
void AnimationClip::getMatrix(const u32 nodeNumber, Matrix4x4 *result) const {
    Transform temp;
    animation->animations[nodeNumber].getTransform(Animator::Wrap_Loop, current, &temp);

//    temp.translate.set(0, 0, 0);
//    temp.rotate.set(0, 0, 0, 0);
//    temp.scale.set(1,1, 1);
    temp.getMatrix(result);
}

}
