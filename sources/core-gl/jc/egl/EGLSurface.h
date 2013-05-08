/*
 * jcglEGLSurface.h
 *
 *  Created on: 2012/08/22
 */

#ifndef JCGLEGLSURFACE_H_
#define JCGLEGLSURFACE_H_

#include    "jc/egl/EGL.h"
#include    "jc/graphics/Pixel.h"


namespace jc {
namespace gl {

/**
 * レンダリング対象のサーフェイスを管理するためのプロトコル
 */
class EGLSurfaceProtocol {
public:
    virtual ~EGLSurfaceProtocol() {

    }

    /**
     * サーフェイスの幅を取得する。
     */
    virtual s32 getWidth() const = 0;

    /**
     * サーフェイスの高さを取得する。
     */
    virtual s32 getHeight() const = 0;

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
