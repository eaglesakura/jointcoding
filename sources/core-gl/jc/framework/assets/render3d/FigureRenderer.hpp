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
protected:
    /**
     * レンダリングパス
     */
    s32 renderPass;
public:
    FigureRenderer() {
        renderPass = 0;
    }

    virtual ~FigureRenderer() {
    }

    /**
     * レンダリングパスを指定する
     */
    virtual void setRenderingPass(const s32 pass) {
        renderPass = pass;
    }

    /**
     * 現在のレンダリングパスを取得する
     */
    virtual s32 getRenderingPass() const {
        return renderPass;
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

}
}

#endif /* FIGURERENDERER_H_ */
