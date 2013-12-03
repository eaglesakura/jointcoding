/*
 * SpriteBatch.h
 *
 *  Created on: 2013/12/01
 */
#ifndef SPRITEBATCH_H_
#define SPRITEBATCH_H_

#include    "jc/framework/assets/GLAsset.hpp"
#include    "jc/framework/assets/ImageAsset.hpp"

namespace jc {
namespace gl {

struct BatchVertex {
    /**
     * ベースになる位置
     */
    Vector2f pos;

    /**
     * UV
     */
    Vector2f coord;

    /**
     * 移動量
     */
    Vector2f move;

    /**
     * 回転
     */
    float rotate;

    /**
     * 頂点カラー
     */
    Color color;

    /**
     * テクスチャインデックス
     */
    float textureIndex;

    /**
     * 色ブレンディング用データ
     */
//    float colorBlend;
};

/**
 * スプライトレンダリングをバッチ化する。
 *
 * 同一テクスチャで描画することを前提に設計されている
 * ポリゴン裏表判定は処理の簡略化のため行わない
 */
class SpriteBatch: public Object {

public:
    typedef typename std::vector<BatchVertex, StlAllocator<BatchVertex> > vertex_container;
    typedef typename std::vector<u16, StlAllocator<u16> > indices_container;
private:
    enum {
        /**
         * バッチバッファ
         */
        BATCHBUFFER_NUM = 2,
    };

    struct VertexStream {
        /**
         * CPU管理下の頂点配列
         */
        vertex_container cpuVertices;

        /**
         * CPU管理下のインデックス配列
         */
        indices_container cpuIndices;

        /**
         * GPU管理下の頂点配列
         */
        jc_sp<VertexBufferObject<BatchVertex> > gpuVertices;

        /**
         * GPU管理下の頂点配列
         */
        MIndexBufferObject gpuIndices;
    };

    /**
     * コミット対象のストリーム
     */
    s32 commitStreamIndex;

    /**
     * レンダリング対象のストリーム
     */
    s32 renderingStreamIndex;

    /**
     * レンダリング対象のサーフェイスサイズ
     */
    Vector2f surfaceSize;

    /**
     * バッファリング用頂点ストリーム
     */
    VertexStream vertices[BATCHBUFFER_NUM];

    /**
     * 現在操作対象のバッファを取得する
     */
    inline VertexStream& getCurrentVertices() {
        return vertices[commitStreamIndex];
    }

    /**
     * 現在操作対象のバッファを取得する
     */
    inline const VertexStream& getCurrentVertices() const {
        return vertices[commitStreamIndex];
    }

    struct {
        VertexAttribute<BatchVertex, 2, GL_FLOAT, GL_FALSE, 0> position;
        VertexAttribute<BatchVertex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2> coord;
        VertexAttribute<BatchVertex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4> move;
        VertexAttribute<BatchVertex, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 6> rotate;
        VertexAttribute<BatchVertex, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(float) * 7> color;
        VertexAttribute<BatchVertex, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7 + sizeof(Color)> textureIndex;
    }attribute;

    struct {
        /**
         * テクスチャUniform
         */
        TextureArrayUniform textures;
    }uniform;

    /**
     * レンダリング用シェーダー
     */
    MGLShaderProgram shader;

    struct {
        /**
         * 描画対象のテクスチャリスト
         */
        texture_array textures;

        /**
         * 現在設定対象のテクスチャ
         */
        s32 textureIndex;

        /**
         * 白テクスチャ
         */
        MTextureImage white;
    }context;

    /**
     * レンダリング情報
     */
    MRenderingContext renderingContext;
public:
    SpriteBatch(MRenderingContext context, MDevice device);

    virtual ~SpriteBatch();

    /**
     * レンダリング用シェーダーを設定する
     */
    virtual void setShader(MGLState state, MGLShaderProgram shader);

    /**
     * アタッチされているシェーダーを設定する
     */
    virtual MGLShaderProgram getShader() const {
        return shader;
    }

    /**
     * サーフェイス設定を変更する
     */
    virtual void setSurfaceAspect(const float width, const float height) {
        surfaceSize.x = width;
        surfaceSize.y = height;
    }

    /**
     * サーフェイス設定を変更する
     */
    virtual void setSurfaceAspect(const Vector2f &size) {
        surfaceSize = size;
    }

    /**
     * 描画の開始を行う
     */
    virtual void begin(MGLState state);

    /**
     * 四角形を描画する
     */
    virtual void nextRect(const float x, const float y, const float w, const float h, const Color color);

    /**
     * テクスチャ描画を開始する
     * テクスチャが能力限界を超えて描画開始に失敗したらfalseを返す
     */
    virtual jcboolean beginTexture(MTextureImage texture);

    /**
     * 四角形を描画する
     */
    virtual void nextRect(const float imgX, const float imgY, const float imgW, const float imgH, const float x, const float y, const float w, const float h, const float rotate, const Color color);

    /**
     * テクスチャ描画を終了する
     */
    virtual void endTexture();

    /**
     * ストリームをGPUへ書き込む
     */
    virtual void commit(MGLState state);

    /**
     * 最後にコミットしたストリームを描画する
     */
    virtual void rendering(MGLState state);
};

/**
 * managed
 */
typedef jc_sp<SpriteBatch> MSpriteBatch;

}
}

#endif /* SPRITEBATCH_H_ */
