/*
 * SpriteBatchGroup.hpp
 *
 *  Created on: 2013/07/04
 */

#ifndef SPRITEBATCHGROUP_HPP_
#define SPRITEBATCHGROUP_HPP_

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {
namespace fw {

/**
 * レンダリングデータ
 */
struct PolygonRequest {
    /**
     * 位置情報
     *
     * TriangleStrip情報
     */
    struct VertexInfo {
        Vector3f pos;
        Vector2f coord;
        Color color;
    };

    /**
     * 頂点情報配列
     */
    VertexInfo *vertex_info;

    /**
     * 頂点の長さ
     */
    s32 vertex_info_length;

    /**
     * テクスチャ
     * NULLの場合白紙を描画する
     */
    MTextureImage texture;

    /**
     * 回転角情報
     */
    float rotate;

    /**
     * ブレンディングタイプ指定
     */
    GLBlendType_e blend;

    PolygonRequest() {
        blend = GLBlendType_Alpha;
        rotate = 0;
        vertex_info = NULL;
        vertex_info_length = 0;
    }
};

/**
 * テンプレートを利用して適当な長さのリクエストを行えるようにする
 */
template<u32 NUM_VERTEX>
struct TSpriteRequest {
    PolygonRequest req;

    PolygonRequest::VertexInfo info[NUM_VERTEX];

    TSpriteRequest() {
        req.vertex_info = info;
        req.vertex_info_length = NUM_VERTEX;
    }
};

/**
 * 三角形のリクエスト
 */
typedef TSpriteRequest<3> TriangleSpriteRequest;

/**
 * 四角形のリクエスト
 */
typedef TSpriteRequest<4> QuadSpriteRequest;

/**
 * スプライトバッチはひとつの巨大なデータとして管理し、必要な部位ごとに部分描画を行うことで対応する
 * 一度CPUメモリ上で巨大な配列を作成し、GPUメモリ上でメッシュ描画を行う
 */
struct SpriteBatchSource {
    /**
     * 頂点キャッシュ
     */
    std::vector<PrimitiveBatchVertex> vertices;

    /**
     * インデックスバッファキャッシュ
     */
    std::vector<GLushort> indices;
};

/**
 * スプライトバッチ描画用の１つのレンダリンググループを形成する
 */
class PolygonBatchGroup: public Object {
    struct {
        s32 start;
        s32 num;
    } indices_range;

public:
    PolygonBatchGroup() {
        indices_range.start = -1;
        indices_range.num = -1;
    }

    virtual ~PolygonBatchGroup() {
    }

    /**
     * リクエストを開始する
     */
    virtual void beginRequest(SpriteBatchSource *mesh);

    /**
     * レンダリングリクエストを送る
     * リクエストが受諾された場合trueを返す
     */
    virtual jcboolean requestRendering(SpriteBatchEnvironmentState *env, SpriteBatchSource *mesh, const PolygonRequest *sprite);

    /**
     * レンダリングを行う範囲を取得する
     */
    virtual jcboolean getRenderingRange(s32 *result_indices_start, s32 *result_indices_num) {
        assert(result_indices_start);
        assert(result_indices_num);

        // レンダリング可能なデータが存在しない
        if (indices_range.num <= 0) {
            return jcfalse;
        }

        return jctrue;
    }
};

typedef jc_sp<PolygonBatchGroup> MSpriteBatchGroup;

}
}

#endif /* SPRITEBATCHGROUP_HPP_ */
