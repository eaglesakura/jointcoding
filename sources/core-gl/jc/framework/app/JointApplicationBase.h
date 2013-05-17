/*
 * JointApplicationBase.h
 *
 *  Created on: 2013/05/06
 */

#ifndef JOINTAPPLICATIONBASE_H_
#define JOINTAPPLICATIONBASE_H_

#include    "jointcoding.h"
#include    "jc/collection/BitFlags.hpp"
#include    "jc/platform/Platform.h"
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
 * 何らかのエラーが生じた時にどのようなアクションを取らせるかを確認する
 */
enum ApplicationRestoreStatus_e {
    /**
     * 復旧が可能なため、実行を継続する
     */
    ApplicationRestoreStatus_Running,

    /**
     * 復旧を諦めてアプリを終了する
     */
    ApplicationRestoreStatus_Abort,
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

    ApplicationQueryKey(s32 main_key, s32 sub_key = 0) {
        this->main_key = main_key;
        this->sub_key = sub_key;
    }
};

/**
 * アプリのタスクを定義する
 */
struct ApplicationTaskContext {
    /**
     * タスクの一意のID
     */
    s32 uniqueId;

    /**
     * ユーザーが自由に使える領域
     */
    s32 user_data;

    /**
     * スレッド識別用のID
     */
    ThreadID threadId;

    ApplicationTaskContext() {
        uniqueId = 0;
        user_data = 0;
    }
};

/**
 * プラットフォームへのアクセスが必要な場合に必要なコンテキスト
 * 各プラットフォームで継承して利用するため、アプリが直接触ることは無い。
 */
class PlatformContext: public Object {
protected:
    /**
     * 実行されているプラットフォーム
     */
    Platform_e platform;

    /**
     * 描画先のウィンドウ
     */
    MDevice windowDevice;

    PlatformContext(Platform_e platform) {
        this->platform = platform;
    }
public:
    virtual ~PlatformContext() {
    }

    /**
     * 実行中のプラットフォームを取得する
     */
    virtual Platform_e getPlatform() const {
        return platform;
    }

    /**
     * 描画用のウィンドウデバイスを取得する
     */
    virtual MDevice getWindowDevice() const {
        return windowDevice;
    }

    /**
     * スレイブ操作用デバイスを生成する。
     * ウィンドウデバイスとSharedContextになっており、例えば別スレッドでのテクスチャ読込等を行うために利用する。
     *
     * ----
     * for Android.
     * 内部的には1x1のPBufferSurfaceを保持しているが、描画には適さない。
     * ----
     * for iOS
     * 内部的には空のEGLSurfaceProtocolを保持し、bind()してもコンテキストを変更しない。
     */
    virtual MDevice createSlaveDevice() const = 0;

    /**
     * 新規タスクを生成する
     * タスクはJointApplicationBase::dispatchNewtask()をコールし、その中で処理を行う
     */
    virtual void startNewtask(const s32 uniqueId, const s32 user_data) = 0;
};

/**
 * managed
 */
typedef jc_sp<PlatformContext> MPlatformContext;

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
     * バインドされているプラットフォーム情報
     */
    MPlatformContext platformContext;

    /**
     * 問い合わせ・書き込み操作のためのミューテックス
     */
    Mutex query_mutex;

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

    virtual ~JointApplicationBase();
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
     * 保留ステートを持っているならtrue
     */
    virtual s32 hasPendingState() const {
        return pendingState >= 0;
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
     * ウィンドウ描画用デバイスを取得する
     */
    virtual MDevice getWindowDevice() const {
        assert(platformContext);
        return platformContext->getWindowDevice();
    }

    /**
     * 新規の非同期タスク（スレッド）を生成する。
     * 新規のスレッドからdispatchNewtask()に引数の値が渡され、JointApplicationBaseと継承したクラスはuniqueIdの値を元に処理を分岐させる。
     *
     * JointApplicationProtocol#SystemTask_XXXX系のuniqueIdは予約されている。
     * システムによって予約されているuniqueIdは利用時に注意すること。
     */
    virtual void startNewtask(const s32 uniqueId, const s32 user_data) {
        assert(platformContext);
        platformContext->startNewtask(uniqueId, user_data);
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
     * ウィンドウサイズを取得する
     */
    virtual Vector2i getWindowSize() const {
        assert(platformContext);
        return getWindowDevice()->getSurfaceArea().wh();
    }

    /**
     * スプライト管理クラスを取得する
     */
    virtual MSpriteManager getSpriteManager() const {
        assert(windowManager);
        return windowManager->getWindowContext()->getSpriteManager();
    }

protected:
    /* フレームワークライフサイクル */

    /**
     * プラットフォームとの接続が完了した
     */
    virtual void onBindPlatform() {

    }

    /**
     * メインループでアプリ初期化を行わせる
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppInitialize() = 0;

    /**
     * メインループでアプリのサーフェイスリサイズを行わせる
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppSurfaceResized(const s32 width, const s32 height) = 0;

    /**
     * メインループ更新を行わせる
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
     * 新規タスクの実行をリクエストされた
     * ハンドル出来るタスクならtrueを返す
     * ハンドル出来なかったらfalseを返す
     */
    virtual jcboolean onAppTask(const ApplicationTaskContext &task) = 0;

    /**
     * アプリのステートが変更された
     */
    virtual void onAppStateChanged(const s32 oldState, const s32 newState) {
    }

    /**
     * エラーハンドリングを行う
     */
    virtual ApplicationRestoreStatus_e handleError(const EGLException_e exception) {
        switch (exception) {
            // 復旧が不可能なエラー一覧
            case EGLException_OutOfMemory:
            case EGLException_InitializeFailed:
                return ApplicationRestoreStatus_Abort;
            default:
                break;
        }

        return ApplicationRestoreStatus_Running;
    }

public:

    /**
     * プラットフォームの接続を行う
     * このメソッド呼び出し後、ウィンドウ描画やプラットフォーム固有のAPIを利用できる
     */
    virtual void dispatchBindPlatform(const MPlatformContext context);

    /**
     * 新規タスクを実行する。
     * 実行される度に新たなスレッドとして呼び出される。
     */
    virtual void dispatchTask(const ApplicationTaskContext &task);

protected:
    /* アプリライフサイクル */

    /**
     * レンダリングの休止を求める場合、trueを返す
     */
    virtual jcboolean isLoopSleep() const;

    /**
     * アプリの実行ステートを変更する
     */
    virtual void changeAppState();

    /**
     * メインループの呼び出しを行う
     * SystemTask_Mainloopによって開始される。
     * メインループは現在の状態を管理し、シンプルな状態遷移を行う
     *
     * 1. 初期化
     * 2. メインループ -> 休止 || リサイズ -> メインループ
     * 3. 休止 -> 復帰 -> メインループ
     * 4. メインループ -> 廃棄ß
     */
    virtual void dispatchMainloop();

    /**
     * 初期化処理を行う
     */
    virtual void dispatchInitialize();

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
    virtual void dispatchSurfaceResized();

    /**
     * サーフェイスが廃棄された
     */
    virtual void dispatchDestroy();

    /**
     * ステート変更リクエストが送られた
     */
    virtual jcboolean dispatchOnStateChangeRequest(const ApplicationQueryKey *key, const s32 *params, const s32 params_length);
};

}
}

#endif /* JOINTAPPLICATIONBASE_H_ */
