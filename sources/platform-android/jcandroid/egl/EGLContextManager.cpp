/*
 * ndkEGLContextManager.cpp
 *
 *  Created on: 2012/07/21
 */
#include    "jcandroid/egl/EGLContextManager.h"
#include    "EGL/egl.h"

namespace ndk {

EGLContextManager::EGLContextManager(EGLContext context, EGLDisplay display, EGLConfig config) {
    this->display = display;
    this->config = config;
    this->context = context;
    this->state.reset(new GLState());
    this->vram.reset(new _VRAM());
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

