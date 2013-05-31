/*
 * FigureRenderer.h
 *
 *  Created on: 2013/05/28
 */

#ifndef FIGURERENDERER_H_
#define FIGURERENDERER_H_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * Figureの実描画処理を行う
 */
class FigureRenderer: public Object {
public:
    FigureRenderer() {
    }

    virtual ~FigureRenderer() {
    }

    /**
     * レンダリング処理は継承先のメソッドに任せる
     */
    virtual void rendering(MDevice device, const jc_selp<Figure> figure, const jc_selp<FigureInstanceState> instance) = 0;

    /**
     * レンダリング用インスタンスパラメータを生成する
     */
    virtual FigureInstanceState* createInstanceState(const jc_selp<Figure> figure) = 0;
};

    /**
     * 基本的な描画を行わせるシンプルなフィギュアレンダラー
     */
class BasicFigureRenderer: public FigureRenderer {

protected:

    /**
     * 位置属性
     */
    Attribute attr_position;

    /**
     * UV
     */
    Attribute attr_coord;

    /**
     * 法線
     */
    Attribute attr_normal;

    /**
     * WLP行列
     */
    MatrixUniform unif_worldlookprojection;

    /**
     * レンダリング用シェーダー
     */
    MGLShaderProgram shader;

    /**
     * フラグメント自体の描画を行わせる
     */
    virtual void onRenderingFragment(const Figure *pFigure, const u32 nodeIndex, const u32 meshIndex, const MeshMaterial *pMaterial, const u32 fragmentIndex, const MeshFragment *pFragment) {

    }

public:
    BasicFigureRenderer() {
    }

    virtual ~BasicFigureRenderer() {
    }

    /**
     * レンダリング処理は継承先のメソッドに任せる
     */
    virtual void rendering(MDevice device, const jc_selp<Figure> figure, const jc_selp<FigureInstanceState> instance) {
        assert(figure);
        assert(instance);

        const Figure *pFigure = figure.get();
        const FigureInstanceState *pInstance = instance.get();

        for (s32 n = 0; n < figure->getNodeNum(); ++n) {
            const FigureNode *pNode = pFigure->getNode(n);

            for (s32 g = 0; g < pNode->getMeshGroupNum(); ++g) {
                const MeshGroup *pGroup = pNode->getMeshGroup(g);
                const MeshMaterial *pMaterial = NULL;

                for (s32 f = 0; f < pGroup->getFragmentNum(); ++f) {
                    onRenderingFragment(pFigure, n, g, pMaterial, f, pGroup->getFragment(f));
                }
            }
        }
    }

    /**
     * レンダリング用インスタンスパラメータを生成する
     * レンダラーとはセットで運用され、このレンダラーから生成されたインスタンス以外の描画を保証しない。
     */
    virtual FigureInstanceState* createInstanceState(const jc_selp<Figure> figure) {
        return new FigureInstanceState(figure);
    }
};

}
}

#endif /* FIGURERENDERER_H_ */
