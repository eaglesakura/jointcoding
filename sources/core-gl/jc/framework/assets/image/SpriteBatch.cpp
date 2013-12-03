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

    // 白ポリゴン描画用
    {
        MTextureImage whiteTexture;
        whiteTexture.reset(new TextureImage(1, 1, device));
        whiteTexture->bind(device->getState());
        {
            const u16 rgb565 = 0xFFFF;
            whiteTexture->copyPixelLine(&rgb565, PixelFormat_RGB565, 0, 0, 1, device->getState());
        }
        whiteTexture->unbind(device->getState());

        context.white = whiteTexture;
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
    {
        attribute.position.setLocation(shader, "attr_position");
        attribute.position.enable(state);

        attribute.coord.setLocation(shader, "attr_coord");
        attribute.coord.enable(state);

        attribute.move.setLocation(shader, "attr_move");
        attribute.move.enable(state);

        attribute.rotate.setLocation(shader, "attr_rotate");
        attribute.rotate.enable(state);

        attribute.color.setLocation(shader, "attr_color");
        attribute.color.enable(state);

        attribute.textureIndex.setLocation(shader, "attr_textureIndex");
        attribute.textureIndex.enable(state);
    }
    {
        uniform.textures.setLocation(shader, "unif_textures");
    }

}

/**
 * 描画の開始を行う
 */
void SpriteBatch::begin(MGLState state) {
    // コミット対象を次へ設定する
    commitStreamIndex = (commitStreamIndex + 1) % BATCHBUFFER_NUM;

    stream.cpuIndices.clear();
    stream.cpuVertices.clear();

    // テクスチャ状態を戻す
    {
        context.textures.clear();
        context.textures.push_back(context.white);
        context.textureIndex = 0;
    }
}

/**
 * 四角形を描画する
 */
void SpriteBatch::nextRect(const float x, const float y, const float w, const float h, const Color color) {
    nextRect(0, 0, 0, 0, x, y, w, h, 0, color);
}

/**
 * テクスチャ描画を開始する
 */
jcboolean SpriteBatch::beginTexture(MTextureImage texture) {
    if (context.textures.size() > GPUCapacity::getMaxTextureUnits()) {
        // テクスチャの能力を超えた
        context.textureIndex = 0;

        jcalert("over texture unit specs!!");
        return jcfalse;
    }

    s32 index = 0;
    {
        texture_array::iterator itr = context.textures.begin(), end = context.textures.end();
        while (itr != end) {
            // テクスチャが一致したらOK
            if (texture == (*itr)) {
                context.textureIndex = index;
                return jctrue;
            }

            ++index;
            ++itr;
        }

        // インデックスが見つからなかったら末尾に追加する
        context.textures.push_back(texture);
        context.textureIndex = index;
    }
    return jctrue;
}

/**
 * 四角形を描画する
 */
void SpriteBatch::nextRect(const float imgX, const float imgY, const float imgW, const float imgH, const float x, const float y, const float w, const float h, const float rotate, const Color color) {
    BatchVertex vert[4];

    // 正規化座標系のX/Y
    const float normX = (x / surfaceSize.x * 2.0f) - 1.0f;
    const float normY = (y / surfaceSize.y * 2.0f);
    // 正規化座標系のWidth/Height
    const float normWidth = w / surfaceSize.x * 2.0f;
    const float normHeight = h / surfaceSize.y * 2.0f;

    // 頂点位置設定
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

    // テクスチャが設定さているならそれを描画する
    if (context.textureIndex) {
        MTextureImage texture = context.textures[context.textureIndex];
        const float left = imgX / (float) texture->getTextureWidth();
        const float top = imgY / (float) texture->getTextureHeight();
        const float right = (imgX + imgW) / (float) texture->getTextureWidth();
        const float bottom = (imgY + imgH) / (float) texture->getTextureHeight();

        // v0 = left top
        vert[0].coord.set(left, top);
        // v1 = left bottom
        vert[1].coord.set(left, bottom);
        // v2 = right top
        vert[2].coord.set(right, top);
        // v3 = right bottom
        vert[3].coord.set(right, bottom);
    }

//    jclogf("x(%f) y(%f) w(%f) h(%f)", normX, normY, normWidth, normHeight);

    for (int i = 0; i < 4; ++i) {
        // 色を確定する
        vert[i].color = color;
        vert[i].textureIndex = context.textureIndex;

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
 * テクスチャ描画を終了する
 */
void SpriteBatch::endTexture() {
    context.textureIndex = 0;
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
        attribute.textureIndex.attributePointer(state);
    }
    {
        // uniform設定
        uniform.textures.upload(state, &(context.textures[0]), context.textures.size());
    }
    vertices[renderingStreamIndex].gpuIndices->rendering(GL_TRIANGLE_STRIP);
}

}
}
