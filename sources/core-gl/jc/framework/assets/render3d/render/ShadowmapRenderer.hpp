/*
 * ShadowmapRenderer.hpp
 *
 *  Created on: 2013/06/11
 */

#ifndef SHADOWMAPRENDERER_HPP_
#define SHADOWMAPRENDERER_HPP_

#include    "jc/framework/assets/Rendering3DAsset.hpp"

namespace jc {
namespace fw {

/**
 * シャドウマップのレンダリングを行わせる
 */
class ShadowmapRenderer: public BasicFigureRenderer {
public:
    ShadowmapRenderer() {
    }

    virtual ~ShadowmapRenderer() {
    }

protected:
    /**
     * マテリアルを関連付ける
     */
    virtual void bindMaterial(MDevice device, const Figure *pFigure, const MeshResource *pResource, const MeshMaterial *pMaterial, FigureInstanceState *pInstance) {
        MGLState state = device->getState();
        // シェーダーのバインドを行う
        {
            shader->bind();

            // 深度バッファをONに切り替える
            state->depthTestEnable(jctrue);
        }

        // 頂点属性の関連付けを行う
        {
            attributes.attribute(state);
        }

        // ライトマップ設定
        {
            EnvironmentInstanceState *env = pInstance->getEnvironmentState();
            // シャドウマップには環境設定が必要になる
            assert(env);

            // 行列を転送
            unif_world.upload(pInstance->getModelview());
            unif_lookprojection.upload(env->getShadowmapLight()->getShadowCamera()->getLookProjectionMatrix());
        }

    }

};

}
}

#endif /* SHADOWMAPRENDERER_HPP_ */
