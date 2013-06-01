/*
 * TextureCache.hpp
 *
 *  Created on: 2013/06/01
 */

#ifndef TEXTURECACHE_HPP_
#define TEXTURECACHE_HPP_

#include    "jc/framework/assets/ImageAsset.hpp"

namespace jc {
namespace fw {

/**
 * テクスチャ読み込み用のキャッシュ処理を行う
 */
class TextureCache: public Object {
protected:
    /**
     * テクスチャキャッシュ
     */
    std::map<u32, MTextureImage> textures;

public:
    TextureCache() {
    }

    virtual ~TextureCache() {
    }

    /**
     * 明示的にキャッシュを投入する
     */
    virtual jcboolean putCache(const u32 uniqueId, const MTextureImage tex) {
        if (textures.find(uniqueId) == textures.end()) {
            return jcfalse;
        }

        textures.insert(std::map<u32, MTextureImage>::value_type(uniqueId, tex));
        return jctrue;
    }

    /**
     * 明示的にキャッシュを投入する
     */
    virtual jcboolean putCache(const String &name, const MTextureImage tex) {
        return putCache((u32) name.hashCode(), tex);
    }

    /**
     * 明示的にキャッシュを投入する
     */
    virtual jcboolean putCache(const Uri &uri, const MTextureImage tex) {
        return putCache(uri.getUri(), tex);
    }

    /**
     * キャッシュされているテクスチャ数を取得する
     */
    virtual u32 getTextureNum() const {
        return textures.size();
    }

    /**
     * キャッシュされているテクスチャを取得する
     * 見つからなかったらNULLオブジェクトを返す
     */
    virtual MTextureImage get(const u32 uniqueId) const {
        std::map<u32, MTextureImage>::const_iterator itr = textures.find(uniqueId);
        if (itr != textures.end()) {
            return itr->second;
        }

        return MTextureImage();
    }

    /**
     * 名前からキャッシュを取得する
     */
    virtual MTextureImage get(const String &name) const {
        return get((u32) name.hashCode());
    }

    /**
     * 名前からキャッシュを取得する
     */
    virtual MTextureImage get(const Uri &uri) const {
        return get((u32) uri.getUri().hashCode());
    }

    /**
     * キャッシュをクリアする
     */
    virtual void clear() {
        textures.clear();
    }

    /**
     * キャッシュにテクスチャがあればキャッシュから返し、無ければ新規にロードして返す
     */
    virtual MTextureImage getOrLoad(MDevice device, const Uri &uri, const PixelFormat_e pixFormat = PixelFormat_NativeRGBA, TextureLoadOption *option = NULL) {
        MTextureImage result = get(uri);
        if (result) {
            return result;
        }

        result = TextureImage::decode(device, uri, pixFormat, option);
        if (result) {
            putCache(uri, result);
        }

        return result;
    }
};

typedef jc_sp<TextureCache> MTextureCache;

}
}

#endif /* TEXTURECACHE_HPP_ */
