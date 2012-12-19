/*
 * VertexContainer.h
 *
 *  Created on: 2012/12/17
 */

#ifndef VERTEXCONTAINER_H_
#define VERTEXCONTAINER_H_

#include    "jointcoding-fbx.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Vec2.h"
#include    "jc/graphics/figure/mesh/BoneWeight.h"
#include    <vector>

namespace jc {
namespace fbx {

/**
 * FBXの頂点を一元管理する
 * FBXが保持していたままの配列のため、各配列数が一致するとは限らない。
 *
 * positions.size == weights.size
 * normals.size == coords.size
 */
class VertexContainer {
public:
    /**
     * 位置情報
     */
    std::vector<Vector3f> positions;

    /**
     * 法線
     */
    std::vector<Vector3f> normals;

    /**
     * UV情報
     */
    std::vector<Vector2f> coords;

    /**
     * 頂点の重み情報
     */
    std::vector<SimpleBoneWeight> weights;

};

}
}

#endif /* VERTEXCONTAINER_H_ */
