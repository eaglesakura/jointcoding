/*
 * NodeFragment.hpp
 *
 *  Created on: 2013/05/21
 */

#ifndef NODEFRAGMENT_HPP_
#define NODEFRAGMENT_HPP_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * ノードの一部分を定義する
 * 3Dツール上の１マテリアルに対して１つのMeshGroupが割り当てられ、複数分割描画が必要な場合はFragmentに分割される。
 * MeshGroupは同一シェーダー・同一条件で描画されることを想定する。
 */
class MeshGroup: public Object {
    /**
     * Nodeの断片を構築するためのコンテキストグループ
     */
    safe_array<MeshFragment*> fragment;

protected:

    virtual MeshFragment* createMeshContext(MDevice device, const u32 index) {
        return new MeshFragment();
    }

public:
    MeshGroup() {
        safe_delete(fragment);
    }

    virtual ~MeshGroup() {
        safe_delete(fragment);
        jclogf("delete NodeFragment(0x%x)", this);
    }

    /**
     * フラグメントを取得する
     */
    virtual MeshFragment* getFragment(const s32 index) const {
        return fragment[index];
    }

    /**
     * コンテキスト数を取得する
     */
    virtual u32 getFragmentNum() const {
        return fragment.length;
    }

    /**
     * コンテキスト初期化を行う
     */
    virtual void initializeFragments(MDevice device, const u32 contexts) {
        fragment.alloc(contexts);
        for (u32 i = 0; i < contexts; ++i) {
            fragment[i] = createMeshContext(device, i);
            fragment[i]->initialize(device);
        }
    }
};

}
}

#endif /* NODEFRAGMENT_HPP_ */
