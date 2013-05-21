/*
 * FigureNode.h
 *
 *  Created on: 2013/05/20
 */

#include    "jc/framework/assets/figure/FigureAsset.hpp"

namespace jc {
namespace gl {

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
     * 子のノード番号を示した配列
     */
    safe_array<handle_data> handles;

protected:
    /**
     * デフォルト行列
     */
    Matrix4x4 def_transform;

    /**
     * デフォルト行列の逆行列
     */
    Matrix4x4 def_transform_inv;

public:
    FigureNode() {
    }

    virtual ~FigureNode() {
    }

    /**
     * 子の数を指定する。
     * 子の数はモデリング時に固定されるため、一度だけ設定可能
     *
     * children_indicesの内容は内部的にコピーされ、ポインタは内部で残さない。
     */
    void setChildrens(const handle_data *children_handles, const s32 num) {
        handles.alloc(num);
        handles.memcpy(children_handles, num);
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
     * 子のノードアクセスハンドルを取得する
     */
    const handle_data& getChildNodeHandle(const s32 index) const {
        return handles[index];
    }
};

}
}

