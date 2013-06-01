/*
 * UriTextureFactory.hpp
 *
 *  Created on: 2013/06/01
 */

#ifndef URITEXTUREFACTORY_HPP_
#define URITEXTUREFACTORY_HPP_

#include    "jc/framework/assets/ImageAsset.hpp"

namespace jc {
namespace fw {

class UriTextureFactory: public TextureFactory {

    /**
     * Root用のURI
     */
    String uriHead;

    /**
     * ファイル拡張子
     */
    String ext;
public:
    UriTextureFactory() {

    }

    virtual ~UriTextureFactory() {

    }

    /**
     * ファイル名に拡張子を付与する
     *
     * .png
     * .ktx
     */
    virtual void setFilenameExt(const String &ext) {
        this->ext = ext;
    }

    /**
     * assetsディレクトリから取り出す
     */
    virtual void fromAssets(const String directory) {
        uriHead = Uri::fromAssets(directory).getUri();
    }

    /**
     * テクスチャのロードを行わせる
     */
    virtual MTextureImage load(MDevice device, const String &fileName, const PixelFormat_e pixFormat = PixelFormat_NativeRGBA, TextureLoadOption *option = NULL) {
        Uri uri = Uri::parse(uriHead + fileName + ext);
        return cache->getOrLoad(device, uri, pixFormat, option);
    }
};

}
}

#endif /* URITEXTUREFACTORY_HPP_ */
