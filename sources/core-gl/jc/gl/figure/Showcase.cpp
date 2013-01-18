/*
 * Showcase.cpp
 *
 *  Created on: 2013/01/18
 */

#include    "jc/gl/figure/Showcase.h"

namespace jc {
namespace gl {

Showcase::Showcase() {
    animation.offset = 1.0f;
}

Showcase::~Showcase() {

}

/**
 * 描画を行う
 */
void Showcase::rendering(FigureRenderer *render) const {
    render->getShader()->bind();

    if (animation.clip) {
        // ポーズの設定を行う
        figure->posing(animation.clip.get());
    }

    // update world matrix
    {
        Matrix4x4 world;
        transform.getMatrix(&world);

        glUniformMatrix4fv(render->getWorldUniformLocation(), 1, GL_FALSE, (float*)&world);
    }

    figure->rendering(render->getShaderParams().get());
}

}
}

