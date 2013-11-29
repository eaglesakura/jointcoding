/*
 * FigureRenderer.h
 *
 *  Created on: 2013/01/18
 */

#ifndef FIGURERENDERER_H_
#define FIGURERENDERER_H_

#include    "jc/gl/figure/Camera.hpp"
#include    "jc/gl/shader/ShaderProgram.h"
#include    "jc/gl/figure/GLFigure.h"

namespace jc {
namespace gl {

/**
 * フィギュア描画用のレンダラー
 */
class FigureRenderer: public Object {
    /**
     * レンダリング用カメラ
     */
    MCamera camera;

    /**
     * レンダリング用シェーダー
     */
    MGLShaderProgram shader;

    /**
     * シェーダー用パラメータ
     */
    jc_sp<GLFigure::ShaderParams> params;

    /**
     * ワールド行列
     */
    s32 unif_world;

    /**
     * look & projection行列
     */
    s32 unif_look_prj;

public:
    FigureRenderer();

    virtual ~FigureRenderer();

    /**
     * 初期化を行う
     */
    virtual void initialize( MDevice device, const Uri &vertexShader, const Uri &fragmentShader );

    /**
     * シェーダーパラメーターを取得する
     */
    virtual jc_sp<GLFigure::ShaderParams> getShaderParams() const {
        return params;
    }

    /**
     * シェーダー用パラメータを設定する
     */
    virtual void setShaderParams(const jc_sp<GLFigure::ShaderParams> params) {
        this->params = params;
    }

    /**
     * シェーダーを取得する
     */
    virtual MGLShaderProgram getShader() const {
        return shader;
    }

    /**
     * シェーダーを設定する
     */
    virtual void setShader(const MGLShaderProgram shader) {
        this->shader = shader;
    }

    /**
     * カメラを取得する
     */
    virtual MCamera getCamera() const {
        return camera;
    }

    /**
     * カメラを設定する
     */
    virtual void setCamera(const MCamera camera) {
        this->camera = camera;
    }

    virtual s32 getWorldUniformLocation() const {
        return unif_world;
    }

    virtual s32 getLoolPrjUniformLocation() const {
        return unif_look_prj;
    }

    /**
     * 描画用のカメラをセットアップする
     */
    virtual void bindCamera(MGLState state);
};

/**
 * managed
 */
typedef jc_sp<FigureRenderer> MFigureRenderer;

}
}

#endif /* FIGURERENDERER_H_ */
