/*
 * BasicFigureRenderer.h
 *
 *  Created on: 2013/06/01
 */

#ifndef BASICFIGURERENDERER_H_
#define BASICFIGURERENDERER_H_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * 位置
 */
typedef VertexAttribute<BasicVertex, 3, GL_FLOAT, GL_FALSE, 0> BasicPositionAttribute;

/**
 * UV
 */
typedef VertexAttribute<BasicVertex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3f)> BasicCoordAttribute;

/**
 * 法線
 */
typedef VertexAttribute<BasicVertex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f)> BasicNormalAttribute;

/**
 * シンプルな描画を行わせるシンプルなフィギュアレンダラー
 */
class BasicFigureRenderer: public FigureRenderer {

protected:

    /**
     * 位置属性
     */
    BasicPositionAttribute attr_position;

    /**
     * UV
     */
    BasicCoordAttribute attr_coord;

    /**
     * 法線
     */
    BasicNormalAttribute attr_normal;

    /**
     * WLP行列
     */
    MatrixUniform unif_worldlookprojection;

    /**
     * レンダリング用シェーダー
     */
    MGLShaderProgram shader;

    /**
     * マテリアルを関連付ける
     */
    virtual void bindMaterial(MDevice device, const Figure *pFigure, const MeshResource *pResource, const MeshMaterial *pMaterial, FigureInstanceState *pInstance);

    /**
     * フラグメント自体の描画を行わせる
     * 描画の必要が無ければ、描画を行わないこと。
     */
    virtual void renderingFragment(MDevice device, const Figure *pFigure, const MeshResource *pResource, const MeshMaterial *pMaterial, const FigureNode *pNode, const MeshGroup *pGroup, const MeshFragment *pFragment, FigureInstanceState *pInstance);
public:
    BasicFigureRenderer();
    virtual ~BasicFigureRenderer();

    /**
     * 初期化を行わせる
     */
    virtual void initialize(MDevice device, MGLShaderProgram shader);

    /**
     * レンダリング処理は継承先のメソッドに任せる
     */
    virtual void rendering(MDevice device, const jc_selp<Figure> figure, const jc_selp<FigureInstanceState> instance);

    /**
     * レンダリング用インスタンスパラメータを生成する
     * レンダラーとはセットで運用され、このレンダラーから生成されたインスタンス以外の描画を保証しない。
     */
    virtual FigureInstanceState* createInstanceState(const jc_selp<Figure> figure) {
        FigureInstanceState *result = new FigureInstanceState();
        result->initialize(figure);
        return result;
    }
};

}
}

#endif /* BASICFIGURERENDERER_H_ */
