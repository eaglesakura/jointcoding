/*
 * esBenchmarkApplication.h
 *
 *  Created on: 2013/05/06
 */

#ifndef ESBENCHMARKAPPLICATION_H_
#define ESBENCHMARKAPPLICATION_H_

#include    "benchmark.hpp"
#include    "jc/gl/render/RenderBufferObject.hpp"
#include    "jc/gl/render/FrameBufferObject.hpp"

namespace es {

class BenchmarkApplication: public JointApplicationBase, public RenderingContextListener {
    MTextureImage texture;

    enum {
        /**
         * テクスチャ読込を行わせるタスク
         */
        BenchmarkTask_LoadTexture,
    };

    /**
     * 3Dレンダリングフィギュア
     */
    jc_sp<fw::Figure> susanow;

    /**
     * スキニングフィギュア
     */
    jc_sp<fw::Figure> antan;

    float rotate;

    /**
     * 環境
     */
    MEnvironmentInstanceState worldEnv;

    /**
     * フィギュア用のレンダラ
     */
    jc_sp<BasicFigureRenderer> renderer;

    MGLShaderProgram basicShader;

    /**
     * シャドウマップ用のレンダラ
     */
    jc_sp<ShadowmapRenderer> shadowRenderer;

    /**
     * フィギュア情報
     */
    MFigureInstanceState susanow_instance;

    /**
     * フィギュア情報
     */
    MFigureInstanceState antan_instance;

    /**
     * シャドウマップ
     */
    MFrameBufferObject shadowmap;

    MSpriteManager spriteManager;

    /**
     * シャドウマップ用テクスチャ
     */
    MTextureImage shadowmapTexture;

    MWindowManager windowManager;
public:
    BenchmarkApplication();
    virtual ~BenchmarkApplication();

    virtual void loadTexture(MDevice subDevice);

protected:

    /**
     * サーフェイスの初期化に必要なスペックのリクエストを受け取る。
     * 可能な限りリクエストに沿ったサーフェイスを作成する。
     */
    virtual SurfaceSpecs getRenderingSurfaceSpecs() const;

    /**
     * レンダリングサーフェイスが変更された
     */
    virtual void onRenderingSurfaceChanged(RenderingContext *pContext, const MRenderingSurface old, const MRenderingSurface now);
protected:
    /* アプリライフサイクル */

    /**
     * アプリ初期化を行わせる
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppInitialize();

    /**
     * アプリのサーフェイスリサイズを行わせる
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppSurfaceResized(const s32 width, const s32 height);

    /**
     * メインループを行わせる
     * メソッド呼び出し時点でデバイスロック済み
     */
    virtual void onAppMainUpdate();

    /**
     * アプリ休止処理
     * 音声の休止等の処理を行う
     */
    virtual void onAppPause();

    /**
     * アプリの再開処理
     * 音声の再開等の処理を行う
     */
    virtual void onAppResume();

    /**
     * アプリのメモリ解放を行う
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppDestroy();

    /**
     * 新規タスクの実行をリクエストした
     */
    virtual jcboolean onAppTask(const ApplicationTaskContext &task);

};

}

#endif /* ESBENCHMARKAPPLICATION_H_ */
