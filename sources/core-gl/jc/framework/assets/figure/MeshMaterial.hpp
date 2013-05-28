/*
 * MeshMaterial.hpp
 *
 *  Created on: 2013/05/28
 */

#ifndef MESHMATERIAL_HPP_
#define MESHMATERIAL_HPP_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * メッシュに付与されるマテリアル情報を定義する
 */
class MeshMaterial: public Object {
    /**
     * メッシュ名
     */
    String name;

    /**
     * 拡散反射用テクスチャ
     */
    MTextureImage diffuse;
public:
    MeshMaterial() {

    }

    virtual ~MeshMaterial() {

    }

    /**
     * マテリアルを示す一意のIDを返す。
     * 通常はマテリアル名のhashCodeがあれば十分。
     */
    virtual u32 getUniqueId() const {
        return (u32) name.hashCode();
    }

    /**
     * 拡散反射テクスチャを取得する
     */
    virtual MTextureImage getDiffuse() const {
        return diffuse;
    }

    /**
     * 拡散反射テクスチャを設定する
     */
    virtual void setDiffuse(const MTextureImage diffuse) {
        this->diffuse = diffuse;
    }
};

}
}

#endif /* MESHMATERIAL_HPP_ */
