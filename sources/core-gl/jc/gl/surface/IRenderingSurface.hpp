/*
 * IRenderSurface.hpp
 *
 *  Created on: 2013/06/03
 */

#ifndef IRENDERINGSURFACE_HPP_
#define IRENDERINGSURFACE_HPP_

#include    "jointcoding.h"
#include    <list>
#include    "jc/gl/surface/VirtualDisplay.hpp"

namespace jc {
namespace gl {

/**
 * レンダリング用サーフェイスを定義する
 */
class IRenderingSurface {
public:
    IRenderingSurface() {

    }

    virtual ~IRenderingSurface() {
    }

    /**
     * サーフェイス幅を取得する
     */
    virtual s32 getWidth() const = 0;

    /**
     * サーフェイス高さを取得する
     */
    virtual s32 getHeight() const = 0;
};

/**
 * managed
 */
typedef jc_sp<IRenderingSurface> MRenderingSurface;

/**
 * 現在レンダリング対象のサーフェイスをスタックする。
 * 一番下は必ずEGLSurfaceとなる。
 *
 * width/height等のサーフェイス情報は仮想ディスプレイが基本となる。
 * 仮想ディスプレイはpush/popで自動的に設定できるが、明示的に修正することも可能。
 */
class RenderingStack: public Object {
    std::list<MRenderingSurface> surfaceStack;

    /**
     * 現在レンダリング対象となるサーフェイス
     */
    MRenderingSurface currentSurface;

    /**
     * 仮想ディスプレイスタック
     */
    std::list<MVirtualDisplay> displayStack;

    /**
     * 現在のレンダリング対象の仮想ディスプレイ
     */
    MVirtualDisplay currentDisplay;
public:
    RenderingStack() {

    }

    virtual ~RenderingStack() {
    }

    /**
     * レンダリングサーフェイスを一つ追加する
     * 自動的に仮想ディスプレイがひとつ積まれ、ディスプレイがセットされる。
     *
     * @return 古いレンダリングサーフェイス
     */
    virtual MRenderingSurface pushSurface(const MRenderingSurface surface) {
        // 現在の値を保存する
        MRenderingSurface result = currentSurface;

        // push
        {
            surfaceStack.push_front(surface);
            currentSurface = surface;
        }
        {
            MVirtualDisplay display(new VirtualDisplay());
            display->setRealDisplaySize(surface->getWidth(), surface->getHeight());
            display->setVirtualDisplaySize(surface->getWidth(), surface->getHeight());
            display->updateViewport(VirtualDisplay::FitType_Auto);

            currentDisplay = display;
            displayStack.push_front(display);
        }

        return result;
    }

    /**
     * レンダリングサーフェイスを一つ戻す
     * 自動的に仮想ディスプレイがひとつ戻される
     *
     * @return 古いレンダリングサーフェイス
     */
    virtual MRenderingSurface popSurface() {
        // 現在の値を保存する
        MRenderingSurface result = currentSurface;

        // pop
        {
            surfaceStack.pop_front();
            assert(!surfaceStack.empty()); // emptyになってはならない（最下層にはEGLSurfaceが存在するはずである）
            currentSurface = surfaceStack.front();
        }
        {
            displayStack.pop_front();
            assert(!displayStack.empty()); // emptyになってはならない（最下層にはEGLSurfaceが存在するはずである）
            currentDisplay = displayStack.front();
        }

        return result;
    }

    /**
     * 現在の描画先を取得する
     */
    virtual MRenderingSurface getCurrentRenderingTarget() const {
        return currentSurface;
    }

    /**
     * 現在の仮想ディスプレイを取得する。
     * 初期値は必ずサーフェイス値と一致している。
     */
    virtual MVirtualDisplay getCurrentVirtualDisplay() const {
        return currentDisplay;
    }

    /**
     * サーフェイスサイズの階層数を取得する
     */
    virtual s32 getSurfaceDepth() const {
        return surfaceStack.size();
    }
};

/**
 * managed
 */
typedef jc_sp<RenderingStack> MRenderingStack;

}
}

#endif /* IRENDERINGSURFACE_HPP_ */
