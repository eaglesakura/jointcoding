/*
 * SpriteBatchList.cpp
 *
 *  Created on: 2013/07/12
 */

#include    "jc/framework/assets/Rendering2DAsset.hpp"

namespace jc {
namespace fw {

SpriteBatchList::SpriteBatchList() {
    this->state = BatchState_Wait;
}

SpriteBatchList::~SpriteBatchList() {
}

/**
 * 再度初期化を行う。
 * 毎フレーム、操作を開始する度に行う。
 */
void SpriteBatchList::initialize(MDevice device) {
    if (!vbo.indices) {
        vbo.indices.reset(new IndexBufferObject(device));
    }

    if (!vbo.vertices) {
        vbo.vertices.reset(new VertexBufferObject<PrimitiveBatchVertex>(device));
    }

    sprites.clear();

    primitives.indices.clear();
    primitives.vertices.clear();

    // 待受中に変更
    state = BatchState_Requesting;
}

/**
 * GPUへレンダリングソースをアップロードする
 */
void SpriteBatchList::uploadGPU(MDevice device) {
    // empty状態なら何もしない
    if (primitives.indices.empty()) {
        state = BatchState_Wait;
        return;
    }

    MGLState state = device->getState();
    // インデックスバッファ
    {
        vbo.indices->bind(state);
        if (vbo.indices->length() >= primitives.indices.size()) {
            // 既に十分な長さを確保済みであれば、部分転送だけを行う
            vbo.indices->bufferSubData(0, (const u16*) (&primitives.indices[0]), primitives.indices.size());
        } else {
            // 十分な長さが無いため、再度転送する
            vbo.indices->bufferData((const u16*) (&primitives.indices[0]), primitives.indices.size(), GL_STREAM_DRAW);
        }
    }
    // 頂点バッファ
    {
        vbo.vertices->bind(state);
        if (vbo.vertices->length() >= primitives.indices.size()) {
            // 既に十分な長さを確保済みであれば、部分転送だけを行う
            vbo.vertices->bufferSubData(0, (void*) (&primitives.vertices[0]), sizeof(PrimitiveBatchVertex), primitives.vertices.size());
        } else {
            // 十分な長さが無いため、再度転送する
            vbo.vertices->bufferData((void*) (&primitives.vertices[0]), sizeof(PrimitiveBatchVertex), primitives.vertices.size(), GL_STREAM_DRAW);
        }
    }

    // アップロード済み
    this->state = BatchState_Uploaded;
}

/**
 * レンダリングを行わせる
 */
void SpriteBatchList::rendering(MGLState state) {
    if (this->state != BatchState_Uploaded) {
        return;
    }

    vbo.indices->bind(state);
    vbo.vertices->bind(state);

    // レンダリング
    {
        std::list<MSpriteRenderer>::iterator itr = sprites.begin(), end = sprites.end();
        while (itr != end) {
            (*itr)->rendering(state);
            ++itr;
        }
    }
}

/**
 * スプライトレンダリングをリクエストする
 */
void SpriteBatchList::request(const PolygonRequest *request) {
    assert(request);

    // 既にあるバッファへ登録できるなら登録する
    if (!sprites.empty()) {
        MSpriteRenderer current = sprites.back();
        if (current->request(&primitives, request)) {
            return;
        }
    }

    // すでにあるバッファへ登録できないため、新規バッファを作る
    {
        MSpriteRenderer next(new SpriteRenderer());

        // レンダリング環境指定
        {
            SpriteBatchEnvironmentState *env = next->getEnvironment();
            next->getEnvironment()->setBlend(request->blend);
        }
        next->request(&primitives, request);

        // 末尾に追加
        sprites.push_back(next);

    }
}

}
}

