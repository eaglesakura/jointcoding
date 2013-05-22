/*
 * FigureNode.h
 *
 *  Created on: 2013/05/20
 */

#include    "jc/framework/assets/figure/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * １フィギュアを構築するノード情報
 *
 * １ノードは最大１メッシュを構築できる
 *   * 1メッシュは複数のマテリアルを含むことが出来る
 *    * 1マテリアルは複数のコンテキストに分割され、このコンテキストが１描画単位となる
 *
 * FigureNode[nodes]として生成し、配列で管理する
 * ノードはそれぞれハンドルで繋がれており、適宜子にアクセスして利用する
 */
class FigureNode {
    /**
     * 子ノード配列
     * 実体のメモリ管理（ptr delete）は別箇所で行う。
     */
    safe_array<handle_data> children;

    /**
     * マテリアルごとのフラグメント情報
     * １マテリアルにつき1フラグメントを描画する
     */
    safe_array<NodeFragment*> fragments;
protected:
    /**
     * デフォルト行列
     */
    Matrix4x4 def_transform;

    /**
     * デフォルト行列の逆行列
     */
    Matrix4x4 def_transform_inv;

    /**
     * フラグメントを生成する
     */
    virtual NodeFragment* createFragment(const s32 material) {
        return new NodeFragment();
    }

    /**
     * 自身のノード番号
     */
    s32 number;
public:
    FigureNode() {
        number = -1;
    }

    virtual ~FigureNode() {
        safe_delete(fragments);
    }

    /**
     * 初期化を行う
     *
     * @param selfNodeNumber thisの指すノード番号
     * @param materials このノードで描画すべきマテリアル数
     *                  0の場合、ノードは描画ではなくアンカー等の用途として使われることを想定する。
     */
    virtual void initialize(MDevice device, const s32 selfNodeNumber, const u32 materials) {
        assert(selfNodeNumber >= 0);
        this->number = selfNodeNumber;


        if (!materials) {
            return;
        }

        // マテリアル数だけ確保を行う
        fragments.alloc(materials);

        for (u32 i = 0; i < materials; ++i) {
            fragments[i] = createFragment(i);
        }
    }

    /**
     * マテリアルごとのメッシュ断片を取得する
     */
    virtual NodeFragment* getFragment(const s32 index) {
        return fragments[index];
    }

    /**
     * デフォルトの姿勢行列を取得する
     */
    inline Matrix4x4* getDefaultTransform() {
        return &def_transform;
    }

    /**
     * デフォルト姿勢行列の逆行列を取得する
     */
    inline void calcInvertTransform() {
        def_transform.invert(&def_transform_inv);
    }

    /**
     * 子の数を指定する。
     * 子の数はモデリング時に固定されるため、一度だけ設定可能
     *
     * children_indicesの内容は内部的にコピーされ、ポインタは内部で残さない。
     * children_handles[0 - (num-1)]は既にretain済みであることを想定する。
     */
    void setChildrenNum(const handle_data *children_handles, const s32 num) {
        children.alloc(num);
        children.copyFrom(children_handles, num);
    }

    /**
     * 子ノードをテーブルからピックアップする
     */
    FigureNode* getChildNode(const handle_table<FigureNode*> &table, const s32 index) const {
        return table.get(children[index]);
    }
};

}
}

