/*
 * jcglSpriteManager.h
 *
 *  Created on: 2012/10/05
 */

#ifndef JCGLSPRITEMANAGER_H_
#define JCGLSPRITEMANAGER_H_

#include    "jc/math/Matrix.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/gl/sprite/Quad.h"
#include    "jc/gl/shader/ShaderProgram.h"
#include    "jc/gl/texture/TextureImage.h"

#include    "jc/gl/shader/VectorUniform.hpp"
#include    "jc/gl/shader/TextureUniform.hpp"
#include    "jc/gl/shader/ColorUniform.hpp"

namespace jc {
namespace gl {

/**
 * スプライト描画クラス。
 */
class SpriteManager: public Object {
protected:
    /**
     * スプライト用シェーダー
     */
    MGLShaderProgram shader;

    /**
     * ステータス情報
     */
    MDevice device;

    /**
     * 頂点属性インデックス
     * @shader_attribute vPosition
     */
    s32 attrVertices;

    /**
     * UV属性インデックス
     * @shader_attribute vTexCoord
     */
    s32 attrCoords;

    /**
     * ポリゴン情報のuniformインデックス
     * @shader_uniform poly_data
     */
    s32 unifPolyData;

    /**
     * ポリゴンの回転
     */
    s32 unifRotate;

    /**
     * テクスチャ行列用uniform
     */
    s32 unifTexM;

    /**
     * テクスチャ情報のuniformインデックス
     * @shader_uniform tex
     */
    s32 unifTexture;

    /**
     * テクスチャUVのuniformインデックス
     * @shader_uniform poly_uv
     */
    s32 unifPolyUv;

    /**
     * ポリゴンのブレンド色
     */
    s32 unifBlendColor;

    /**
     * 画面アスペクト比
     */
    s32 unifAspect;

    /**
     * 四角形描画用の白テクスチャ
     */
    MTextureImage whiteTexture;

    /**
     * 保持しておくべきシェーダー設定値
     */
    struct {
        /**
         * 回転角度情報
         * 同値が設定されている場合、何もしない。
         * degreeで設定する。
         * 初期値は0.0
         */
        float rotate;

        /**
         * サーフェイスのアスペクト比
         */
        float aspect;

        /**
         * バインドされたテクスチャ番号
         */
        s32 bindedTextureIndex;

        /**
         * ポリゴンのブレンド色
         */
        Color blendColor;
    } shaderContext;

    /**
     * レンダリング用の板ポリゴン
     */
    jc_sp<Quad> quad;

    /**
     * 初期化を行う
     */
    virtual void initialize(MDevice device);

    SpriteManager(MDevice device, MGLShaderProgram shader);

    /**
     * 現在の環境にしたがってレンダリングさせる。
     */
    virtual void rendering(const float x, const float y, const float w, const float h );

public:
    virtual ~SpriteManager();

    /**
     * 四角形描画を行う
     */
    virtual void renderingRect(const float x, const float y, const float w, const float h, const u32 rgba);

    /**
     * 四角形描画を行う
     */
    virtual void renderingRect(const RectI &dst, const u32 rgba) {
        renderingRect(dst.left, dst.top, dst.width(), dst.height(), rgba);
    }

    /**
     * 四角形描画を行う
     */
    virtual void renderingRect(const RectF &dst, const u32 rgba) {
        renderingRect(dst.left, dst.top, dst.width(), dst.height(), rgba);
    }

    /**
     * 画像を描画する
     * @param src{XYWH} テクスチャ内の座標をpix単位で指定する
     * @param dst{XYWH} 描画先の座標をpix単位で指定する。
     * @param degree 画像の回転角（360度系）
     * @param rgba   画像の色補正（RGBA形式）
     */
    virtual void renderingImage( MTextureImage image, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH, const float degree, const u32 rgba);

