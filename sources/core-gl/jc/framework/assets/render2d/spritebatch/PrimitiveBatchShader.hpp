/*
 * PrimitiveBatchShader.hpp
 *
 *  Created on: 2013/07/12
 */

#include    "jc/framework/assets/GLAsset.hpp"

namespace jc {
namespace fw {

/**
 * プリミティブのバッチレンダリング用シェーダー補助クラス
 */
class PrimitiveBatchShader: public Object {

    /**
     * コンパイル済みシェーダー本体
     */
    MGLShaderProgram shader;

    /**
     * 属性グループ
     */
    VertexAttributeCombine attributes;

    /**
     * GPUデータ
     */
    struct {
        /**
         * 転送済みのテクスチャユニット
         *
         * uTextures
         */
        TextureUniform textures;
    } uniform;

public:
    PrimitiveBatchShader() {

    }

    virtual ~PrimitiveBatchShader() {

    }

    /**
     * レンダリング用シェーダーを指定する
     */
    virtual void setShader(MGLShaderProgram shader) {
        this->shader = shader;

        // attributes
        {
            const VertexAttributeRequest requests[] = {
            // 位置
                    { "aPosition", VertexAttributeData_float3 },
                    // UV
                    { "aCoord", VertexAttributeData_float2 },
                    // 色
                    { "aColor", VertexAttributeData_ubyte4_normalized },
                    // 回転角
                    { "aRotate", VertexAttributeData_float1 },
                    // テクスチャユニット
                    { "aTextureIndex", VertexAttributeData_int1 }, };
            attributes.request(shader, requests, VertexAttributeRequest_length(requests));
        }

        // uniforms
        {
            uniform.textures.setLocation(shader, "uTextures");
        }
    }

    /**
     * テクスチャ情報をアップロードする
     */
    virtual void upload(MGLState state, MTextureImage *textures, const u32 textures_length) {
        assert(textures_length <= GPUCapacity::getMaxTextureUnits());

        // テクスチャ操作を行う
        GLint units[32] = { 0 };
        // 順番通りにテクスチャをバインドする
        for (int i = 0; i < textures_length; ++i) {
            textures[i]->bind(i, state);
            units[i] = i;
        }

        // シェーダーへアップロードする
        uniform.textures.uploadDirect(state, units, textures_length);
    }
};

typedef jc_sp<PrimitiveBatchShader> MPrimitiveBatchShader;

}
}

