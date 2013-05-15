/*
 * TextureImage_NDK.cpp
 *
 *  Created on: 2013/02/15
 */

#include    "jointcoding-ios.h"
#include    "jc/platform/Platform.h"
#include    "jc/platform/Timer.h"
#include    "jc/gl/texture/TextureImage.h"


namespace jc {
namespace gl {
/**
 * プラットフォームデコーダを利用してデコードを行う
 * iOS / AndroidであればJpeg / PNG / Bitmapが共通でデコードできる
 */
MPixelBuffer TextureImage::decodePixelsFromPlatformDecoder(MDevice device, const Uri &uri, TextureLoadOption *option) {
    // not impl
    assert(false);
    
    MPixelBuffer result;
    return result;
}

}
}

