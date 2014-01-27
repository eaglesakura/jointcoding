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

    std::vector<jc_sp<TextureType>, StlAllocator<jc_sp<TextureType> > > value_array;

    std::vector<String, StlAllocator<String> > key_array;

public:
    TextureTable() {
    }

    virtual ~TextureTable() {
    }

    virtual void clear() {
        value_array.clear();
        key_array.clear();
    }

    /**
     * 要素を追加する
     */
    virtual void put( const String &key, const jc_sp<TextureType> texture ) {
        if(get(key) ) {
            return;
        }

        key_array.push_back(key);
        value_array.push_back(texture);
    }

    /**
     * テクスチャ要素を取得する
     */
    virtual jc_sp<TextureType> get( const String &key) {
        std::vector<String >::iterator itr = key_array.begin(), end = key_array.end();

        int index = 0;
        while(itr != end) {
            if(*itr == key) {
                jc_sp<TextureType> cache = value_array[index];
                if(cache && cache->exist()) {
                    return cache;
                } else {
                    jclogf("texture dead cache!! (%d)", cache.get());
                    return jc_sp<TextureType>();
                }
            }
            ++index;
            ++itr;
        }
        return jc_sp<TextureType>();
    }

    virtual size_t size() {
        return key_array.size();
    }
};

}

#endif /* TEXTURETABLE_H_ */
