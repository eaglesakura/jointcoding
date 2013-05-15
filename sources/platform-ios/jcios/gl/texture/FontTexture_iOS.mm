/*
 * FontTexture_NDK.cpp
 *
 *  Created on: 2013/02/19
 */

#include    "jointcoding-ios.h"
#include    "jc/platform/Platform.h"
#include    "jc/gl/texture/FontTexture.h"

namespace jc {
namespace gl {

/**
 * フォントテクスチャを作成する
 */
//
jc_sp<FontTexture> FontTexture::createInstance(const String &text, const u32 height, MDevice device) {
    // not impl
    assert(false);
    return jc_sp<FontTexture>();
}

}
}

