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

    /**
     * 対象のフィギュア情報
     */
    jc_selp<Figure> figure;
public:
    /**
     *
     */
    FigureInstanceState() {
    }

    virtual ~FigureInstanceState() {
    }

    /**
     * 初期化を行う
     */
    virtual void initialize(const jc_selp<Figure> figure) {
        this->figure = figure;
        wlpMatrices.reserve(figure->getNodeNum());
    }

    /**
     * このインスタンスのWLP行列を取得する
     */
    const Matrix4x4& getWorldLookProjection() const {
        return wlp;
    }

    /**
     * このインスタンスのWLP行列を取得する
     */
    Matrix4x4& getWorldLookProjection() {
        return wlp;
    }

    /**
     * ノードごと全行列のポインタを取得する
     */
    Matrix4x4* getNodeMatrices() {
        return wlpMatrices.ptr;
    }

    /**
     * ノード番号を指定してポインタを取得する
     */
    Matrix4x4& getNodeMatrix(const u32 node_nubmer) {
        return wlpMatrices[node_nubmer];
    }
};

}
}

#endif /* FIGUREINSTANCESTATE_HPP_ */
