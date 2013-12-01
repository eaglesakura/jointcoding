/*
 * SpriteBatch.cpp
 *
 *  Created on: 2013/12/01
 */

#include    "jc/framework/assets/image/SpriteBatch.h"

namespace jc {
namespace gl {

#define stream (this->getCurrentVertices())

SpriteBatch::SpriteBatch(MDevice device) {
    commitStreamIndex = -1;
    renderingStreamIndex = -1;

    surfaceSize.x = device->getSurface()->getWidth();
    surfaceSize.y = device->getSurface()->getHeight();

    for (int i = 0; i < BATCHBUFFER_NUM; ++i) {
        vertices[i].gpuIndices.reset(new IndexBufferObject(device));
        vertices[i].gpuVertices.reset(new VertexBufferObject<BatchVertex>(device));
    }
}

SpriteBatch::~SpriteBatch() {
}
/**
 * レンダリング用シェーダーを設定する
 */
void SpriteBatch::setShader(MGLState state, MGLShaderProgram shader) {
    this->shader = shader;

    shader->bind(state);
    attribute.position.setLocation(shader, "attr_position");
    attribute.position.enable(state);

    attribute.color.setLocation(shader, "attr_coord");
    attribute.color.enable(state);

    attribute.move.setLocation(shader, "attr_move");
    attribute.move.enable(state);

    attribute.rotate.setLocation(shader, "attr_rotate");
    attribute.rotate.enable(state);

    attribute.color.setLocation(shader, "attr_color");
    attribute.color.enable(state);

}

/**
 * 描画の開始を行う
 */
void SpriteBatch::begin(MGLState state) {
    // コミット対象を次へ設定する
    commitStreamIndex = (commitStreamIndex + 1) % BATCHBUFFER_NUM;

    stream.cpuIndices.clear();
    stream.cpuVertices.clear();
}

/**
 * 四角形を描画する
 */
void SpriteBatch::nextRect(const float x, const float y, const float w, const float h, const Color color) {
    BatchVertex vert[4];

    // 正規化座標系のX/Y
    const float normX = (x / surfaceSize.x * 2.0f) - 1.0f;
    const float normY = (y / surfaceSize.y * 2.0f);
    // 正規化座標系のWidth/Height
    const float normWidth = w / surfaceSize.x * 2.0f;
    const float normHeight = h / surfaceSize.y * 2.0f;

    {
        // v0 = left top
        vert[0].pos.x = -normWidth / 2.0f;
        vert[0].pos.y = normHeight / 2.0f;

        // v1 = left bottom
        vert[1].pos.x = -normWidth / 2.0f;
        vert[1].pos.y = -normHeight / 2.0f;

        // v2 = right top
        vert[2].pos.x = normWidth / 2.0f;
        vert[2].pos.y = normHeight / 2.0f;

        // v3 = right bottom
        vert[3].pos.x = normWidth / 2.0f;
        vert[3].pos.y = -normHeight / 2.0f;
    }

//    jclogf("x(%f) y(%f) w(%f) h(%f)", normX, normY, normWidth, normHeight);

    for (int i = 0; i < 4; ++i) {
        vert[i].color = color;

        // 移動量・回転量を設定する
        vert[i].rotate = 0;
        vert[i].move.set(normWidth / 2 + normX, (-normHeight / 2) + 1.0f - normY);
    }

    const s32 vertBeginIndex = stream.cpuVertices.size();
    for (int i = 0; i < 4; ++i) {
        // 頂点をストリームへ書き込む
        stream.cpuVertices.push_back(vert[i]);
    }

    // 既に四角形が登録されているなら、縮退化して連結する
    if (vertBeginIndex) {
        stream.cpuIndices.push_back(vertBeginIndex - 1);
        stream.cpuIndices.push_back(vertBeginIndex);
    }
    for (int i = 0; i < 4; ++i) {
        // インデックスをストリームへ書き込む
        stream.cpuIndices.push_back(vertBeginIndex + i);
    }
}

/**
 * ストリームをGPUへ書き込む
 */
void SpriteBatch::commit(MGLState state) {
    if (stream.cpuIndices.empty()) {
        // 描画対象のストリームが無い
        jclog("stream is empty");
        return;
    }

    // 頂点/インデックスストリームを書き込む
    {
        stream.gpuVertices->bind(state);
        stream.gpuVertices->bufferData(&(stream.cpuVertices[0]), sizeof(BatchVertex), stream.cpuVertices.size(), GL_DYNAMIC_DRAW);

        stream.gpuIndices->bind(state);
        stream.gpuIndices->bufferData(&(stream.cpuIndices[0]), stream.cpuIndices.size(), GL_DYNAMIC_DRAW);
    }

    // レンダリング対象のインデックスを保存する
    renderingStreamIndex = commitStreamIndex;
}

/**
 * 最後にコミットしたストリームを描画する
 */
void SpriteBatch::rendering(MGLState state) {

    shader->bind(state);
    vertices[renderingStreamIndex].gpuVertices->bind(state);
    vertices[renderingStreamIndex].gpuIndices->bind(state);

    {
        // attribute設定
        attribute.position.attributePointer(state);
        attribute.coord.attributePointer(state);
        attribute.move.attributePointer(state);
        attribute.rotate.attributePointer(state);
        attribute.color.attributePointer(state);
    }
    vertices[renderingStreamIndex].gpuIndices->rendering(GL_TRIANGLE_STRIP);
}

}
}
