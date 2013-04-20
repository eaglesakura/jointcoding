/*
 * jcglSpriteManager.cpp
 *
 *  Created on: 2012/10/05
 */
#include    "jc/gl/sprite/SpriteManager.h"

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
                "uniform mediump float   aspect;"
// ポリゴンの回転角度
                "uniform mediump float   rotate;"
// ポリゴンのUV情報
                "uniform mediump vec4    poly_uv;\n"
// アクセス用のショートカットを設定する
                "#define poly_x         poly_data.x\n"
                "#define poly_y         poly_data.y\n"
                "#define poly_width     poly_data.z\n"
                "#define poly_height    poly_data.w\n"
//
                "#define poly_uv_u      poly_uv.x\n"
                "#define poly_uv_v      poly_uv.y\n"
                "#define poly_uv_w      poly_uv.z\n"
                "#define poly_uv_h      poly_uv.w\n"
//
                "attribute vec4 vTexCoord;"
                "attribute vec4 vPosition;"
                "varying vec2 fTexCoord;"

                "void main() {"
                "mediump mat4 mat = mat4(1.0);"
                "mediump mat4 temp = mat4(1.0);"
                "   {"

// 移動行列を作成する
                "       {"
                "           mat[3][0] = poly_x;"
                "           mat[3][1] = poly_y;"
                "       }"
// 回転行列を作成する
                "       {"
                "           temp[0][0] = cos(rotate);"
                "           temp[1][0] = sin(rotate);"
                "           temp[0][1] = -temp[1][0];"
                "           temp[1][1] = temp[0][0];"
                "           mat = mat * temp;"
                "       }"
// スケーリング行列を作成する
                "       {"
                "           temp = mat4(1.0);"
                "           temp[0][0] = poly_width * aspect;"
                "           temp[1][1] = poly_height;"
                "           mat = mat * temp;"
                "       }"
                "       {"
                "           temp = mat4(1.0);"
                "           temp[0][0] = 1.0 / aspect;"
                "           mat = mat * temp;"
                "       }"
                "       gl_Position = mat * vPosition;"
                "   }"
// テクスチャ操作
                "   {"
                "       mat = mat4(1.0);"
                "       {"
// スケーリング行列を作成する
                "           mat[0][0] = poly_uv_w;"
                "           mat[1][1] = poly_uv_h;"
// 移動行列を作成する
                "           mat[3][0] = poly_uv_u;"
                "           mat[3][1] = poly_uv_v;"
                "       }"
                "       fTexCoord.xy = (mat * vTexCoord).xy;"
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
        "            gl_FragColor = texture2D(tex, fTexCoord) * blendColor;"
        "        }"
//
;

static const charactor *VERTEX_EXTERNAL_SHADER_SOURCE =
//
        /**
         * ( poly_x | poly_y )はそれぞれ0.0f〜1.0fの正規化座標で設定する。
         */
// ポリゴンのXYWH
        ""
                "uniform mediump vec4    poly_data;"
// ポリゴンの回転角度
                "uniform mediump float   aspect;"
                "uniform mediump float   rotate;"
// ポリゴンのUV情報
                "uniform mediump mat4    unif_texm;\n"

// アクセス用のショートカットを設定する
                "#define poly_x         poly_data.x\n"
                "#define poly_y         poly_data.y\n"
                "#define poly_width     poly_data.z\n"
                "#define poly_height    poly_data.w\n"
//
                "attribute vec4 vTexCoord;"
                "attribute vec4 vPosition;"
                "varying vec2 fTexCoord;"

                "void main() {"
// 位置操作
                "   mediump mat4 mat = mat4(1.0);"
                "   {"
                "       mediump mat4 temp = mat4(1.0);"

// 移動行列を作成する
                "       {"
                "           mat[3][0] = poly_x;"
                "           mat[3][1] = poly_y;"
                "       }"
// 回転行列を作成する
                "       {"
                "           temp[0][0] = cos(rotate);"
                "           temp[1][0] = sin(rotate);"
                "           temp[0][1] = -temp[1][0];"
                "           temp[1][1] = temp[0][0];"
                "           mat = mat * temp;"
                "       }"
