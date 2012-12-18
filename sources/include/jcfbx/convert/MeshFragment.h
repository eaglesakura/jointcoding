/*
 * MeshFragment.h
 *
 *  Created on: 2012/12/18
 */

#ifndef JCFBXMESHFRAGMENT_H_
#define JCFBXMESHFRAGMENT_H_

#include    "jointcoding-fbx.h"
#include    "jcfbx/convert/FbxVetex.h"
#include    <vector>

namespace jc {
namespace fbx {

/**
 * メッシュを構築する一部分を管理する。
 * この配下にあるメッシュは全て同一マテリアルで描画する。
 * 使用するボーン数が規程を超えた場合、複数に分割する
 * Mesh
 *   L MeshFragment
 *      L MeshFragment
 *          L MeshFragment
 *          L MeshFragment
 *      L MeshFragment
 *      L MeshFragment
 *   L MeshFragment
 *   L MeshFragment
 */
class MeshFragment {
public:
    /**
     * 頂点インデックス
     */
    std::vector<u16> indices;

    /**
     *
     */
    MeshFragment();

    /**
     *
     */
    virtual ~MeshFragment();

    /**
     * インデックスバッファを追加する
     */
    virtual void addIndices(const std::vector<SimpleVertex> &vertices, const u16 p0, const u16 p1, const u16 p2);

    /**
     * ポリゴン数を数える。
     * 必ず三角ポリゴンで格納されるため、３で割るだけでいい
     */
    virtual s32 getPolygonCount() const {
        return indices.size() / 3;
    }
};

/**
 * managed
 */
typedef jc_sp<MeshFragment> MMeshFragment;

}
}

#endif /* MESHGROUP_H_ */
