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
#include    <map>

namespace jc {
namespace fbx {

class MeshFragment;
typedef jc_sp<MeshFragment> MMeshFragment;

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

    /**
     * 既にボーンを使用済みか
     */
    jcboolean isUsingBone(u8 index);

    /**
     * 利用できるボーン数をオーバーする場合true
     */
    jcboolean isOverBone(const u8 *bones, const s32 bones_length);

    /**
     * １つのフラグメントが利用できる最大ボーン数
     */
    s32 maxBones;

    /**
     * 頂点インデックス
     */
    std::vector<u16> indices;

    /**
     * 使うボーン番号
     */
    std::map<u8, u8> useBoneIndices;
protected:

    /**
     * 現在のデータから分離を行い、新たなフラグメントとして子を作成する。
     */
    void separation();
public:
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

    /**
     * 利用するボーン数を取得する
     */
    virtual s32 getBoneCount() const {
        return useBoneIndices.size();
    }
};

/**
 * managed
 */
typedef jc_sp<MeshFragment> MMeshFragment;

}
}

#endif /* MESHGROUP_H_ */
