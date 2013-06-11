/*
 * BasicFigureRenderer.cpp
 *
 *  Created on: 2013/06/01
 */

#include    "jc/framework/assets/Rendering3DAssets.hpp"

namespace jc {
namespace fw {

BasicFigureRenderer::BasicFigureRenderer() {
}

BasicFigureRenderer::~BasicFigureRenderer() {
}

/**
 * 初期化を行わせる
 */
void BasicFigureRenderer::initialize(MDevice device, MGLShaderProgram shader) {
    this->shader = shader;

    // attribute
    {
        attr_position.setLocation(shader, "attr_pos");
        // 最低限、位置情報は持っていなければならない
        assert(attr_position.valid());

        attr_coord.setLocation(shader, "attr_uv");
        attr_normal.setLocation(shader, "attr_normal");
    }

    // uniform
    {
        // world loop projection行列
        unif_worldlookprojection.setLocation(shader, "unif_wlp");

        // diffuseテクスチャ
        unif_diffseTexture.setLocation(shader, "unif_diffuse");

        // メインの陰影用ライト
        unif_mainlightdirection.setLocation(shader, "unif_mainlightdir");
        unif_normalmatrix.setLocation(shader, "unif_normalm");
    }
}

/**
 * マテリアルを関連付ける
 */
void BasicFigureRenderer::bindMaterial(MDevice device, const Figure *pFigure, const MeshResource *pResource, const MeshMaterial *pMaterial, FigureInstanceState *pInstance) {
    MGLState state = device->getState();

    // シェーダーのバインドを行う
    {
        shader->bind();

        // 深度バッファをONに切り替える
        state->depthTestEnable(jctrue);
    }

    // 頂点属性の関連付けを行う
    {
        attr_position.attributePointer(state);
        attr_coord.attributePointer(state);
        attr_normal.attributePointer(state);
    }

    // uniformのアップロードを行う
    {
        unif_worldlookprojection.upload(pInstance->getWorldLookProjection());

        EnvironmentInstanceState *env = pInstance->getEnvironmentState();
        // 環境ステータスが設定されている場合、環境情報をセットアップする
        if (env) {
            MLight light = env->getShadowmapLight();
            unif_mainlightdirection.upload(light->getDirection());

            Matrix3x3 m;
            copyMatrix(pInstance->getModelview(), &m)->invert();
            m.transpose();
            unif_normalmatrix.upload(m, GL_FALSE);
        }
    }

    // テクスチャのバインドを行う
    {
        MTextureImage diffuse = pMaterial->getDiffuse();
        if (diffuse) {
            unif_diffseTexture.upload(state, diffuse);
        }
    }
}

/**
 * フラグメント自体の描画を行わせる
 * 描画の必要が無ければ、描画を行わないこと。
 */
void BasicFigureRenderer::renderingFragment(MDevice device, const Figure *pFigure, const MeshResource *pResource, const MeshMaterial *pMaterial, const FigureNode *pNode, const MeshGroup *pGroup, const MeshFragment *pFragment, FigureInstanceState *pInstance) {
    // マテリアルが一致しない場合、描画を行わない。
    if (pGroup->getMaterial() != pMaterial) {
        return;
    }

    // インデックスバッファの描画範囲を取り出す
    s32 indices_start = 0;
    s32 indices_length = 0;
    pFragment->getIndicesRange(&indices_start, &indices_length);

    // レンダリングを行わせる
    pResource->getIndices()->rendering(GL_TRIANGLES, indices_start, indices_length);
}

/**
 * レンダリング処理は継承先のメソッドに任せる
 */
void BasicFigureRenderer::rendering(MDevice device, const jc_selp<Figure> figure, const jc_selp<FigureInstanceState> instance) {

    /**
     * 描画対象を取り出す
     */
    const Figure *pFigure = figure.get();
    FigureInstanceState *pInstance = instance.get();

    /**
     * 描画対象が存在しなければならない
     */
    assert(pFigure);
    assert(pInstance);

    /**
     * マテリアル数だけ繰り返す
     */
    MeshResource *pResource = pFigure->getResource().get();
    assert(pResource);

    const u32 materialNum = pResource->getMaterialNum();

    MGLState state = device->getState();
    /**
     * インデックスバッファと頂点バッファの関連付けを行う
     */
    pResource->getVertices()->bind(state);
    pResource->getIndices()->bind(state);

    /**
     * フィギュア全体で有効になるステートを切り替える
     */
    {
        if(pInstance->isEnableFlag(FigureInstanceFlag_NotCullface)) {
            state->cullFaceEnable(jcfalse);
        } else {
            state->cullFaceEnable(jctrue);
            state->frontface(GL_CCW);
        }
    }

    /**
     * 全メッシュ・マテリアルを巡回して描画を行わせる
     */
    for(u32 mat = 0; mat < materialNum; ++mat) {
        const MeshMaterial *pMaterial = pResource->getMaterial(mat);

        // マテリアルの関連付けを行う
        bindMaterial(device, pFigure, pResource, pMaterial, pInstance);

        // ノード数だけ描画を繰り返す
        for (s32 n = 0; n < figure->getNodeNum(); ++n) {
            const FigureNode *pNode = pFigure->getNode(n);
            for (s32 g = 0; g < pNode->getMeshGroupNum(); ++g) {
                const MeshGroup *pGroup = pNode->getMeshGroup(g);
                for (s32 f = 0; f < pGroup->getFragmentNum(); ++f) {
                    renderingFragment(device, pFigure, pResource, pMaterial, pNode, pGroup, pGroup->getFragment(f), pInstance);
                }
            }
        }
    }
}

}
}
