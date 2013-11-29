/*
 * Showcase.cpp
 *
 *  Created on: 2013/01/18
 */

#include    "jc/gl/figure/Showcase.h"

namespace jc {
namespace gl {

Showcase::Showcase() {
    animation.speed = 1.0f;
    animation.originRate = 30;
}

Showcase::~Showcase() {

}

/**
 * アニメーションを進める。
 */
jcboolean Showcase::nextAnimation(const MLoopController controller) {
    if (animation.clip) {
        // １秒間に進めるフレーム数
        const float anim_offet_sec = (float) animation.originRate * animation.speed;

        // このフレームｄねおオフセットに変更
        const float frame_offset = controller->sec2frame(anim_offet_sec);

        const float next_frame = animation.clip->getCurrentFrame() + frame_offset;
        animation.clip->setCurrentFrame(next_frame);

        // anim finish?
        return next_frame >= animation.clip->getAnimationLength();
    }

    return jcfalse;
}

/**
 * 描画を行う
 */
void Showcase::rendering(MGLState state, FigureRenderer *render) const {
    render->getShader()->bind(state);

    if (animation.clip) {
        // ポーズの設定を行う
        figure->posing(animation.clip.get());
    }

    // update world matrix
    {
        Matrix4x4 world;
        transform.getMatrix(&world);

        glUniformMatrix4fv(render->getWorldUniformLocation(), 1, GL_FALSE, (float*) &world);
    }

    figure->rendering(render->getShaderParams().get());
}

}
}

