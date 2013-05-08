/*
 * EGLContextWrapper.h
 *
 *  Created on: 2013/05/08
 */

#ifndef EGLCONTEXTWRAPPER_H_
#define EGLCONTEXTWRAPPER_H_

#include    "jointcoding-android.h"
#include    "jc/egl/EGL.h"
#include    "jc/egl/EGLContext.h"
#include    "jc/egl/EGLSurface.h"

// SDK impl
#include    "android-classes/ndkEGLContextWrapper.h"

namespace ndk {
using namespace jc;
using namespace jc::gl;

class SdkEGLContextWrapper: public Object, public EGLContextProtocol {
    MGLState state;
    VRAM vram;
public:
    jc_sp<EGLContextWrapper> eglContext;
public:
    SdkEGLContextWrapper(const jc_sp<EGLContextWrapper> eglContext) {
        this->eglContext = eglContext;

        state.reset(new GLState());
        vram.reset(new _VRAM());
    }

    virtual ~SdkEGLContextWrapper() {

    }

    /**
     * コンテキストを関連づいたステート状態を取得する。
     */
    virtual MGLState getState() {
        return state;
    }

    /**
     * vramを取得する。
     */
    virtual VRAM getVRAM() {
        return vram;
    }

    /**
     * 握っている資源を明示的に開放する
     */
    virtual void dispose() {
        vram.reset();
        state.reset();
        eglContext.reset();
    }
};

}

#endif /* EGLCONTEXTWRAPPER_H_ */
