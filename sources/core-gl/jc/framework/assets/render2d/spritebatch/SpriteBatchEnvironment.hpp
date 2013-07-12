/*
 * SpriteBatch.hpp
 *
 *  Created on: 2013/07/01
 */

#ifndef SPRITEBATCHENVIRONMENT_HPP_
#define SPRITEBATCHENVIRONMENT_HPP_

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {

namespace fw {

/**
 * バッチ処理用の頂点
 */
struct PrimitiveBatchVertex {
    /**
     * 位置情報
     */
    Vector3f position;

    /**
     * UV位置情報
     */
    Vector2f coord;

    /**
     * 頂点カラー
     *
     * α等を処理する
     */
    Color color;

    /**
     * 回転角情報
     */
    float rotate;

    /**
     * レンダリング対象のテクスチャユニット
     * 負の値の場合、通常のテクスチャを適用せずに描画する
     */
    s32 texture_index;
};

/**
 * スプライトレンダリング時の環境情報を設定する
 *
 */
class SpriteBatchEnvironmentState: public Object {
    /**
     * ブレンディング情報
     */
    GLBlendType_e blend;

    /**
     * レンダリング用テクスチャ
     */
    std::vector<MTextureImage> textures;

    /**
     * レンダリング用シェーダー
     */
    MGLShader shader;

    /**
     * 属性グループ
     */
    MVertexAttributeCombine attributes;

    struct {
        /**
         * 転送済みのテクスチャユニット
         *
         * uTextures
         */
        TextureUniform textures;
    } uniform;

public:
    SpriteBatchEnvironmentState() {
        blend = GLBlendType_Alpha;
        attributes.reset(new VertexAttributeCombine());
    }

    virtual ~SpriteBatchEnvironmentState() {
    }

    /**
     * シェーダーを設定する
     */
    virtual void setShader(MGLShaderProgram shader) {
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
        attributes->request(shader, requests, VertexAttributeRequest_length(requests));
    }

    /**
     * ブレンディング関数を設定する
     */
    virtual void setBlend(const GLBlendType_e type) {
        this->blend = type;
    }

    virtual GLBlendType_e getBlend() const {
        return blend;
    }

    /**
     * 管理しているテクスチャを解放する
     */
    virtual void clearTextures() {
        textures.clear();
    }

    /**
     * テクスチャを環境へ追加する
     */
    virtual jcboolean addTexture(const MTextureImage texture, s32 *result_index) {
        assert(texture);
        assert(result_index);

        // テクスチャが限界を超えていたら追加できない
        if (textures.size() >= GPUCapacity::getMaxTextureUnits()) {
            return jcfalse;
        }

        std::vector<MTextureImage>::iterator itr = textures.begin(), end = textures.end();
        int index = 0;
        while (itr != end) {

            // 既に登録済みなのでその値を返却する
            if ((*itr) == texture) {
                *result_index = index;
                return jctrue;
            }

            ++index;
            ++itr;
        }

        // 末尾に追加する
        textures.push_back(texture);
        *result_index = textures.size();

        return jctrue;
    }

    /**
     * レンダリング準備を行わせる
     */
    virtual void bind(MGLState state) {
        // ブレンド情報を設定する
        state->blendFunc(blend);

        // 属性情報を割り当てる
        attributes->attributePointer(state);

        // テクスチャ操作を行う
        {
            GLint units[32] = { 0 };
            // 順番通りにテクスチャをバインドする
            for (int i = 0; i < textures.size(); ++i) {
                textures[i]->bind(i, state);
                units[i] = i;
            }

            // シェーダーへアップロードする
            uniform.textures.uploadDirect(state, units, textures.size());
        }
    }

    /**
     * ステートの状態をoutStateへコピーする
     *
     * 但し、テクスチャ情報はコピーしない
     */
    virtual void clone(SpriteBatchEnvironmentState* outState) {
        assert(outState);

        outState->attributes = this->attributes;
        outState->blend = this->blend;
        outState->uniform = this->uniform;
        outState->shader = shader;
        {
            outState->uniform.textures.clearCache();
        }
    }
};

/**
 * managed
 */
typedef jc_sp<SpriteBatchEnvironmentState> MSpriteBatchEnvironmentState;

}
}

#endif /* SPRITEBATCH_HPP_ */
