/*
 * Material.h
 *
 *  Created on: 2012/12/17
 */

#ifndef JCFBXMATERIAL_H_
#define JCFBXMATERIAL_H_

namespace jc {
namespace fbx {

/**
 * 属性情報
 */
class Material {
    /**
     * 利用するテクスチャ番号を保存する
     */
    s32 textureNum;
public:
    Material() {
        textureNum = 0;
    }

    virtual ~Material() {

    }

    /**
     * テクスチャ番号を設定する
     */
    virtual void setTextureNumber(s32 set) {
        textureNum = set;
    }

    /**
     * テクスチャ番号を取得する
     */
    virtual s32 getTextureNumber() const {
        return textureNum;
    }
};

}
}

#endif /* MATERIAL_H_ */
