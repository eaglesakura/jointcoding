/*
 * SpriteBatchGroup.cpp
 *
 *  Created on: 2013/07/04
 */

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {
namespace fw {

/**
 * レンダリングリクエストを送る
 * リクエストが受諾された場合trueを返す
 */
jcboolean PolygonBatchGroup::requestRendering(SpriteBatchEnvironmentState *env, SpriteBatchSource *mesh, const PolygonRequest *sprite) {
    assert(env);
    assert(mesh);
    assert(sprite);

    // ブレンドタイプをチェック
    if (sprite->blend != env->getBlend()) {
        // ブレンドが一致しないため、描画が出来ない
        jclog("fail blend");
        return jcfalse;
    }

    // テクスチャ番号
    s32 textureIndex = -1;

    // テクスチャを登録可能かをチェック
    if (sprite->texture) {
        // テクスチャを追加しきれない場合、追加は出来ない
        if (!env->addTexture(sprite->texture, &textureIndex)) {
            jclog("fail textures");
            return jcfalse;
        }
    }

    // 頂点数を加算しておく
    indices += sprite->vertex_info_length;

    GLuint last_index = 0;
    // 頂点を生成する
    PrimitiveBatchVertex vtx;
    for (int i = 0; i < sprite->vertex_info_length; ++i) {
        vtx.position = sprite->vertex_info[i].pos;
        vtx.coord = sprite->vertex_info[i].coord;
        vtx.color = sprite->vertex_info[i].color;
        vtx.rotate = sprite->rotate;
        vtx.texture_index = textureIndex;

        // 末尾に追加する
        {
            const GLushort next_index = (GLushort) mesh->vertices.size();
            last_index = next_index;

            // ２つめ以降のプリミティブには接続用のダミーインデックスを挿入する
            if (i == 0 && mesh->vertices.size()) {
                // 最後に指定されたインデックス
                const GLushort last_index = mesh->indices[mesh->indices.size() - 1];

                // ダミーインデックスを追加する
                mesh->indices.push_back(last_index);
                mesh->indices.push_back(next_index);

                // ダミーインデックス分の頂点数を加算する
                indices += 2;
            }

            // 実描画用のインデックスを追加する
            mesh->vertices.push_back(vtx);
            mesh->indices.push_back(next_index);
        }
    }

    // 奇数個頂点ならばカリング調整用インデックスを挿入する
    if (indices % 2) {
        mesh->indices.push_back(last_index);
        ++indices;
    }

    return jctrue;
}

}
}

