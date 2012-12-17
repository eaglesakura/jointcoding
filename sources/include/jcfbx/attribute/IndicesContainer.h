/*
 * IndicesContainer.h
 *
 *  Created on: 2012/12/17
 */

#ifndef INDICESCONTAINER_H_
#define INDICESCONTAINER_H_

#include    "jointcoding-fbx.h"
#include    "jc/math/Vec2.h"
#include    "jcfbx/attribute/Material.h"

namespace jc {
namespace fbx {

class Polygon {
    /**
     * 頂点インデックス
     */
    u32 index[3];

    /**
     * 頂点ごとのUV情報
     */
    Vector2f uv[3];

    /**
     * 接続されたマテリアル番号
     */
    s32 materialNumber;

public:
    Polygon() {
        for (int i = 0; i < 3; ++i) {
            index[i] = 0;
        }
        materialNumber = 0;
    }

    ~Polygon() {
    }

    void setVertexIndex(s32 a, s32 b, s32 c) {
        index[0] = a;
        index[1] = b;
        index[2] = c;
    }

    void setUV(Vector2f &v0, Vector2f &v1, Vector2f &v2) {
        uv[0] = v0;
        uv[1] = v1;
        uv[2] = v2;
    }

    void setUV(s32 vNum, Vector2f &set) {
        uv[vNum] = set;
    }

    u32* getVertexIndexArray() {
        return index;
    }

    u32 getVertexIndex(s32 idx) const {
        return index[idx];
    }

    Vector2f getVertexUV(s32 idx) const {
        return uv[idx];
    }

    /**
     * ポリゴンに割り当てられたマテリアル番号を設定する
     */
    void setMaterialNumber(s32 num) {
        materialNumber = num;
    }

    /**
     * ポリゴンに割り当てられたマテリアル番号を取得する
     */
    s32 getMaterialNumber() const {
        return materialNumber;
    }
};

class IndicesContainer {
public:
    /**
     * 接続されたマテリアル情報
     */
    std::vector<Material> materials;

    /**
     * 接続されたポリゴン情報
     */
    std::vector<Polygon> polygons;
};

}
}

#endif /* INDICESCONTAINER_H_ */
