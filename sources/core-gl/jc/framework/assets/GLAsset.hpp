/*
 * GLAsset.hpp
 *
 *  Created on: 2013/06/01
 */

#ifndef GLASSET_HPP_
#define GLASSET_HPP_

#include    "jointcoding.h"
#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Vec4.h"
#include    "jc/math/Matrix.h"
#include    "jc/graphics/Color.h"
#include    "jc/collection/ArrayHandle.hpp"
#include    "jc/collection/SafeArray.hpp"

// GL系
#include    "jc/gl/GL.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/gl/gpu/DeviceLock.hpp"
#include    "jc/gl/texture/TextureImage.h"
#include    "jc/gl/vbo/IndexBufferObject.hpp"
#include    "jc/gl/vbo/VertexBufferObject.hpp"
#include    "jc/gl/shader/VertexAttribute.hpp"
#include    "jc/gl/shader/VectorUniform.hpp"
#include    "jc/gl/shader/TextureUniform.hpp"

namespace jc {
namespace fw {
using namespace jc;
using namespace jc::gl;
}
}


#endif /* GLASSET_HPP_ */
