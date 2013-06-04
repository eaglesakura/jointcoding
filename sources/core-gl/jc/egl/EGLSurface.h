/*
 * jcglEGLSurface.h
 *
 *  Created on: 2012/08/22
 */

#ifndef JCGLEGLSURFACE_H_
#define JCGLEGLSURFACE_H_

#include    "jc/egl/EGL.h"
#include    "jc/graphics/Pixel.h"
#include    "jc/gl/surface/IRenderingSurface.hpp"

namespace jc {
namespace gl {

/**
 * レンダリング対象のサーフェイスを管理するためのプロトコル
 *
 * IRenderingSurfaceを継承しているため、レンダリングサーフェイスの基底として動作する。
 */
class EGLSurfaceProtocol: public IRenderingSurface {
public:
    virtual ~EGLSurfaceProtocol() {

    }

    /**
     * 握っている資源を明示的に開放する
     */
    virtual void dispose() = 0;
};

typedef jc_sp<EGLSurfaceProtocol> MEGLSurfaceProtocol;

#define EGL_NULL_SURFACE    MEGLSurfaceProtocol()

}
}

#endif /* JCGLEGLSURFACE_H_ */
