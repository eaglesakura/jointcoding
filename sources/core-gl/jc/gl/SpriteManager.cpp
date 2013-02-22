/*
 * jcglSpriteManager.cpp
 *
 *  Created on: 2012/10/05
 */
#include    "jc/gl/SpriteManager.h"

namespace jc {
namespace gl {

namespace {
static const charactor *VERTEX_SHADER_SOURCE =
//
        /**
         * ( poly_x | poly_y )はそれぞれ0.0f〜1.0fの正規化座標で設定する。
         * poly_yについては2D座標系を基本として、画面左上が(0, 0)、画面右下が(1, 1)として扱う。
         */
// ポリゴンのXYWH
        ""
                "uniform mediump vec4    poly_data;"
// ポリゴンの回転角度
                "uniform mediump float   rotate;"
// ポリゴンのUV情報
                "uniform mediump vec4    poly_uv;\n"
// アクセス用のショートカットを設定する
                "#define poly_x         poly_data[0]\n"
                "#define poly_y         poly_data[1]\n"
                "#define poly_width     poly_data[2]\n"
                "#define poly_height    poly_data[3]\n"
//
                "#define poly_uv_u      poly_uv[0]\n"
                "#define poly_uv_v      poly_uv[1]\n"
                "#define poly_uv_w      poly_uv[2]\n"
                "#define poly_uv_h      poly_uv[3]\n"
//
                "attribute vec4 vTexCoord;"
                "attribute vec4 vPosition;"
                "varying vec2 fTexCoord;"

                "void main() {"
// 位置操作
                "   {"
                "       mediump mat4 trans = mat4(1.0);"
                "       mediump mat4 scale = mat4(1.0);"
// 移動行列を作成する
                "       {"
                "           trans[3][0] = poly_x;"
                "           trans[3][1] = poly_y;"
                "       }"
// スケーリング行列を作成する
                "       {"
                "           scale[0][0] = poly_width;"
                "           scale[1][1] = poly_height;"
                "       }"
                "       gl_Position = trans * scale * vPosition;"
                "   }"
// テクスチャ操作
                "   {"
                "       mediump mat4 trans = mat4(1.0);"
// 移動行列を作成する
                "       {"
                "           trans[3][0] = poly_uv_u;"
                "           trans[3][1] = poly_uv_v;"
                "       }"
                "       mediump mat4 scale = mat4(1.0);"
// スケーリング行列を作成する
                "       {"
                "           scale[0][0] = poly_uv_w;"
                "           scale[1][1] = poly_uv_h;"
                "       }"
                "       mediump vec4 tempTex = trans * scale * vTexCoord;"
                "       fTexCoord.x = tempTex.x;"
                "       fTexCoord.y = tempTex.y;"
                "   }"
                "}"
//
;

static const charactor *FRAGMENT_SHADER_SOURCE = ""
        ""
        "        precision mediump float;"
        // UV setting"
        "        varying vec2 fTexCoord;"
        // texture
        "        uniform sampler2D tex;"
        // color
        "        uniform mediump vec4    blendColor;"
        "        void main() {"
        "            vec4 color = texture2D(tex, fTexCoord) * blendColor;"
        "            color.a *= blendColor.a;"
        "            gl_FragColor = color;"
        "        }"
//
;

//
}

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

    // シェーダーが設定されて無ければ、組み込みで起動する
    if (!shader) {
        this->shader = jc::gl::ShaderProgram::buildFromSource(device, VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
        assert(this->shader.get() != NULL);
    }
    this->shaderContext.rotate = 0;
    this->shaderContext.bindedTextureIndex = 0;
    this->unifPolyData = UNIFORM_DISABLE_INDEX;
    this->unifTexture = UNIFORM_DISABLE_INDEX;
    this->unifPolyUv = UNIFORM_DISABLE_INDEX;
    this->unifBlendColor = UNIFORM_DISABLE_INDEX;
    this->attrCoords = ATTRIBUTE_DISABLE_INDEX;
    this->attrVertices = ATTRIBUTE_DISABLE_INDEX;
    this->shaderContext.blendColor = Color::fromRGBAi(0, 0, 0, 0);
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
    if (shaderContext.blendColor.rgba != rgba) {
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

/**
 * インスタンスを作成する
 */
MSpriteManager SpriteManager::createInstance(MDevice device) {
    MSpriteManager result(new SpriteManager(device, MGLShaderProgram()));
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

