/*
 * Vertex.h
 *
 *  Created on: 2012/12/18
 */

#ifndef JCFBXVERTEX_H_
#define JCFBXVERTEX_H_

#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"
#include    "jc/graphics/figure/mesh/BoneWeight.h"
#include    <vector>

namespace jc {
namespace fbx {

template<s32 WEIGHT_NUM>
class _SimpleVertex {
public:
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
     * ボーンウェイト情報
     */
    _BoneWeight<WEIGHT_NUM> weight;

    /**
     * 同一頂点である場合はtrueを返す
     */
    bool equals(const _SimpleVertex<WEIGHT_NUM> &v1) const {
        if (!jc::equals(position, v1.position, 0.01f)) {
            return false;
        }

        if (!jc::equals(uv, v1.uv, 0.001f)) {
            return false;
        }

        if (!jc::equals(normal, v1.normal, 0.01f)) {
            return false;
        }

        for (s32 i = 0; i < WEIGHT_NUM; ++i) {
            if (weight.indices[i] != v1.weight.indices[i]) {
                return false;
            }

            if (!jc::equals(weight.weights[i], v1.weight.weights[i], 0.001f)) {
                return false;
            }
        }

        //! 全情報合致
        return true;
    }

};

/**
 * デフォルトのシンプルな頂点情報
 */
typedef _SimpleVertex<4> FigureVertex;

/**
 * 頂点を登録し、インデックスを返す
 * 同一頂点が存在する場合はそのインデックスを返す。
 * 同一頂点が存在しない場合は末尾に登録され、最後のindexが変える。
 */
inline s32 registerVertex(std::vector<FigureVertex> *result, const FigureVertex &vertex) {

    std::vector<FigureVertex>::iterator itr = result->begin(), end = result->end();
    int index = 0;
    while (itr != end) {
        if (vertex.equals(*itr)) {
            //! 頂点が一致した
//            jclogf("    vertex cached(%d)", index);
            return index;
        }
        ++index;
        ++itr;
    }
    result->push_back(vertex);
    //    jclogf("    vertex created(%d)", vertices.size() - 1);
    return (s32) (result->size() - 1);
}

}
}

#endif /* JCFBXVERTEX_H_ */
