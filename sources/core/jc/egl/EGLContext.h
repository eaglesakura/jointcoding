/*
 * jcglEGLContext.h
 *
 *  Created on: 2012/08/22
 */

#ifndef JCGLEGLCONTEXT_H_
#define JCGLEGLCONTEXT_H_

#include    "jc/system/Macro.h"
#include    "jc/gl/State.h"
#include    "jc/gl/VRAM.h"

namespace jc {
namespace gl {

/**
 * レンダリングコンテキストを管理するためのプロトコル
 * 実装は各プラットフォームに依存する。
 */
class EGLContextProtocol {
public:
    virtual ~EGLContextProtocol() {
    }

    /**
     * コンテキストを関連づいたステート状態を取得する。
     */
    virtual MGLState getState() = 0;

    /**
     * vramを取得する。
     */
    virtual VRAM getVRAM() = 0;

    /**
     * 握っている資源を明示的に開放する
     */
    virtual void dispose() = 0;
};

typedef jc_sp<EGLContextProtocol> MEGLContextProtocol;

#define EGL_NULL_CONTEXT   MEGLContextProtocol()

}
}

#endif /* JCGLEGLCONTEXT_H_ */
