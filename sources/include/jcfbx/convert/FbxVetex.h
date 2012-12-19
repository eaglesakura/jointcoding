/*
 * Vertex.h
 *
 *  Created on: 2012/12/18
 */

#ifndef JCFBXVERTEX_H_
#define JCFBXVERTEX_H_

#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"
#include    "jcfbx/attribute/BoneWeight.h"
#include    <vector>

namespace jc {
namespace fbx {

template<s32 WEIGHT_NUM>
class _FbxVertex {
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
    bool equals(const _FbxVertex<WEIGHT_NUM> &v1) const {
        if (position != v1.position) {
            return false;
        }

        if (normal != v1.normal) {
            return false;
        }

        if (uv != v1.uv) {
            return false;
        }

        for (s32 i = 0; i < WEIGHT_NUM; ++i) {
            if (weight.indices[i] != v1.weight.indices[i]) {
                return false;
            }
            if (weight.weights[i] != v1.weight.weights[i]) {
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
typedef _FbxVertex<4> FigureVertex;

/**
 * 頂点を登録し、インデックスを返す
 * 同一頂点が存在する場合はそのインデックスを返す。
 * 同一頂点が存在しない場合は末尾に登録され、最後のindexが変える。
 */
inline s32 registerVertex(std::vector<FigureVertex> *result, const FigureVertex &vertex) {
    for (u32 i = 0; i < result->size(); ++i) {
        if (vertex.equals((*result)[i])) {
            //! 頂点が一致した
            //            jclogf("    vertex cached(%d)", i);
            return i;
        }
    }

    result->push_back(vertex);
    //    jclogf("    vertex created(%d)", vertices.size() - 1);
    return (s32) (result->size() - 1);
}


}
}

#endif /* JCFBXVERTEX_H_ */
