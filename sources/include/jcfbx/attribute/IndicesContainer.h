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

/**
 * Fbxコンバート前三角情報.
 * 必ず三角ポリで構成する
 */
struct ConvertedPolygon {
    /**
     * 位置・法線
     */
    s32 position[3]; //!<
    s32 attributes[3]; //!<    UV/Normal座標
    s32 material; //!<    マテリアル番号
};


/**
 * インデックスバッファを格納する。
 */
class IndicesContainer {
public:
    /**
     * 接続されたマテリアル情報
     */
    std::vector<Material> materials;

    /**
     * 接続されたポリゴン情報
     */
    std::vector<ConvertedPolygon> polygons;
};

}
}

#endif /* INDICESCONTAINER_H_ */
