/*
 * RenderingContext.hpp
 *
 *  Created on: 2013/06/03
 */

#ifndef RENDERINGCONTEXT_HPP_
#define RENDERINGCONTEXT_HPP_

#include    "jointcoding.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/gl/surface/IRenderingSurface.hpp"

namespace jc {
namespace gl {

class RenderingContext;
/**
 * レンダリングコンテキストの操作を受け取るリスナ
 * 必要なメソッドのみオーバーライドする。
 */
class RenderingContextListener {
public:
    RenderingContextListener() {
    }

    virtual ~RenderingContextListener() {
    }

    /**
     * レンダリングサーフェイスが変更された
     */
    virtual void onRenderingSurfaceChanged(RenderingContext *pContext, const MRenderingSurface old, const MRenderingSurface now) {
    }
};

/**
 * selector
 */
typedef jc_selp<RenderingContextListener> SRenderingContextListener;

/**
 * レンダリングに必要になる情報を一塊にしたクラス
 */
class RenderingContext: public Object {
protected:
    /**
     * ウィンドウデバイス
     */
    MDevice device;

    /**
     * レンダリングスタック
     */
    MRenderingStack renderStack;

    typedef typename std::list<SRenderingContextListener> ContextListenerList;
    /**
     * リスナ
     * ポインタ保持はセレクト式であるため、場合によっては各々でポインタ生存期間を管理すること
     */
    ContextListenerList listeners;

    /**
     *
     */
    virtual void onRenderingSurfaceChanged(const MRenderingSurface old, const MRenderingSurface now) {
        {
            // 仮想ディスプレイのレンダリング領域を変更する
            virtualDisplay->setRealDisplaySize(surface->getWidth(), surface->getHeight());
            virtualDisplay->updateViewport(VirtualDisplay::FitType_Auto);
        }

        // リスナを呼び出す
        {
            ContextListenerList::iterator itr = listeners.begin(), end = listeners.end();
            while (itr != end) {
                RenderingContextListener *pListener = itr->get();
                if (pListener) {
                    // コールバック
                    pListener->onRenderingSurfaceChanged(this, old, renderStack->getCurrentRenderingTarget());
                    ++itr;
                } else {
                    itr = listeners.erase(itr);
                    end = listeners.end();
                }

            }
        }
    }
public:
    RenderingContext(MDevice device) {
    }

    virtual ~RenderingContext() {
    }

    /**
     * 初期化を行う
     */
    virtual void initialize(MDevice device) {
        assert(device);
        this->device = device;

        // レンダリングスタックの最初はEGLSurfaceである
        {
            renderStack.reset(new RenderingStack());
            pushSurface(device->getSurface());
        }
    }

    virtual VRAM getVRAM() const {
        assert(device);
        return device->getVRAM();
    }

    virtual MGLState getState() const {
        assert(device);
        return device->getState();
    }

    virtual MDevice getDevice() const {
        assert(device);
        return device;
    }

    virtual MRenderingSurface getCurrentRenderingTarget() const {
        return renderStack->getCurrentRenderingTarget();
    }

    /**
     * 現在の仮想ディスプレイを取得する
     */
    virtual MVirtualDisplay getCurrentVirtualDisplay() const {
        return renderStack->getCurrentVirtualDisplay();
    }

    /**
     * リスナーを追加する
     * リスナーの生存期間はセレクト式であるため、特に生ポインタで管理している場合は十分に注意して生存期間を管理すること。
     */
    virtual void addListener(const SRenderingContextListener listener) {
        listeners.push_back(listener);
    }

    /**
     * 指定したリスナを削除する
     */
    virtual void removeListener(const SRenderingContextListener listener) {
        listeners.remove(listener);
    }

    /**
     * 生存期間を過ぎたリスナを削除する
     */
    virtual void cleanupListener() {
        ContextListenerList::iterator itr = listeners.begin(), end = listeners.end();
        while (itr != end) {
            if (itr->exist()) {
                ++itr;
            } else {
                itr = listeners.erase(itr);
                end = listeners.end();
            }

        }
    }

    /**
     * レンダリングサーフェイスを更新する
     */
    virtual MRenderingSurface pushSurface(const MRenderingSurface surface) {
        MRenderingSurface old = renderStack->pushSurface(surface);

        onRenderingSurfaceChanged(old, surface);
        return old;
    }

    /**
     * レンダリングサーフェイスを一つ書き戻す
     */
    virtual MRenderingSurface popSurface() {
        MRenderingSurface old = renderStack->popSurface();

        onRenderingSurfaceChanged(old, renderStack->getCurrentRenderingTarget());
        return old;
    }

    /**
     * レンダリングスタックを取得する
     */
    virtual MRenderingStack getRenderingStack() const {
        return renderStack;
    }
};

typedef jc_sp<RenderingContext> MRenderingContext;

}
}

#endif /* RENDERINGCONTEXT_HPP_ */
