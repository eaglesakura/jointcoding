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
    virtual FigureInstanceState* createInstanceState() = 0;
};

    /**
     * 基本的な描画を行わせるフィギュアレンダラーの基礎
     */
class BasicFigureRendererBase: public FigureRenderer {

protected:

    /**
     * フラグメント自体の描画を行わせる
     */
    virtual void onRenderingFragment(const Figure *pFigure, const u32 nodeIndex, const u32 meshIndex, const MeshMaterial *pMaterial, const u32 fragmentIndex, const MeshFragment *pFragment) = 0;

public:
    BasicFigureRendererBase() {
    }

    virtual ~BasicFigureRendererBase() {
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
            const FigureNodeHandle node = pFigure->getNodeHandle(n);

            for (s32 g = 0; g < node->getMeshGroupNum(); ++g) {
                const MeshGroup *pGroup = node->getMeshGroup(g);
                const MeshMaterial *pMaterial = NULL;

                for (s32 f = 0; f < pGroup->getFragmentNum(); ++f) {
                    onRenderingFragment(pFigure, n, g, pMaterial, f, pGroup->getFragment(f));
                }
            }
        }
    }
};

}
}

#endif /* FIGURERENDERER_H_ */
