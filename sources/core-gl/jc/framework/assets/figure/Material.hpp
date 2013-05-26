/*
 * Material.hpp
 *
 *  Created on: 2013/05/26
 */

#ifndef FIGUREMATERIAL_HPP_
#define FIGUREMATERIAL_HPP_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * 属性情報
 */
enum MaterialAttribute_e {
    /**
     * 位置情報
     */
    MaterialAttribute_Position,

    /**
     * UV
     */
    MaterialAttribute_Coord,

    /**
     * 法線
     */
    MaterialAttribute_Normal,

    /**
     * 頂点ウェイト
     */
    MaterialAttribute_BoneWeight,

    /**
     * 頂点ウェイトインデックス
     */
    MaterialAttribute_BoneIndices,

    /**
     * この番号以降がユーザー拡張で引き渡される
     */
    MaterialAttribute_UserExtension,

    /**
     * デフォルトで規定される量
     */
    MaterialAttribute_e_Num,
};

/**
 * １シェーダー用のマテリアル
 *
 * Materialクラスは複数のシェーダーを組み合わせて一つのバッファを生成する
 */
class MaterialInstance {
protected:
    /**
     * 属性情報
     */
    safe_array<Attribute> attributes;

    /**
     * インスタンスID
     */
    s32 id;
public:
    MaterialInstance(const s32 id) {
        this->id = id;
    }

    virtual ~MaterialInstance() {
    }
};

/**
 * フィギュア描画用のマテリアルを定義する
 *
 */
class Material: public Object {
public:
    Material() {
    }

    virtual ~Material() {
    }
};

}
}

#endif /* MATERIAL_HPP_ */
