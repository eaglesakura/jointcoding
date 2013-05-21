/*
 * FigureAsset.hpp
 *
 *  Created on: 2013/05/20
 */

#ifndef FIGUREASSET_HPP_
#define FIGUREASSET_HPP_

#include    "jointcoding.h"
#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Vec4.h"
#include    "jc/collection/ArrayHandle.hpp"
#include    "jc/collection/SafeArray.hpp"

// GL系
#include    "jc/gl/GL.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/gl/gpu/DeviceLock.hpp"
#include    "jc/gl/texture/TextureImage.h"
#include    "jc/gl/vbo/IndexBufferObject.hpp"
#include    "jc/gl/vbo/VertexBufferObject.hpp"

namespace jc {
namespace fw {
using namespace jc;
using namespace jc::gl;
}
}

// フィギュアを簡単に扱うためのフレームワーク
// カスタマイズした頂点やシェーダーを扱いたい場合は適宜拡張を行う
#include    "jc/framework/assets/figure/FigureVertex.hpp"
#include    "jc/framework/assets/figure/FigureBuffer.hpp"
#include    "jc/framework/assets/figure/FigureLoader.h"

#endif /* FIGUREASSET_HPP_ */
