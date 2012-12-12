/*
 * jcglSpriteManager.h
 *
 *  Created on: 2012/10/05
 */

#ifndef JCGLSPRITEMANAGER_H_
#define JCGLSPRITEMANAGER_H_

#include    "jcglDevice.h"
#include    "jcglQuad.h"
#include    "jcglShaderProgram.h"
#include    "jcglTextureImage.h"
#include    "jcglQuad.h"

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
    virtual void rendering( s32 x, s32 y, s32 w, s32 h );

public:
    virtual ~SpriteManager();

    /**
     * 四角形描画を行う
     */
    virtual void renderingRect(const s32 x, const s32 y, const s32 w, const s32 h, const u32 rgba);

    /**
     * 画像を描画する
     * @param src{XYWH} テクスチャ内の座標をpix単位で指定する
     * @param dst{XYWH} 描画先の座標をpix単位で指定する。
     * @param degree 画像の回転角（360度系）
     * @param rgba   画像の色補正（RGBA形式）
     */
    virtual void renderingImage( MTextureImage image, const s32 srcX, const s32 srcY, const s32 srcW, const s32 srcH, const s32 dstX, const s32 dstY, const s32 dstW, const s32 dstH, const float degree, const u32 rgba);

    /**
     * 画像を描画する
     */
    virtual void renderingImage( MTextureImage image, const s32 srcX, const s32 srcY, const s32 srcW, const s32 srcH, const s32 dstX, const s32 dstY, const s32 dstW, const s32 dstH) {
        renderingImage(image, srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH, 0.0f, 0xFFFFFFFF);
    }

    /**
     * 明示的な解放を行う
     */
    virtual void dispose();

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
