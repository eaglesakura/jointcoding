/*
 * SpriteBatchGroup.cpp
 *
 *  Created on: 2013/07/04
 */

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {
namespace fw {

/**
 * リクエストを開始する
 */
void SpriteBatchGroup::beginRequest(SpriteBatchSource *mesh) {

}

/**
 * レンダリングリクエストを送る
 * リクエストが受諾された場合trueを返す
 */
jcboolean SpriteBatchGroup::requestRendering(MSpriteBatchEnvironmentState env, SpriteBatchSource *mesh, const SpriteRequest *sprite) {
    assert(env);
    assert(mesh);
    assert(sprite);

    // テクスチャ番号
    s32 textureIndex = -1;

    // テクスチャを登録可能かをチェック
    if (sprite->texture) {
        // テクスチャを追加しきれない場合、追加は出来ない
        if (!env->addTexture(sprite->texture, &textureIndex)) {
            return jcfalse;
        }
    }

    // 頂点を生成する
    SpriteBatchVertex vtx;
    for (int i = 0; i < sprite->vertex_info_length; ++i) {
        vtx.position = sprite->vertex_info[i].pos;
        vtx.coord = sprite->vertex_info[i].coord;
        vtx.color = sprite->vertex_info[i].color;
        vtx.rotate = sprite->rotate;
        vtx.texture_index = textureIndex;

        // 末尾に追加する
        {
            const GLushort next_index = (GLushort) mesh->vertices.size();

            // 接続用のダミーインデックスを挿入する
            if (i == 0 && mesh->vertices.size()) {
                // 最後に指定されたインデックス
                const GLushort last_index = mesh->indices[mesh->indices.size() - 1];

                // ダミーインデックスを追加する
                mesh->indices.push_back(last_index);
                mesh->indices.push_back(next_index);
            }

            // 実描画用のインデックスを追加する
            mesh->vertices.push_back(vtx);
            mesh->indices.push_back(next_index);
        }
    }

    return jctrue;
}

}
}

