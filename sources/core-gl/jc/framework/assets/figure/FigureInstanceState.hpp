/*
 * FigureInstanceState.hpp
 *
 *  Created on: 2013/05/28
 */

#ifndef FIGUREINSTANCESTATE_HPP_
#define FIGUREINSTANCESTATE_HPP_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * フィギュア個々のフラグを管理する
 */
enum FigureInstanceFlag_e {
    /**
     * アルファを持つことを明示する
     */
    FigureInstanceFlag_HasAlpha,

    /**
     * フラグ数管理
     */
    FigureInstanceFlag_Num,
};

/**
 * 1Figureの描画用データを保持する
 * 各Figureごとに一つのStateを保持し、Figure自体がインスタンス情報を持つことを避ける。
 */
class FigureInstanceState: public Object {

    /**
     * 全ノードのWLPブレンド済み行列
     */
    safe_array<Matrix4x4> wlpMatrices;

    /**
     * フィギュア自体のWLP行列
     */
    Matrix4x4 wlp;

    /**
     * デフォルトで管理されるフラグ
     */
    BitFlags<FigureInstanceFlag_Num> flags;
public:

    /**
     *
     */
    FigureInstanceState(const jc_selp<Figure> figure) {
        wlpMatrices.reserve(figure->getNodeNum());
    }

    virtual ~FigureInstanceState() {
    }

    const Matrix4x4& getRootMatrix() const {
        return wlp;
    }

    /**
     * 全行列のポインタを取得する
     */
    Matrix4x4* getMatrices() {
        return wlpMatrices.ptr;
    }

    /**
     * ノード番号を指定してポインタを取得する
     */
    Matrix4x4& getMatrix(const u32 node_nubmer) {
        return wlpMatrices[node_nubmer];
    }
};

}
}

#endif /* FIGUREINSTANCESTATE_HPP_ */
