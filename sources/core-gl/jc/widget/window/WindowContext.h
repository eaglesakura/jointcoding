/*
 * WindowContext.h
 *
 *  Created on: 2013/04/11
 */

#ifndef WINDOWCONTEXT_H_
#define WINDOWCONTEXT_H_

#include    "jointcoding.h"
#include    "jc/gl/SpriteManager.h"

namespace jc {
namespace view {

using namespace gl;

class Window;
/**
 * View階層を維持するために必要なWindow共通情報を設定する
 */
class WindowContext {
    /**
     * Window本体.
     */
    jc_wp<Window> window;

    /**
     * レンダリング用のスプライトマネージャ
     */
    MSpriteManager spriteManager;
public:
    WindowContext() {
    }

    virtual ~WindowContext() {
        jclogf("delete WindowContext(%x)", this);
    }

    /**
     * 一時的にWindowへアクセスする
     */
    virtual jc_sp<Window> lockWindow() const {
        return window.lock();
    }

    /**
     * ウィンドウを設定する
     */
    virtual void setWindow(jc_wp<Window> window) {
        this->window = window;
    }

    /**
     * スプライト管理クラスを取得する
     */
    virtual MSpriteManager getSpriteManager() const {
        return spriteManager;
    }

    /**
     * スプライト管理クラスを登録する
     */
    virtual void setSpriteManager(const MSpriteManager spriteManager) {
        this->spriteManager = spriteManager;
    }
};

/**
 * context
 */
typedef jc_sp<WindowContext> MWindowContext;}
}

#endif /* WINDOWCONTEXT_H_ */
