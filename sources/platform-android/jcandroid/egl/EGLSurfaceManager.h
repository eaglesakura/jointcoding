/*
 * ndkEGLSurfaceManager.h
 *
 *  Created on: 2012/07/21

 */

#ifndef NDKEGLSURFACEMANAGER_H_
#define NDKEGLSURFACEMANAGER_H_

#include    "EGLImpl.h"

namespace ndk {
using namespace jc;
using namespace jc::gl;

/**
 * レンダリング対象サーフェイスを管理する
 */
class EGLSurfaceManager: public Object, public EGLSurfaceProtocol {
    s32 width;
    s32 height;

    /**
     * 描画対象サーフェイス
     */
    EGLSurface surface;

    /**
     * 描画対象ディスプレイ
     */
    EGLDisplay display;
public:
    EGLSurfaceManager(EGLDisplay display, EGLSurface surface);

    virtual ~EGLSurfaceManager();

    /**
     * サーフェイスの幅を取得する。
     */
    virtual s32 getWidth() const {
        return width;
    }

    /**
     * サーフェイスの高さを取得する。
     */
    virtual s32 getHeight() const {
        return height;
    }

    /**
     * ネイティブのサーフェイスを取得する
     */
    virtual EGLSurface  getSurface( ) const {
        return surface;
    }

    /**
     * 資源の開放を行う
     */
    virtual void dispose();

    /**
     * サーフェイスサイズをチェックする
     */
    virtual void    onSurfaceResized();
};

}

#endif /* NDKEGLSURFACEMANAGER_H_ */
