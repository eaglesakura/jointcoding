/*
 * JointApplicationBase.h
 *
 *  Created on: 2013/05/06
 */

#ifndef JOINTAPPLICATIONBASE_H_
#define JOINTAPPLICATIONBASE_H_

#include    "jointcoding.h"
#include    "jc/collection/BitFlags.hpp"
#include    "jc/widget/window/WindowManager.h"
#include    "jc/gl/GL2D.h"
#include    "protocol/jcJointApplicationProtocol.h"

namespace jc {
namespace gl {

using namespace jc::view;

/**
 * サーフェイススペックの拡張機能
 */
enum SurfaceSupecExtension_e {
    /**
     * レンダリングにTextureViewを利用する。
     * デフォルトではSurfaceViewが利用される。
     * for Android.
     */
    SurfaceSupecExtension_AndroidTextureView,

    /**
     * num flags...
     */
    SurfaceSupecExtension_Num,
};

/**
 * レンダリングサーフェイスの初期化内容をリクエストする
 */
struct SurfaceSpecs {
    /**
     * サーフェイスのピクセルフォーマット
     * 非対応の場合はRGBA8が選択される
     */
    PixelFormat_e surfaceFormat;

    /**
     * 深度バッファを生成する場合はtrue
     */
    jcboolean hasDepth;

    /**
     * ステンシルバッファを生成する場合はtrue
     */
    jcboolean hasStencil;

    /**
     * 拡張機能の有無設定
     */
    BitFlags<SurfaceSupecExtension_Num> extensions;

    SurfaceSpecs() {
        // サーフェイスはRGB888
        // アルファを要求した場合、レンダリング処理の負荷が増える場合がある
        surfaceFormat = PixelFormat_RGB888;

        // 深度とステンシルはデフォルトON
        // 但し、両者はパッキングされて１バッファにまとめられる可能性がある(iOS)
        // その場合は強制的に両方同時生成される
        hasDepth = hasStencil = jctrue;
    }
};

/**
 * 状態の問い合わせを行うキー情報
 */
struct ApplicationQueryKey {
    /**
     * 主キー
     */
    s32 main_key;

    /**
     * サブキー
     * 負荷状態を持たせたい場合に利用する
     */
    s32 sub_key;

    ApplicationQueryKey() {
        main_key = sub_key = 0;
    }

    ApplicationQueryKey(s32 main_key, s32 sub_key) {
        this->main_key = main_key;
        this->sub_key = sub_key;
    }
};

/**
 * マルチプラットフォーム共通アプリのベースクラス
 */
class JointApplicationBase: public Object, public WindowEventHandler {
    /**
     * アプリのステート
     */
    s32 appState;

    /**
     * ステートの変更リクエスト
     */
    s32 pendingState;

    struct {
        /**
         * 初期化済みであればtrue
         */
        jcboolean initialized;

        /**
         * 廃棄済みであればtrue
         */
        jcboolean destroyed;
    } flags;

protected:
    /**
     * レンダリングデバイス
     */
    MDevice device;

    /**
     * 問い合わせ・書き込み操作のためのミューテックス
     */
    mutable Mutex query_mutex;

    /**
     * ウィンドウクラス
     */
    MWindowManager windowManager;

    /**
     * 最後にメインループを追加した時のサーフェイスサイズ
     */
    Vector2i checkedSurfaceSize;

    /**
     * メッセージで送られた実際のサーフェイスサイズ
     */
    Vector2i surfaceSize;
public:
    JointApplicationBase();

    virtual ~JointApplicationBase() {
    }

public:
    /* 初期化情報 */

    /**
     * サーフェイスの初期化に必要なスペックのリクエストを受け取る。
     * 可能な限りリクエストに沿ったサーフェイスを作成する。
     */
    virtual SurfaceSpecs getRenderingSurfaceSpecs() const {
        return SurfaceSpecs();
    }

public:
    /* アクセサ */

    /**
     * ステータスの問い合わせを行う
     * Native系との簡単なやり取りに利用する。
     * ちょっとしたパラメータやりとりのためにメソッドを追加するコストを避ける
     */
    virtual jcboolean queryParams(const ApplicationQueryKey *key, s32 *result, const s32 result_rength) const;

