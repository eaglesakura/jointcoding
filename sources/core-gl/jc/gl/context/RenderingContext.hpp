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

    /**
     * レンダリングサーフェイスのサイズが変更された
     * 例えば端末の縦横を回転させた場合がそれに当たる。
     */
    virtual void onPlatformViewResized(RenderingContext *pContext, const MRenderingSurface now, const s32 newWidth, const s32 newHeight) {
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
    RenderingContext() {
    }

    virtual ~RenderingContext() {
    }

    /**
     * 初期化を行う
     * デバイスの移行を行う場合も設定し直す
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

    virtual VideoMemory getVRAM(const VRAM_e type) const {
        return device->getVRAM(type);
    }

    virtual MGLState getState() const {
        assert(device);
        return device->getState();
    }

    virtual MDevice getDevice() const {
        assert(device);
        return device;
    }

    /**
     * 現在のレンダリングターゲットを取得する
     */
    virtual MRenderingSurface getRenderingTarget() const {
        return renderStack->getCurrentRenderingTarget();
    }

    /**
     * 現在の仮想ディスプレイを取得する
     */
    virtual MVirtualDisplay getVirtualDisplay() const {
        return renderStack->getCurrentVirtualDisplay();
    }

    /**
     * 現在の仮想ディスプレイアスペクト比を取得する
     */
    virtual float getVirtualDisplayAspect() const {
        return getVirtualDisplay()->getVirtualDisplayAspect();
    }

    /**
     * 現在の仮想ディスプレイサイズを取得する
     */
    virtual const Vector2f& getVirtualDisplaySize() const {
        return getVirtualDisplay()->getVirtualDisplaySize();
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

    /**
     * 現在の仮想ディスプレイ設定に基づいてviewportを指定する
     */
    virtual void viewportVirtual() const {
        // 現在のviewportを変更する
        MVirtualDisplay display = getVirtualDisplay();
        RectF viewport = display->getDisplayViewport();

        // Viewport指定を行う
        getState()->viewport2D(display->getRealDisplayWidth(), display->getRealDisplayHeight(), viewport.left, viewport.top, viewport.width(), viewport.height());
    }

    /**
     * Platform用のView（SurfaceView/TextureView/SurfaceHolder/UIView）のサイズが変更された
     *
     * このメッセージはフレームの頭に到達するため、必ずEGLSurfaceProtocolが設定されていると仮定できる。
     */
    virtual void onPlatformViewResized(const s32 newWidth, const s32 newHeight) {
        assert(device);
        assert(renderStack->getSurfaceDepth() == 1);

        // viewportを変更する
        {
            MVirtualDisplay display = getVirtualDisplay();
            display->setRealDisplaySize(newWidth, newHeight);
            display->updateViewport(VirtualDisplay::FitType_Auto);
            viewportVirtual();
        }

        MRenderingSurface surface = getRenderingTarget();
        ContextListenerList::iterator itr = listeners.begin(), end = listeners.end();
        while (itr != end) {
            RenderingContextListener *pListener = itr->get();
            if (pListener) {
                // コールバック
                pListener->onPlatformViewResized(this, surface, newWidth, newHeight);
                ++itr;
            } else {
                itr = listeners.erase(itr);
                end = listeners.end();
            }

        }
    }
};

typedef jc_sp<RenderingContext> MRenderingContext;

}
}

#endif /* RENDERINGCONTEXT_HPP_ */
