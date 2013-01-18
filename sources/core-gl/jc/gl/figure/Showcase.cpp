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
}

}
}

