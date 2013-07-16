/*
 * SpriteBatchRenderer.cpp
 *
 *  Created on: 2013/07/12
 */

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {
namespace fw {

SpriteRenderer::SpriteRenderer() {

}

SpriteRenderer::~SpriteRenderer() {

}

/**
 * レンダリングを行わせる
 */
void SpriteRenderer::rendering(MGLState state, SpriteRendererCache *cache) {
    assert(state);
    assert(cache);

    s32 indices_num = 0;
    if (!this->getBatchGroup()->getRenderingRange(&indices_num)) {
        // レンダリングすべき情報が無いなら何も行わない
        jclog("no rendering range");
        return;
    }

    // 環境のセットアップを行わせる
    {
        SpriteBatchEnvironmentState *env = getEnvironment();
        env->bind(state);
    }

    // ストリップ描画を行う
    glDrawElements(GL_TRIANGLE_STRIP, indices_num, GL_UNSIGNED_SHORT, (void*) cache->vertex_head);

    // レンダリング情報を次へ進める
    cache->vertex_head += indices_num;

}

}
}

