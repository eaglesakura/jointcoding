/*
 * RenderingSurfaceStub.hpp
 *
 *  Created on: 2013/12/14
 */
#ifndef RENDERINGSURFACESTUB_HPP_
#define RENDERINGSURFACESTUB_HPP_

#include    "jc/gl/surface/IRenderingSurface.hpp"

namespace jc {
namespace gl {
/**
 * レンダリングサーフェイス用スタブを設定する
 * 一時的なレンダリング領域を生成する場合に利用する
 */
class RenderingSurfaceStub: public Object, public IRenderingSurface {
    s32 width;
    s32 height;
public:
    RenderingSurfaceStub(const s32 width, const s32 height) {
        this->width = width;
        this->height = height;
    }

    static jc_sp<IRenderingSurface> createInstance(const s32 width, const s32 height) {
        return jc_sp<IRenderingSurface>(new RenderingSurfaceStub(width, height));
    }

    static jc_sp<IRenderingSurface> createInstance(const Vector2i &size) {
        return jc_sp<IRenderingSurface>(new RenderingSurfaceStub(size.x, size.y));
    }

    virtual ~RenderingSurfaceStub() {

    }

    /**
     * サーフェイス幅を取得する
     */
    virtual s32 getWidth() const {
        return width;
    }

    /**
     * サーフェイス高さを取得する
     */
    virtual s32 getHeight() const {
        return height;
    }
};

}
}

#endif /* RENDERINGSURFACESTUB_HPP_ */
