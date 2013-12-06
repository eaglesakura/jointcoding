/*
 * TextureFactory.hpp
 *
 *  Created on: 2013/06/01
 */

#ifndef TEXTUREFACTORY_HPP_
#define TEXTUREFACTORY_HPP_

#include    "jc/framework/assets/ImageAsset.hpp"

namespace jc {
namespace fw {

/**
 * FigureLoaderからテクスチャを生成させるためのファクトリクラス
 */
class TextureFactory: public Object {
protected:
    /**
     * 読み込み済みのテクスチャキャッシュ
     */
    MTextureCache cache;

public:
    TextureFactory() {
        cache.reset(mark_new TextureCache());
    }

    virtual ~TextureFactory() {
    }

    /**
     * キャッシュを設定する
     */
    virtual void setTextureCache(const MTextureCache texCache) {
        this->cache = texCache;
    }

    /**
     * テクスチャのロードを行わせる
     */
    virtual MTextureImage load(MDevice device, const String &fileName, const PixelFormat_e pixFormat = PixelFormat_NativeRGBA, TextureLoadOption *option = NULL) = 0;
};

/**
 * managed
 */
typedef jc_sp<TextureFactory> MTextureFactory;

}
}

#endif /* TEXTUREFACTORY_HPP_ */
