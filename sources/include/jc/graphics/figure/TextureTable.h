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

    std::vector<jc_sp<TextureType> > value_array;

    std::vector<String> key_array;

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
                return value_array[index];
            }
            ++index;
        }
        return jc_sp<TextureType>();
    }
};

}

#endif /* TEXTURETABLE_H_ */