// スケーリング行列を作成する
                "       {"
                "           temp = mat4(1.0);"
                "           temp[0][0] = poly_width * aspect;"
                "           temp[1][1] = poly_height;"
                "           mat = mat * temp;"
                "       }"
                "       {"
                "           temp = mat4(1.0);"
                "           temp[0][0] = 1.0 / aspect;"
                "           mat = mat * temp;"
                "       }"
                "       gl_Position = mat * vPosition;"
                "   }"
// テクスチャ操作
                "   {"
                "       fTexCoord.xy = (unif_texm * vTexCoord).xy;"
                "   }"
                "}"
//
;

static const charactor *FRAGMENT_EXTERNAL_SHADER_SOURCE = ""
        ""
        "#extension GL_OES_EGL_image_external : require\n"
        "        precision mediump float;"
        // UV setting"
        "        varying vec2 fTexCoord;"
        // texture
        "        uniform samplerExternalOES tex;"
// color
        "        uniform mediump vec4    blendColor;"
        "        void main() {"
        "            gl_FragColor = texture2D(tex, fTexCoord) * blendColor;"
        "        }"
//
;

const static Quad::QuadVertex g_revert_vertices[] = {
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
    this->attrVertices = shader->getAttribLocation("vPosition");
    this->attrCoords = shader->getAttribLocation("vTexCoord");

    assert(attrVertices != ATTRIBUTE_DISABLE_INDEX);
    assert(attrCoords != ATTRIBUTE_DISABLE_INDEX);

    this->unifPolyData = shader->getUniformLocation("poly_data");
    assert(unifPolyData != UNIFORM_DISABLE_INDEX);

    uniform.texture.setUniformLocation(shader, "tex");
    uniform.color.setUniformLocation(shader, "blendColor");
    uniform.aspect.setUniformLocation(shader, "aspect");
    uniform.rotate.setUniformLocation(shader, "rotate");

    {
        // optional
        this->unifPolyUv = shader->getUniformLocation("poly_uv");
        this->unifTexM = shader->getUniformLocation("unif_texm");
        if (unifTexM != UNIFORM_DISABLE_INDEX) {
            setTextureMatrix(Matrix4x4());
        }
    }

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

    this->unifPolyData = UNIFORM_DISABLE_INDEX;
    this->unifTexM = UNIFORM_DISABLE_INDEX;
    this->unifPolyUv = UNIFORM_DISABLE_INDEX;
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
    if (unifTexM == UNIFORM_DISABLE_INDEX) {
        return;
    }

    shader->bind();
    glUniformMatrix4fv(unifTexM, 1, GL_FALSE, (const float*) m.m);
}

/**
 * サーフェイスのアスペクト比を設定する
 */
void SpriteManager::setSurfaceAspect(const u32 surface_width, const u32 surface_height) {
    shader->bind();
    uniform.aspect.uploadFloat1((float) surface_width / (float) surface_height);
}

/**
 * 現在の環境にしたがってレンダリングさせる。
 */
void SpriteManager::rendering(const float x, const float y, const float width, const float height) {
    const float displayWidth = (float) device->getSurface()->getWidth();
    const float displayHeight = (float) device->getSurface()->getHeight();

    const float sizeX = width / displayWidth * 2;
    const float sizeY = height / displayHeight * 2;
    const float sx = x / displayWidth * 2;
    const float sy = y / displayHeight * 2;
    const float translateX = -1.0f + sizeX / 2.0f + sx;
    const float translateY = 1.0f - sizeY / 2.0f - sy;

    float poly_data[] = { translateX, translateY, sizeX, sizeY };

// データを転送する
    glUniform4fv(unifPolyData, 1, poly_data);
    assert_gl();
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
    uniform.rotate.uploadFloat1(jc::degree2radian(degree));

//! テクスチャ描画位置を行列で操作する
    if (unifPolyUv != UNIFORM_DISABLE_INDEX && image != whiteTexture) {
        const float TEXTURE_WIDTH = (float) image->getTextureWidth();
        const float TEXTURE_HEIGHT = (float) image->getTextureHeight();

        const float sizeX = (float) jc::round(srcW) / TEXTURE_WIDTH;
        const float sizeY = (float) jc::round(srcH) / TEXTURE_HEIGHT;
        const float sx = (float) jc::round(srcX) / TEXTURE_WIDTH;
        const float sy = (float) jc::round(srcY) / TEXTURE_HEIGHT;

        const float poly_uv[] = { sx, sy, sizeX, sizeY, };
        glUniform4fv(unifPolyUv, 1, poly_uv);
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

