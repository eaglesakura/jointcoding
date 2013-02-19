/*
 * FontTexture.cpp
 *
 *  Created on: 2013/02/19
 */

#ifndef FONTTEXTURE_CPP_
#define FONTTEXTURE_CPP_

#include    "jointcoding.h"
#include    "jc/gl/FontTexture.h"

namespace jc {
namespace gl {

FontTexture::FontTexture(const u32 width, const u32 height, MDevice device, const String &text) :
        TextureImage(width, height, device) {
    this->text = text;
}

FontTexture::~FontTexture() {

}

}
}
#endif /* FONTTEXTURE_CPP_ */