    /**
     * ステータスの書き込みを行う
     * Native系との簡単なやり取りに利用する。
     * ちょっとしたパラメータやりとりのためにメソッドを追加するコストを避ける
     */
    virtual jcboolean postParams(const ApplicationQueryKey *key, const s32 *params, const s32 params_length);

    /**
     * 現在の実行状態を取得する
     */
    virtual s32 getRunningState() const {
        return appState;
    }

    /**
     * 初期化中の場合はtrue
     */
    virtual jcboolean isStateInitializing() const {
        return (!flags.initialized) || appState == JointApplicationProtocol::State_Initializing;
    }

    /**
     * ループ実行中の場合はtrue
     */
    virtual jcboolean isStateRunning() const {
        return appState == JointApplicationProtocol::State_Running;
    }

    /**
     * 休止中の場合はtrue
     */
    virtual jcboolean isStatePaused() const {
        return appState == JointApplicationProtocol::State_Paused;
    }

    /**
     * 廃棄済みの場合はtrue
     */
    virtual jcboolean isStateDestroyed() const {
        return appState == JointApplicationProtocol::State_Destroyed;
    }

    /**
     * レンダリングデバイスを取得する
     */
    virtual MDevice getDevice() const {
        return device;
    }

    /**
     * ウィンドウを取得する
     */
    virtual MWindowManager getWindowManager() const {
        return windowManager;
    }

    /**
     * コンテキストを取得する
     */
    virtual MWindowContext getWindowContext() const {
        return windowManager->getWindowContext();
    }

    /**
     * 現在のカレントステートを取得する
     */
    virtual MGLState getState() const {
        return device->getState();
    }

    /**
     * VRAMを取得する
     */
    virtual VRAM getVRAM() const {
        return device->getVRAM();
    }

    /**
     * ウィンドウサイズを取得する
     */
    virtual Vector2i getWindowSize() const {
        return device->getSurfaceArea().wh();
    }

    /**
     * スプライト管理クラスを取得する
     */
    virtual MSpriteManager getSpriteManager() const {
        return windowManager->getWindowContext()->getSpriteManager();
    }

protected:
    /* フレームワークライフサイクル */

    /**
     * アプリ初期化を行わせる
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppInitialize() = 0;

    /**
     * アプリのサーフェイスリサイズを行わせる
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppSurfaceResized(const s32 width, const s32 height) = 0;

    /**
     * メインループを行わせる
     * メソッド呼び出し時点でデバイスロック済み
     */
    virtual void onAppMainUpdate() = 0;

    /**
     * メインレンダリングを行わせる。
     * メソッド呼び出し時点でデバイスロック済み。
     * フロントバッファ転送もメソッド内で行わせる。
     */
    virtual void onAppMainRendering() = 0;

    /**
     * アプリのメモリ解放を行う
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppDestroy() = 0;

    /**
     * アプリ休止処理
     * 音声の休止等の処理を行う
     */
    virtual void onAppPause() = 0;

    /**
     * アプリの再開処理
     * 音声の再開等の処理を行う
     */
    virtual void onAppResume() = 0;

    /**
     * アプリのステートが変更された
     */
    virtual void onAppStateChanged(const s32 oldState, const s32 newState) {
    }

public:
    /* アプリライフサイクル */

    /**
     * サーフェイスが作成された
     */
    virtual void dispatchSurfaceCreated(MDevice device);

    /**
     * アプリケーションの休止処理を行う
     * Activityの休止等
     */
    virtual void dispatchPause();

    /**
     * アプリケーションの再開処理を行う
     * Activity再開等
     */
    virtual void dispatchResume();

    /**
     * サーフェイスのリサイズが行われた
     */
    virtual void dispatchSurfaceResized(const s32 newWidth, const s32 newHeight);

    /**
     * サーフェイスが廃棄された
     */
    virtual void dispatchDestroy();

    /**
     * メインループの外部呼び出しを行う
     */
    virtual void dispatchMainLoop();
protected:
    /**
     * 初期化処理を行う
     */
    virtual void dispatchInitialize();

};

}
}

#endif /* JOINTAPPLICATIONBASE_H_ */
