/*
 * PKMHeader.h
 *
 *  Created on: 2013/02/25
 */

#ifndef PKMHEADER_H_
#define PKMHEADER_H_

#include    "jointcoding.h"
#include    "jc/math/Math.h"
#include    "jc/io/InputStream.h"

namespace jc {
namespace gl {

class PKMHeader: public Object {
    /**
     * ファイルバージョン
     * 通常は'1''0'
     */
    charactor version[2];

    /**
     * テクスチャ幅
     */
    u32 width;

    /**
     * テクスチャ高さ
     */
    u32 height;

    /**
     * 変換前のテクスチャ幅
     */
    u32 originalWidth;

    /**
     * 変換前のテクスチャ高さ
     */
    u32 originalHeight;

    /**
     * data type
     * ETC1 = 0x0000
     */
    u16 dataType;

    PKMHeader();
public:
    virtual ~PKMHeader();

    virtual const u32 getWidth() const {
        return width;
    }

    virtual const u32 getHeight() const {
        return height;
    }

    virtual const u32 getOriginalWidth() const {
        return originalWidth;
    }

    virtual const u32 getOriginalHeight() const {
        return originalHeight;
    }

    virtual const charactor* getVersion() const {
        return version;
    }

    virtual jcboolean isETC1() const {
        return dataType == 0;
    }

    /**
     * npotテクスチャであればtrueを返す
     */
    virtual jcboolean isNPOT() const {
        return !(jc::isPowerOfTwo(width) && jc::isPowerOfTwo(height));
    }

    /**
     * ヘッダを作成する.
     * 読込に失敗した場合はNULLを返す。
     */
    static jc_sp<PKMHeader> createInstance(MInputStream stream);
};

/**
 * managed
 */
typedef jc_sp<PKMHeader> MPKMHeader;

}
}

#endif /* PKMHEADER_H_ */
