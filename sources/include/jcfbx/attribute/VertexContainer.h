/*
 * VertexContainer.h
 *
 *  Created on: 2012/12/17
 */

#ifndef VERTEXCONTAINER_H_
#define VERTEXCONTAINER_H_

#include    "jointcoding.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Vec2.h"
#include    "jcfbx/attribute/BoneWeight.h"
#include    <vector>

namespace jc {
namespace fbx {

/**
 * 頂点情報
 */
class SimpleVertex {
    /**
     * 位置情報
     */
    Vector3f position;

    /**
     * 法線情報
     */
    Vector3f normal;

    /**
     * UV情報
     */
    Vector2f uv;

    /**
     * 重さ情報
     */
    SimpleBoneWeight weight;
};

/**
 * FBXの頂点を一元管理する
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