    /**
     * 画像を描画する
     * @param src{XYWH} テクスチャ内の座標をpix単位で指定する
     * @param dst{XYWH} 描画先の座標をpix単位で指定する。
     * @param degree 画像の回転角（360度系）
     * @param rgba   画像の色補正（RGBA形式）
     */
    virtual void renderingImage( MTextureImage image, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH) {
        renderingImage(image, srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH, 0.0f, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const float x, const float y, const float width, const float height, const Color color) {
        const u32 IMG_WIDTH = image->getWidth();
        const u32 IMG_HEIGHT = image->getHeight();
        renderingImage(image, 0, 0, IMG_WIDTH, IMG_HEIGHT, x, y, width, height, 0.0f, color.rgba);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const float x, const float y, const float width, const float height) {
        const u32 IMG_WIDTH = image->getWidth();
        const u32 IMG_HEIGHT = image->getHeight();
        renderingImage(image, 0, 0, IMG_WIDTH, IMG_HEIGHT, x, y, width, height, 0.0f, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const float x, const float y, const Color color) {
        const u32 IMG_WIDTH = image->getWidth();
        const u32 IMG_HEIGHT = image->getHeight();
        renderingImage(image, 0, 0, IMG_WIDTH, IMG_HEIGHT, x, y, IMG_WIDTH, IMG_HEIGHT, 0.0f, color.rgba);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const float x, const float y) {
        const u32 IMG_WIDTH = image->getWidth();
        const u32 IMG_HEIGHT = image->getHeight();
        renderingImage(image, 0, 0, IMG_WIDTH, IMG_HEIGHT, x, y, IMG_WIDTH, IMG_HEIGHT, 0.0f, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const RectI &dst) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), 0, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const RectF &dst) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), 0, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const RectI &dst, const u32 color) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), 0, color);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const RectF &dst, const u32 color) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), 0, color);
    }

    /**
     * テクスチャ用行列を設定する
     */
    virtual void setTextureMatrix(const Matrix4x4 &m);

    /**
     * サーフェイスのアスペクト比を設定する
     */
    virtual void setSurfaceAspect(const u32 surface_width, const u32 surface_height);

    /**
     * レンダリング用の矩形を取得する
     */
    virtual jc_sp<Quad> getRenderingQuad() const {
        return quad;
    }

    /**
     * デバッグ用のエリアレンダリング（fill色で塗りつぶし後、line色で線を描く）を行う。
     */
    virtual void renderingArea( const RectF &area, const rgba32 fillColor, const rgba32 lineColor) {
        {
            startQuadRendering();
            renderingRect(area, fillColor);
        }
        {
            RectF lineArea = area;
            startLineRendering();
            setLineWidth(2.0f);
            lineArea.inset(1, 1);
            renderingRect(lineArea, lineColor);
        }
        startQuadRendering();

    }

    /**
     * 線レンダリングの幅を取得する
     */
    virtual void setLineWidth( const float width) {
        device->getState()->lineWidth(2.0f);
    }

    /**
     * 線描画を開始する
     */
    virtual void startLineRendering() {
        quad->setPrimitiveType(GL_LINE_LOOP);
        device->getState()->lineWidth(2.0f);
    }

    /**
     * 四角形描画を開始する
     */
    virtual void startQuadRendering() {
        quad->setPrimitiveType(GL_TRIANGLE_FAN);
    }

    /**
     * レンダリングエリアを設定する
     * 設定はディスプレイ座標系（左上原点）で行う
     */
    virtual void setRenderArea(const RectI &display) {
        setRenderArea(display.left, display.top, display.width(), display.height());
    }

    /**
     * レンダリングエリアを設定する
     * 設定はディスプレイ座標系（左上原点）で行う
     */
    virtual void setRenderArea(const RectF &display) {
        setRenderArea(jc::round(display.left), jc::round(display.top), jc::round(display.width()), jc::round(display.height()));
    }

    /**
     * レンダリングエリアを設定する
     * 設定はディスプレイ座標系（左上原点）で行う
     */
    virtual void setRenderArea( const s32 x, const s32 y, const s32 w, const s32 h ) {
        MGLState state = device->getState();
        s32 view_x = 0, view_y = 0, view_w = 0, view_h = 0;

        // display -> viewport
        device->convertViewportRect(createRectFromXYWH<s32>(x, y, w, h), &view_x, &view_y, &view_w, &view_h);

        // enable scissor
        state->enableScissor(jctrue);
        state->scissor(view_x, view_y, view_w, view_h);
    }

    /**
     * シザーボックスを削除する
     */
    virtual void clearRenderArea() {
        device->getState()->enableScissor(jcfalse);
    }

    /**
     * 明示的な解放を行う
     */
    virtual void dispose();

    /**
     * インスタンスを作成する
     */
    static jc_sp<SpriteManager> createInstance( MDevice device);

    /**
     * インスタンスを作成する
     */
    static jc_sp<SpriteManager> createExternalInstance( MDevice device);

    /**
     * インスタンスを作成する
     */
    static jc_sp<SpriteManager> createInstance( MDevice device, const Uri vertexShaderUri, const Uri fragmentShaderUri );
};

/**
 * managed
 */
typedef jc_sp<SpriteManager> MSpriteManager;

}
}

#endif /* JCGLSPRITEMANAGER_H_ */
