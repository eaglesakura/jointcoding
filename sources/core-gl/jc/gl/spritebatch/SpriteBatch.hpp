/*
 * SpriteBatch.hpp
 *
 *  Created on: 2013/07/01
 */

#ifndef SPRITEBATCH_HPP_
#define SPRITEBATCH_HPP_

#include    "jc/gl/shader/ShaderProgram.h"
#include    "jc/gl/texture/TextureImage.h"

namespace jc {

namespace gl {

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
    Vector2f uv;

    /**
     * 頂点カラー
     *
     * α等を処理する
     */
    Color color;

    /**
     * レンダリング対象のテクスチャユニット
     */
    s32 texture_unit;

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
    std::vector<TextureImage> textures;

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
         * aTextureUnit
         */
        SpriteBatchTextureUnitAttribute texture_unit;
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
        blend.sfactor = GL_ONE;
        blend.dfactor = GL_ZERO;
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
        attribute.textures.setLocation(shader, "aTextureUnit");
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
     * テクスチャを環境へ追加する
     */
    jcboolean addTexture(const MTextureImage texture, s32 *result_index) {
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
        attribute.texture_unit.attributePointer(state);
        attribute.rotate.attributePointer(state);
    }

    /**
     * シェーダー情報をアップロードする
     */
    virtual void uploadUniforms(MGLState state) {
    }
};

/**
 * managed
 */
typedef jc_sp<SpriteBatchEnvironmentState> MSpriteBatchEnvironmentState;

}
}

#endif /* SPRITEBATCH_HPP_ */
