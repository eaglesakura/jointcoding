/*
 * GLGraphics.cpp
 *
 *  Created on: 2012/06/26
 *      Author: Takeshi
 */

#include    "jc/gl/State.h"
#include    "jc/gl/Graphics.h"
#include    "jc/egl/EGLSurface.h"
#include    "jc/egl/EGLContext.h"

namespace jc {
namespace gl {

GLGraphics::GLGraphics(MEGLProtocol egl) {
    this->egl = egl;
    clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    // viewportの標準設定を行う
//    renderSurface->setViewport(0, 0, window->getWidth(), window->getHeight());
}

GLGraphics::~GLGraphics() {
}

void GLGraphics::cleari(const s32 r, const s32 g, const s32 b, const s32 a) {
    getState()->clearColorf(fixed2float(r), fixed2float(g), fixed2float(b), fixed2float(a));
    getState()->clear(clearFlags);
}

void GLGraphics::clearf(const float r, const float g, const float b, const float a) {
    getState()->clearColorf(r, g, b, a);
    getState()->clear(clearFlags);
}
/**
 * 色を指定してクリアする
 */
void GLGraphics::clear(const rgba32 color) {
    Color col;
    col.colors.rgba = color;
    getState()->clearColorf(col.rf(), col.gf(), col.bf(), col.af());
    getState()->clear(clearFlags);
}

void GLGraphics::setViewport(const s32 x, const s32 y, const s32 width, const s32 heigth) {
    getState()->viewport(x, y, width, heigth);
}

/**
 * 関連付けられたステートを取得する
 */
MGLState GLGraphics::getState() const {
    throw create_exception(StubOperationException, "");
}

}
}
