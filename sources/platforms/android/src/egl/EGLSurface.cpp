/*
 * EGLSurface.cpp
 *
 *  Created on: 2012/07/21

 */

#include    "ndkEGLImpl.h"

namespace ndk {

EGLSurfaceManager::EGLSurfaceManager(EGLDisplay display, EGLSurface surface) {
    this->display = display;
    this->surface = surface;

    // 幅と高さを問い合わせる
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);

    jclogf("surface size = %d x %d", width, height);
}

EGLSurfaceManager::~EGLSurfaceManager() {
    this->dispose();
}

void EGLSurfaceManager::dispose() {
    if( surface ) {
        jclogf("destroy surface = %x", surface);
        if(!eglDestroySurface(display, surface) ){
            jclogf("destroy surface failed = %x", surface);
        }
        surface = NULL;
    }
}

}

