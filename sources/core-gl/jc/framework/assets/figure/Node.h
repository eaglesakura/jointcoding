/*
 * FigureNode.h
 *
 *  Created on: 2013/05/20
 */

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

class FigureNode;

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
    safe_array<FigureNode*> children;

    /**
     * マテリアルごとのグループ情報
     * １マテリアルにつき1グループ（複数フラグメント）を描画する
     */
    safe_array<MeshGroup*> groups;
protected:
    /**
     * ノード名
     */
    String name;

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
    virtual MeshGroup* createGroup(const s32 material) {
        return new MeshGroup();
    }

    /**
     * 自身のノード番号
     */
    s32 number;
public:
    FigureNode(const s32 nodeNumber) {
        number = nodeNumber;
    }

    virtual ~FigureNode() {
        safe_delete(groups);
        jclogf("delete FigureNode[%d] (0x%x) (%s)", number, this, name.c_str());
    }

    /**
     * ノード名を取得する
     */
    virtual const String& getName() const {
        return name;
    }

    /**
     * ノード名を設定する
     */
    virtual void setName(const String &set) {
        this->name = set;
    }

    /**
     * 初期化を行う
     *
     * @param selfNodeNumber thisの指すノード番号
     * @param materials このノードで描画すべきマテリアル数
     *                  0の場合、ノードは描画ではなくアンカー等の用途として使われることを想定する。
     */
    virtual void initialize(MDevice device, const u32 materials) {
        if (!materials) {
            return;
        }

        // マテリアル数だけ確保を行う
        groups.alloc(materials);

        for (int i = 0; i < materials; ++i) {
            groups[i] = createGroup(i);
        }
    }

    /**
     * マテリアルごとのメッシュ断片を取得する
     */
    virtual MeshGroup* getMeshGroup(const s32 index) const {
        return groups[index];
    }

    /**
     * フラグメント数を取得する
     */
    virtual u32 getMeshGroupNum() const {
        return groups.length;
    }

    /**
     * デフォルトの姿勢行列を取得する
     */
    inline const Matrix4x4* getDefaultTransform() const {
        return &def_transform;
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
     * 子の数はモデリング時に固定されるため、一度だけ設定を行えば問題ない
     */
    void setChildrenNum(FigureNode* const *ppChildren, const s32 num) {
        children.reserve(num);
        children.memcopyFrom(ppChildren, num);
    }

    /**
     * 子ノードをテーブルからピックアップする
     */
    FigureNode* getChildNode(const s32 index) const {
        return children[index];
    }
};

}
}

