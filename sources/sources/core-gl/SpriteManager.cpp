/*
 * jcglSpriteManager.cpp
 *
 *  Created on: 2012/10/05
 */
#include    "jc/gl/SpriteManager.h"

namespace jc {
namespace gl {

/**
 * 初期化を行う
 */
void SpriteManager::initialize(MDevice device) {
    this->attrVertices = shader->getAttribLocation("vPosition");
    this->attrCoords = shader->getAttribLocation("vTexCoord");

    assert(attrVertices != ATTRIBUTE_DISABLE_INDEX);
    assert(attrCoords != ATTRIBUTE_DISABLE_INDEX);

    this->unifPolyData = shader->getUniformLocation("poly_data");
    assert(unifPolyData != UNIFORM_DISABLE_INDEX);
    this->unifTexture = shader->getUniformLocation("tex");
    assert(unifTexture != UNIFORM_DISABLE_INDEX);
    this->unifPolyUv = shader->getUniformLocation("poly_uv");
    assert(unifPolyData != UNIFORM_DISABLE_INDEX);
    this->unifBlendColor = shader->getUniformLocation("blendColor");
    assert(unifBlendColor != UNIFORM_DISABLE_INDEX);

    this->quad.reset(new Quad(device));

    quad->attributes(attrVertices, attrCoords);

    {
        whiteTexture.reset(new TextureImage(1, 1, device));
        Color pix = Color::fromRGBAf(1.0f, 1.0f, 1.0f, 1.0f);
        whiteTexture->bind();
        {
            whiteTexture->copyPixelLine(&pix, GL_UNSIGNED_BYTE, GL_RGBA, 0, 0, 1);
        }
        whiteTexture->unbind();
    }
}

SpriteManager::SpriteManager(MDevice device, MGLShaderProgram shader) {
    this->device = device;
    this->shader = shader;
    this->shaderContext.rotate = 0;
    this->shaderContext.bindedTextureIndex = 0;
    this->unifPolyData = UNIFORM_DISABLE_INDEX;
    this->unifTexture = UNIFORM_DISABLE_INDEX;
    this->unifPolyUv = UNIFORM_DISABLE_INDEX;
    this->unifBlendColor = UNIFORM_DISABLE_INDEX;
    this->attrCoords = ATTRIBUTE_DISABLE_INDEX;
    this->attrVertices = ATTRIBUTE_DISABLE_INDEX;
    this->initialize(device);
}

SpriteManager::~SpriteManager() {
    this->dispose();
}

/**
 * 現在の環境にしたがってレンダリングさせる。
 */
void SpriteManager::rendering(s32 x, s32 y, s32 width, s32 height) {
    const float displayWidth = (float) device->getSurface()->getWidth();
    const float displayHeight = (float) device->getSurface()->getHeight();

    const float sizeX = (float) width / (float) displayWidth * 2;
    const float sizeY = (float) height / (float) displayHeight * 2;
    const float sx = (float) x / (float) displayWidth * 2;
    const float sy = (float) y / (float) displayHeight * 2;
    const float translateX = -1.0f + sizeX / 2.0f + sx;
    const float translateY = 1.0f - sizeY / 2.0f - sy;

    float poly_data[] = { translateX, translateY, sizeX, sizeY };

    // データを転送する
    glUniform4fv(unifPolyData, 1, poly_data);

    quad->rendering();
}

/**
 * 四角形描画を行う
 */
void SpriteManager::renderingRect(const s32 x, const s32 y, const s32 w, const s32 h, const u32 rgba) {
    renderingImage(whiteTexture, 0, 0, 0, 0, x, y, w, h, 0.0f, rgba);
}

/**
 * レンダリングを行う
 */
void SpriteManager::renderingImage(MTextureImage image, const s32 srcX, const s32 srcY, const s32 srcW, const s32 srcH, const s32 dstX, const s32 dstY, const s32 dstWidth, const s32 dstHeight, const float degree, const u32 rgba) {
    // シェーダーを切り替える
    shader->bind();

    // 変更前のテクスチャを保持しておく
    {
        const s32 old_bindedTextureIndex = shaderContext.bindedTextureIndex;
        // テクスチャ番号を設定する
        shaderContext.bindedTextureIndex = image->bind();
        if (old_bindedTextureIndex != shaderContext.bindedTextureIndex) {
            glUniform1i(unifTexture, shaderContext.bindedTextureIndex);
        }
    }

    // ブレンド色を設定する
    if (shaderContext.blendColor.colors.rgba != rgba) {
        shaderContext.blendColor = Color::fromRGBAi(rgba);
        glUniform4f(unifBlendColor, shaderContext.blendColor.rf(), shaderContext.blendColor.gf(), shaderContext.blendColor.bf(), shaderContext.blendColor.af());
    }

    //! テクスチャ描画位置を行列で操作する
    if (image != whiteTexture) {
        const float sizeX = (float) srcW / (float) image->getWidth();
        const float sizeY = (float) srcH / (float) image->getHeight();
        const float sx = (float) srcX / (float) image->getWidth();
        const float sy = (float) srcY / (float) image->getHeight();

        const float poly_uv[] = { sx, sy, sizeX, sizeY, };
        glUniform4fv(unifPolyUv, 1, poly_uv);
    }

    this->rendering(dstX, dstY, dstWidth, dstHeight);
}

/**
 * 開放処理
 */
void SpriteManager::dispose() {
    quad.reset();
    shader.reset();
    whiteTexture.reset();
    device.reset();
}

MSpriteManager SpriteManager::createInstance(MDevice device, const Uri vertexShaderUri, const Uri fragmentShaderUri) {
    MGLShaderProgram program = ShaderProgram::buildFromUri(device, vertexShaderUri, fragmentShaderUri);
    if (!program) {
        return MSpriteManager();
    }

    MSpriteManager result(new SpriteManager(device, program));

    return result;
}

}
}

