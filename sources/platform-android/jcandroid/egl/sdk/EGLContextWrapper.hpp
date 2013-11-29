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

    /**
     * 管理対象のVRAM
     */
    VideoMemory rams[VRAM_e_num];

public:
    jc_sp<EGLContextWrapper> eglContext;
public:
    SdkEGLContextWrapper(const jc_sp<EGLContextWrapper> eglContext) {
        this->eglContext = eglContext;

        state.reset(new GLState());

        for (int i = 0; i < VRAM_e_num; ++i) {
            this->rams[i].reset(new _VideoMemory((VRAM_e) i));
        }
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
     * VRAM取得
     */
    virtual VideoMemory getVRAM(const VRAM_e type) {
        return rams[type];
    }

    /**
     * Context内のgcを行う
     */
    virtual void gc() {
        // テクスチャのバインドを外す
        {
            unsafe_array<u32> target;
            rams[VRAM_Texture]->getGcTargets(&target);
            state->unbindTextures(target.length, target.ptr);
        }

        // 全VRAMのGCを行う
        for(int i = 0; i < VRAM_e_num; ++i) {
            rams[i]->gc();
        }
    }

    /**
     * 握っている資源を明示的に開放する
     */
    virtual void dispose() {
        // 全VRAMのdisposeを行う
        if(rams[0]) {
            for(int i = 0; i < VRAM_e_num; ++i) {
                rams[i]->dispose();
                rams[i].reset();
            }
        }

        state.reset();
        if(eglContext) {
            eglContext->dispose();
            eglContext.reset();
        }
    }
};

}

#endif /* EGLCONTEXTWRAPPER_H_ */
