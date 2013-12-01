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
    }attribute;

    /**
     * レンダリング用シェーダー
     */
    MGLShaderProgram shader;
public:
    SpriteBatch(MDevice device);

    virtual ~SpriteBatch();

    /**
     * レンダリング用シェーダーを設定する
     */
    virtual void setShader(MGLState state, MGLShaderProgram shader);

    /**
     * 描画の開始を行う
     */
    virtual void begin(MGLState state);

    /**
     * 四角形を描画する
     */
    virtual void nextRect(const float x, const float y, const float w, const float h, const Color color);

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
