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
     * 保持しておくべきシェーダー設定値
     */
    struct {
        /**
         * テクスチャの有効・無効値
         * 初期値はfalseで、trueの場合はテクスチャ描画、falseの場合は単色描画を行う
         */
        jcboolean texEnable;

        /**
         * 回転角度情報
         * 同値が設定されている場合、何もしない。
         * degreeで設定する。
         * 初期値は0.0
         */
        float rotate;
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
    void rendering( s32 x, s32 y, s32 w, s32 h );
public:
    virtual ~SpriteManager();

    /**
     * レンダリングを行う
     * @param src{XYWH} テクスチャ内の座標をpix単位で指定する
     * @param dst{XYWH} 描画先の座標をpix単位で指定する。
     */
    virtual void rendering(MTextureImage texture, const s32 srcX,const s32 srcY,const s32 srcW,const s32 srcH,const s32 dstX,const s32 dstY,const s32 dstW,const s32 dstH);

    /**
     * 明示的な解放を行う
     */
    virtual void dispose();
    static jc_sp<SpriteManager> createInstance( MDevice device );
};

/**
 * managed
 */
typedef jc_sp<SpriteManager> MSpriteManager;

}
}

#endif /* JCGLSPRITEMANAGER_H_ */
