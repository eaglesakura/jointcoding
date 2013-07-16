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
        assert(shader);

        // attributes
        {
            const VertexAttributeRequest requests[] = {
            // 位置
                    { "aPosition", VertexAttributeData_float3, jctrue },
                    // UV
                    { "aCoord", VertexAttributeData_float2, jcfalse },
                    // 色
                    { "aColor", VertexAttributeData_ubyte4_normalized, jcfalse },
                    // 回転角
                    { "aRotate", VertexAttributeData_short1_normalized, jcfalse },
                    // テクスチャユニット
                    { "aTextureIndex", VertexAttributeData_short1_normalized, jcfalse }, };
            attributes.request(shader, requests, VertexAttributeRequest_length(requests));
        }

        // uniforms
        {
            uniform.textures.setLocation(shader, "uTextures");
        }
    }

    /**
     * シェーダーバインドを行う
     */
    virtual void bind(MGLState state) {
        assert(shader);
        shader->bind();
    }

    /**
     * シェーダーの解除を行う
     */
    virtual void unbind(MGLState state) {
        assert(shader);
        shader->unbind();
    }

    /**
     * 属性情報を更新する
     */
    virtual void attributePointer(MGLState state) {
        attributes.attributePointer(state);
    }

    /**
     * テクスチャ情報をアップロードする
     */
    virtual void upload(MGLState state, MTextureImage *textures, const u32 textures_length) {
        attributes.attributePointer(state);

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

