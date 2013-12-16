/*
 * RenderingContext.cpp
 *
 *  Created on: 2013/12/16
 */

#include    "jc/gl/context/RenderingContext.hpp"

namespace jc {
namespace gl {

/**
 * ピクセルバッファのキャプチャを行う
 */
void RenderingContext::capture(MGLState state, const PixelFormat_e format, MRawImage image) {
    switch (format) {
        case PixelFormat_A8:
        case PixelFormat_RGB888:
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            break;
        case PixelFormat_RGBA8888:
            glPixelStorei(GL_PACK_ALIGNMENT, 4);
            break;
        default:
            jclogf("unknown capture format(%d)", format)
            ;
            assert(false);
            break;
    }

    const s32 frameWidth = this->getRenderingTarget()->getWidth();
    const s32 frameHeight = this->getRenderingTarget()->getHeight();

    image->alloc(format, frameWidth, frameHeight);
    glReadPixels(0, 0, frameWidth, frameHeight, TextureImage::toGLPixelFormat(format), TextureImage::toGLPixelDataType(format), image->getPixels());

    // 取得待ちを行う
    glFinish();
}

}
}
