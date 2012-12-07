/*
 * jcglSpriteManager.cpp
 *
 *  Created on: 2012/10/05
 */
#include    "jcglSpriteManager.h"

namespace jc {
namespace gl {

/**
 * 初期化を行う
 */
void SpriteManager::initialize(MDevice device) {
    this->attrVertices = shader->getAttribLocation("vPosition");
    this->attrCoords = shader->getAttribLocation("vTexCoord");

    this->unifPolyData = shader->getUniformLocation("poly_data");
    this->unifTexture = 0;
    this->unifPolyUv = shader->getUniformLocation("poly_uv");

    this->quad.reset(new Quad(device));
    quad->attributes(attrVertices, attrCoords);
}

SpriteManager::SpriteManager(MDevice device, MGLShaderProgram shader) {
    this->device = device;
    this->shader = shader;
    this->shaderContext.rotate = 0;
    this->shaderContext.texEnable = jcfalse;
    this->unifPolyData = UNIFORM_DISABLE_INDEX;
    this->unifTexture = UNIFORM_DISABLE_INDEX;
    this->unifPolyUv = UNIFORM_DISABLE_INDEX;
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

    // 描画する
    shader->bind();
    quad->rendering();
}

/**
 * レンダリングを行う
 */
void SpriteManager::rendering(MTextureImage texture, const s32 srcX, const s32 srcY, const s32 srcW, const s32 srcH, const s32 dstX, const s32 dstY, const s32 dstWidth, const s32 dstHeight) {
    s32 unitIndex = texture->bind();
    const float displayWidth = (float) device->getSurface()->getWidth();
    const float displayHeight = (float) device->getSurface()->getHeight();

    //! 描画位置を行列で操作する
    {
        const float sizeX = (float) srcW / (float) texture->getWidth();
        const float sizeY = (float) srcH / (float) texture->getHeight();
        const float sx = (float) srcX / (float) texture->getWidth();
        const float sy = (float) srcY / (float) texture->getHeight();

        const float poly_uv[] = { sx, sy, sizeX, sizeY, };
        glUniform4fv(unifPolyUv, 1, poly_uv);
//        gl.glTranslatef(sx, sy, 0.0f);
//        gl.glScalef(sizeX, sizeY, 1.0f);
//        gl.glMatrixMode(GL10.GL_MODELVIEW);
    }

    this->rendering(dstX, dstY, dstWidth, dstHeight);
}

/**
 * 開放処理
 */
void SpriteManager::dispose() {
    quad.reset();
    shader.reset();
    device.reset();
}

MSpriteManager SpriteManager::createInstance(MDevice device) {
    MGLShaderProgram program = ShaderProgram::buildFromUri(device, Uri::fromAssets("basic/shader/sprite.vsd"), Uri::fromAssets("basic/shader/sprite.fsd"));
    if (!program) {
        return MSpriteManager();
    }

    MSpriteManager result(new SpriteManager(device, program));
    return result;
}

}
}

