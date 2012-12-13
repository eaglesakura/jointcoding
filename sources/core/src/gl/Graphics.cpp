/*
 * GLGraphics.cpp
 *
 *  Created on: 2012/06/26
 *      Author: Takeshi
 */

#include    "jcGLState.h"
#include    "jcGLGraphics.h"
#include    "jcglEGLSurface.h"
#include    "jcglEGLContext.h"

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

void GLGraphics::cleari(const GLfixed r, const GLfixed g, const GLfixed b, const GLfixed a) {
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
