/*
 * TextureUtil.h
 *
 *  Created on: 2013/12/13
 */
#ifndef TEXTUREUTIL_H_
#define TEXTUREUTIL_H_

#include    "jointcoding.h"
#include    "jc/gl/texture/TextureImage.h"
#include    "jc/gl/texture/PixelBuffer.hpp"

namespace ndk {

/**
 * ピクセルバッファをNativeから生成する
 */
jc::gl::MPixelBuffer createPixelBufferFromImageDecoder(jobject jImageDecoder, Vector2i *pImageSize);

}

#endif /* TEXTUREUTIL_H_ */
