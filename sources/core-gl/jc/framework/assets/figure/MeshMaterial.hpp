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

    /**
     * 識別用の一意なID
     */
    u32 uniqueId;
public:
    MeshMaterial() {
        uniqueId = 0;
    }

    virtual ~MeshMaterial() {

    }

    /**
     * マテリアル名を持っていたらtrueを返す
     */
    virtual jcboolean hasName() const {
        return uniqueId != 0;
    }

    /**
     * マテリアル名を取得する
     */
    virtual String getName() const {
        return name;
    }

    /**
     * マテリアル名を設定する
     * 名前のハッシュに寄って一意を決定しているため、uniqueIdも更新される。
     */
    virtual void setName(const String &name) {
        this->name = name;
        uniqueId = name.hashCode();
    }

    /**
     * マテリアルを示す一意のIDを返す。
     * 通常はマテリアル名のhashCodeがあれば十分。
     */
    virtual u32 getUniqueId() const {
        return uniqueId;
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
