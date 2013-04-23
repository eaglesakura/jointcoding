/*
 * jcglSpriteManager.cpp
 *
 *  Created on: 2012/10/05
 */
#include    "jc/gl/sprite/SpriteManager.h"

// shaders
#include    "jc/gl/sprite/prv_SpriteImageShader.hpp"
#include    "jc/gl/sprite/prv_SpriteExternalImageShader.hpp"

namespace jc {
namespace gl {

namespace {

const static QuadVertex g_revert_vertices[] = {
// 左上
        { -0.5, 0.5, 0.0f, 1.0f, },
        // 左下
        { -0.5, -0.5, 0.0f, 0.0f },
        // 右下
        { 0.5, -0.5, 1.0f, 0.0f },
        // 右上
        { 0.5, 0.5, 1.0f, 1.0f }, };
//
}

/**
 * 初期化を行う
 */
void SpriteManager::initialize(MDevice device) {

    // uniformを設定する
    {
        uniform.poly_data.setLocation(shader, "poly_data");
        uniform.poly_uv.setLocation(shader, "poly_uv");
        uniform.texture.setLocation(shader, "tex");
        uniform.color.setLocation(shader, "blendColor");
        uniform.aspect.setLocation(shader, "aspect");
        uniform.rotate.setLocation(shader, "rotate");
        uniform.texture_matrix.setLocation(shader, "unif_texm");
        if (uniform.texture_matrix.valid()) {
            // テクスチャ行列が有効なら単位行列をデフォルト指定する
            setTextureMatrix(Matrix4x4());
        }
    }

    // attrを設定する
    {
        this->quad.reset(new Quad(device));
        quad->setPositionAttribute(QuadPositionAttribute(shader, "vPosition"));
        quad->setCoordAttribute(QuadCoordAttribute(shader, "vTexCoord"));
    }

    {
        whiteTexture.reset(new TextureImage(1, 1, device));
        whiteTexture->bind();
        {
            u16 rgb565 = 0xFFFF;
            whiteTexture->copyPixelLine(&rgb565, PixelFormat_RGB565, 0, 0, 1);
        }
        whiteTexture->unbind();
    }

    // default shader context
    setSurfaceAspect(device->getSurface()->getWidth(), device->getSurface()->getHeight());
}

SpriteManager::SpriteManager(MDevice device, MGLShaderProgram shader) {
    this->device = device;

    this->shader = shader;

// シェーダーが設定されて無ければ、組み込みで起動する
    if (!shader) {
        this->shader = jc::gl::ShaderProgram::buildFromSource(device, VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
    }
    assert(this->shader.get() != NULL);

    this->attrCoords = ATTRIBUTE_DISABLE_INDEX;
    this->attrVertices = ATTRIBUTE_DISABLE_INDEX;
    this->initialize(device);
}

SpriteManager::~SpriteManager() {
    this->dispose();
}

/**
 * テクスチャ用行列を設定する
 */
void SpriteManager::setTextureMatrix(const Matrix4x4 &m) {
    shader->bind();
    uniform.texture_matrix.upload(m);
}

/**
 * サーフェイスのアスペクト比を設定する
 */
void SpriteManager::setSurfaceAspect(const u32 surface_width, const u32 surface_height) {
    shader->bind();
    uniform.aspect.upload((float) surface_width / (float) surface_height);
}

/**
 * 現在の環境にしたがってレンダリングさせる。
 */
void SpriteManager::rendering(const float x, const float y, const float width, const float height) {

    {
        // ポリゴンのXYWH情報を生成する
        const float displayWidth = (float) device->getSurface()->getWidth();
        const float displayHeight = (float) device->getSurface()->getHeight();

        const float sizeX = width / displayWidth * 2;
        const float sizeY = height / displayHeight * 2;
        const float sx = x / displayWidth * 2;
        const float sy = y / displayHeight * 2;
        const float translateX = -1.0f + sizeX / 2.0f + sx;
        const float translateY = 1.0f - sizeY / 2.0f - sy;
        // データを転送する
        uniform.poly_data.upload(translateX, translateY, sizeX, sizeY);
    }

    // レンダリングを行う
    quad->rendering();
}

/**
 * 四角形描画を行う
 */
void SpriteManager::renderingRect(const float x, const float y, const float w, const float h, const u32 rgba) {
    renderingImage(whiteTexture, 0, 0, 0, 0, x, y, w, h, 0.0f, rgba);
}

/**
 * レンダリングを行う
 */
void SpriteManager::renderingImage(MTextureImage image, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstWidth, const float dstHeight, const float degree, const u32 rgba) {
// シェーダーを切り替える
    shader->bind();
    // テクスチャを転送する
    uniform.texture.upload(image);
    // ブレンド色を設定する
    uniform.color.upload(rgba);
    // ポリゴン回転を設定する
    uniform.rotate.upload(jc::degree2radian(degree));

//! テクスチャ描画位置を行列で操作する
    if (image != whiteTexture) {
        const float TEXTURE_WIDTH = (float) image->getTextureWidth();
        const float TEXTURE_HEIGHT = (float) image->getTextureHeight();

        const float sizeX = (float) jc::round(srcW) / TEXTURE_WIDTH;
        const float sizeY = (float) jc::round(srcH) / TEXTURE_HEIGHT;
        const float sx = (float) jc::round(srcX) / TEXTURE_WIDTH;
        const float sy = (float) jc::round(srcY) / TEXTURE_HEIGHT;

        uniform.poly_uv.upload(sx, sy, sizeX, sizeY);
    } else {
        // UV値を0に設定して、テクスチャキャッシュを使わせる
        uniform.poly_uv.upload(0, 0, 0, 0);
    }

    this->rendering(jc::round(dstX), jc::round(dstY), (s32) dstWidth, (s32) dstHeight);
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

/**
 * インスタンスを作成する
 */
MSpriteManager SpriteManager::createInstance(MDevice device) {
    jclog("createInstance");
    MSpriteManager result(new SpriteManager(device, MGLShaderProgram()));
    return result;
}

/**
 * インスタンスを作成する
 */
MSpriteManager SpriteManager::createExternalInstance(MDevice device) {
    jclog("createExternalInstance");
    MGLShaderProgram program = jc::gl::ShaderProgram::buildFromSource(device, VERTEX_EXTERNAL_SHADER_SOURCE, FRAGMENT_EXTERNAL_SHADER_SOURCE);
    assert(program.get() != NULL);
    MSpriteManager result(new SpriteManager(device, program));
    result->getRenderingQuad()->updateVertices(g_revert_vertices);
    return result;
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

