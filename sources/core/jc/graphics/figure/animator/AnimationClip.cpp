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

    {
//        jclogf(" node[%d] T(%f, %f, %f) R(%f, %f, %f) S(%f, %f, %f)", nodeNumber, temp.translate.x, temp.translate.y, temp.translate.z, temp.rotate.x, temp.rotate.y, temp.rotate.z, temp.scale.x, temp.scale.y, temp.scale.z);
    }

    temp.getMatrix(result);
}

}
