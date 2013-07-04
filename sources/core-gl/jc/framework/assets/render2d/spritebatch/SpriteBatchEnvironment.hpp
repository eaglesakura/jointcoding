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
struct SpriteBatchVertex {
    /**
     * 位置情報
     */
    Vector2f position;

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
     * レンダリング対象のテクスチャユニット
     * 負の値の場合、通常のテクスチャを適用せずに描画する
     */
    s32 texture_index;

    /**
     * 回転角情報
     */
    float rotate;
};

/**
 * 位置情報属性
 */
typedef VertexAttribute<SpriteBatchVertex, 2, GL_FLOAT, GL_FALSE, 0> SpriteBatchPositionAttribute;

/**
 * UV情報属性
 */
typedef VertexAttribute<SpriteBatchVertex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f)> SpriteBatchCoordAttribute;

/**
 * カラー情報属性
 */
typedef VertexAttribute<SpriteBatchVertex, 2, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vector2f) + sizeof(Vector2f)> SpriteBatchColorAttribute;

/**
 * テクスチャユニット属性
 */
typedef VertexAttribute<SpriteBatchVertex, 1, GL_INT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector2f) + sizeof(Color)> SpriteBatchTextureUnitAttribute;

/**
 * 回転属性
 */
typedef VertexAttribute<SpriteBatchVertex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f) + sizeof(Vector2f) + sizeof(Color) + sizeof(int)> SpriteBatchRotateAttribute;

/**
 * スプライトレンダリング時の環境情報を設定する
 *
 */
class SpriteBatchEnvironmentState: public Object {
    /**
     * ブレンディング情報
     */
    struct _ {
        GLenum sfactor;
        GLenum dfactor;
    } blend;

    /**
     * レンダリング用テクスチャ
     */
    std::vector<MTextureImage> textures;

    /**
     * レンダリング用シェーダー
     */
    MGLShader shader;

    struct {
        /**
         * 位置情報
         *
         * aPosition
         */
        SpriteBatchPositionAttribute position;

        /**
         * UV情報
         *
         * aCoord
         */
        SpriteBatchCoordAttribute uv;

        /**
         * color
         *
         * aColor
         */
        SpriteBatchColorAttribute color;

        /**
         * 回転角
         *
         * aRotate
         */
        SpriteBatchColorAttribute rotate;

        /**
         * テクスチャ配列
         *
         * aTextureIndex
         */
        SpriteBatchTextureUnitAttribute texture_index;
    } attribute;

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
        blend.sfactor = GL_SRC_ALPHA;
        blend.dfactor = GL_ONE_MINUS_SRC_ALPHA;
    }

    virtual ~SpriteBatchEnvironmentState() {
    }

    /**
     * シェーダーを設定する
     */
    virtual void setShader(MGLShaderProgram shader) {
        attribute.position.setLocation(shader, "aPosition");
        attribute.uv.setLocation(shader, "aCoord");
        attribute.color.setLocation(shader, "aColor");
        attribute.rotate.setLocation(shader, "aRotate");
        attribute.texture_index.setLocation(shader, "aTextureIndex");
    }

    /**
     * ブレンディング関数を設定する
     */
    virtual void setBlend(const GLenum sfactor, const GLenum dfactor) {
        blend.sfactor = sfactor;
        blend.dfactor = dfactor;
    }

    /**
     * ブレンディング関数を設定する
     */
    virtual void setBlend(const GLBlendType_e type) {
        GLState::getBlendFunc(type, &blend.sfactor, &blend.dfactor);
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
     * 属性情報を関連付ける。
     * 既に頂点バッファはバインド済みである必要がある
     */
    virtual void attachAttributes(MGLState state) {
        attribute.position.attributePointer(state);
        attribute.uv.attributePointer(state);
        attribute.color.attributePointer(state);
        attribute.texture_index.attributePointer(state);
        attribute.rotate.attributePointer(state);
    }

    /**
     * シェーダー情報をアップロードする
     */
    virtual void uploadUniforms(MGLState state) {
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
};

/**
 * managed
 */
typedef jc_sp<SpriteBatchEnvironmentState> MSpriteBatchEnvironmentState;

}
}

#endif /* SPRITEBATCH_HPP_ */
