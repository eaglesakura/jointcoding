/*
 * BasicFigureRenderer.h
 *
 *  Created on: 2013/06/01
 */

#ifndef BASICFIGURERENDERER_H_
#define BASICFIGURERENDERER_H_

#include    "jc/framework/assets/Rendering3DAsset.hpp"

namespace jc {
namespace fw {

/**
 * シンプルな描画を行わせるシンプルなフィギュアレンダラー
 */
class BasicFigureRenderer: public FigureRenderer {

protected:
    /**
     * 頂点属性
     */
    VertexAttributeCombine attributes;

    /**
     * LP行列
     */
    Matrix4Uniform unif_lookprojection;

    /**
     * skinningを行わない場合のワールド行列
     */
    Matrix4Uniform unif_world;

    /**
     * 拡散反射光テクスチャ
     */
    TextureUniform unif_diffseTexture;

    /**
     * シャドウ用ライトの方向
     * メインライトとして利用する
     */
    Vector3fUniform unif_mainlightdirection;

    /**
     * WLP行列の逆行列を転置した行列
     * 法線計算に利用する
     */
    Matrix3Uniform unif_normalmatrix;

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
