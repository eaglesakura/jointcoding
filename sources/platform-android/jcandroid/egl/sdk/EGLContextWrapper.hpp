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

    /**
     * 管理対象のVRAM
     */
    MVideoMemory rams[VRAM_e_num];

public:
    jc_sp<EGLContextWrapper> eglContext;
public:
    SdkEGLContextWrapper(const jc_sp<EGLContextWrapper> eglContext) {
        this->eglContext = eglContext;

        state.reset(new GLState());
        vram.reset(new SharedVRAM());

        for (int i = 0; i < VRAM_e_num; ++i) {
            this->rams[i].reset(new VideoMemory((VRAM_e) i));
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
     * vramを取得する。
     */
    virtual VRAM getVRAM() {
        return vram;
    }

    /**
     * VRAM取得
     */
    virtual MVideoMemory getVRAM(const VRAM_e type) {
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
        vram.reset();

        // 全VRAMのdisposeを行う
        for(int i = 0; i < VRAM_e_num; ++i) {
            if(rams[i]) {
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
