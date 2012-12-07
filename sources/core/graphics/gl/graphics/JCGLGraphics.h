/*
 * JCGLGraphics.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCGLGRAPHICS_H_
#define JCGLGRAPHICS_H_

#include    "jcGL.h"
#include    "jcEGL.h"
#include    "jcGLState.h"
#include    "jcGLObject.h"

namespace jc {

namespace gl {

/**
 * 描画用ヘルパを提供する。
 */
class GLGraphics: public Object {
    /**
     * 画面クリア時のフラグ
     */
    u32 clearFlags;

    /**
     * EGL用管理クラス
     */
    MEGLProtocol egl;

public:
    GLGraphics(MEGLProtocol egl);
    virtual ~GLGraphics();

    /**
     * 画面を指定色でクリアする
     */
    virtual void cleari(const GLfixed r, const GLfixed g, const GLfixed b, const GLfixed a);

    /**
     * 画面を指定色でクリアする
     */
    virtual void clearf(const float r, const float g, const float b, const float a);

    /**
     * 色を指定してクリアする
     */
    virtual void clear(const rgba32 color);

    /**
     * GL座標系に基づいてViewportを設定する
     */
    virtual void setViewport(const s32 x, const s32 y, const s32 width, const s32 heigth);

    /**
     * 関連付けられたステートを取得する
     */
    virtual MGLState getState() const;
};

}
}

#endif /* JCGLGRAPHICS_H_ */
