/*
 * EGLSurfaceWrapper.h
 *
 *  Created on: 2013/05/08
 */

#ifndef EGLSURFACEWRAPPER_H_
#define EGLSURFACEWRAPPER_H_

#include    "jointcoding-android.h"
#include    "jc/egl/EGL.h"
#include    "jc/egl/EGLContext.h"
#include    "jc/egl/EGLSurface.h"

// SDK impl
#include    "android-classes/ndkEGLSurfaceWrapper.h"

namespace ndk {
using namespace jc;
using namespace jc::gl;

class SdkEGLSurfaceWrapper: public Object, public EGLSurfaceProtocol {
public:
    jc_sp<EGLSurfaceWrapper> eglSurface;
public:
    SdkEGLSurfaceWrapper(const jc_sp<EGLSurfaceWrapper> eglSurface) {
        assert(eglSurface);
        this->eglSurface = eglSurface;
    }
    virtual ~SdkEGLSurfaceWrapper() {
    }

    /**
     * サーフェイスの幅を取得する。
     */
    virtual s32 getWidth() const {
        assert(eglSurface);
        return eglSurface->getWidth();
    }

    /**
     * サーフェイスの高さを取得する。
     */
    virtual s32 getHeight() const {
        assert(eglSurface);
        return eglSurface->getHeight();
    }

    /**
     * 握っている資源を明示的に開放する
     */
    virtual void dispose() {
        if(eglSurface) {
            eglSurface->dispose();
            eglSurface.reset();
        }
    }
};

}
#endif /* EGLSURFACEWRAPPER_H_ */
