/*
 * TextureTable.h
 *
 *  Created on: 2012/12/26
 */

#ifndef TEXTURETABLE_H_
#define TEXTURETABLE_H_

#include    "jointcoding.h"
#include    <map>

namespace jc {

template<typename TextureType>
class TextureTable: public Object {

    typedef jc_sp<TextureType> MTexture;

    /**
     * テクスチャのKey-Valueマップ
     */
    std::map<String, jc_sp<TextureType> > textures;

public:
    TextureTable() {

    }

    virtual ~TextureTable() {

    }

    virtual void clear() {
        textures.clear();
    }

    /**
     * 要素を追加する
     */
    virtual void put( const String &key, const jc_sp<TextureType> texture ) {
        textures.insert( std::map<String, jc_sp<TextureType> >::value_type(key, texture) );
    }

    /**
     * テクスチャ要素を取得する
     */
    virtual jc_sp<TextureType> get( const String &key) const {
        std::map<String, jc_sp<TextureType> >::iterator itr = textures.find(key), end = textures.end();

        if(itr == end) {
            return jc_sp<TextureType>();
        }

        return (*itr);
    }
};

}

#endif /* TEXTURETABLE_H_ */
