/*
 * NodeFragment.hpp
 *
 *  Created on: 2013/05/21
 */

#ifndef NODEFRAGMENT_HPP_
#define NODEFRAGMENT_HPP_

#include    "jc/framework/assets/figure/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * ノードの一部分を定義する
 * NodeFragmentに対して１つのマテリアルが割り当てられ、MeshContextは同一マテリアルとして描画される。
 */
class NodeFragment: public Object {
    /**
     * Nodeの断片を構築するためのコンテキストグループ
     */
    safe_array<MeshContext*> fragment;

protected:

    virtual MeshContext* createMeshContext(MDevice device, const u32 index) {
        return new MeshContext();
    }

public:
    NodeFragment() {
        safe_delete(fragment);
    }

    virtual ~NodeFragment() {
        safe_delete(fragment);
        jclogf("delete NodeFragment(0x%x)", this);
    }

    /**
     * レンダリングコンテキストを取得する
     */
    virtual MeshContext* getContext(const s32 index) const {
        return fragment[index];
    }

    /**
     * コンテキスト数を取得する
     */
    virtual u32 getContextNum() const {
        return fragment.length;
    }

    /**
     * コンテキスト初期化を行う
     */
    virtual void initializeContexts(MDevice device, const u32 contexts) {
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
