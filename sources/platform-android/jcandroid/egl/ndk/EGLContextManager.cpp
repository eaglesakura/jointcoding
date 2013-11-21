/*
 * ndkEGLContextManager.cpp
 *
 *  Created on: 2012/07/21
 */
#include    "jcandroid/egl/ndk/EGLContextManager.h"
#include    "EGL/egl.h"

namespace ndk {

EGLContextManager::EGLContextManager(EGLContext context, EGLDisplay display, EGLConfig config) {
    this->display = display;
    this->config = config;
    this->context = context;
    this->state.reset(mark_new GLState());
    this->vram.reset(mark_new _VRAM());
}

EGLContextManager::~EGLContextManager() {
    this->dispose();
}

/**
 * 資源の開放を行う
 */
void EGLContextManager::dispose() {
    vram.reset();

    if (context) {
        jclogf("destroy context = %x", context);
        if (!eglDestroyContext(display, context)) {
            jcalertf("destroy context failed = %x", context);
        }
        context = NULL;
    }
}

}

