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
#include    "jc/gl/context/RenderingContext.hpp"
#include    "jc/system/StlAllocator.hpp"
#include    "jc/framework/fragment/ApplicationFragmentController.hpp"
#include    "jc/framework/app/IDataBank.hpp"

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
     * 追加状態を持たせたい場合に利用する
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

    /**
     * プラットフォームへ明示的に値を送信する。
     * プラットフォームで書き込まれた値はparamsへ反映される
     *
     * ハンドリングが行われなければfalseを返す
     */
    virtual jcboolean postParams(const s32 main_key, const s32 sub_key, unsafe_array<String> *params) = 0;

    /**
     * post/receive出来ないネイティブクラス等のIOに利用するデータバンク
     * add/popのみが行える。適度にキャストして利用する。
     */
    virtual MDataBank getDataBank() = 0;
};

/**
 * managed
 */
typedef jc_sp<PlatformContext> MPlatformContext;

/**
 * マルチプラットフォーム共通アプリのベースクラス
 */
class JointApplicationBase: public Object, public WindowEventHandler {
public:
    typedef unsafe_array<String> string_params;
protected:
    enum ApplicationStateFlag_e {
        /**
         * 初期化済みフラグ
         */
        ApplicationStateFlag_Initialized,

        /**
         * 廃棄済みフラグ
         */
        ApplicationStateFlag_Destroyed,

        /**
         * 数の管理フラグ
         */
        ApplicationStateFlag_Num,
    };
private:
    /**
     * アプリのステート
     */
    s32 appState;

    /**
     * ステートの変更リクエスト
     */
    s32 pendingState;

    BitFlags<ApplicationStateFlag_Num> flags;

    /**
     * 最後にメインループを追加した時のサーフェイスサイズ
     */
    Vector2i checkedSurfaceSize;

    /**
     * メッセージで送られた実際のサーフェイスサイズ
     */
    Vector2i surfaceSize;

    /**
     * フラグメント管理クラス
     */
    MApplicationFragmentController fragmentController;

    struct TaskInfo {
        /**
         * 起動しているタスク数
         */
        s32 num;

        /**
         * 操作用ミューテックス
         */
        jcmutex mutex;

        TaskInfo() {
            num = 0;
        }
    } tasks;
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
     * レンダリング用のコンテキスト
     */
    MRenderingContext renderingContext;
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
     * 現在起動しているタスク数を取得する
     * メインループも含めるため、基本的には１以上になる。
     */
    virtual s32 getTaskNum() const {
        return tasks.num;
    }

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
        return (flags.isDisable(ApplicationStateFlag_Initialized)) || appState == JointApplicationProtocol::State_Initializing;
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
     * マスターデバイスとなるため、Viewと関連づいた1つのみが取得できる。
     */
    virtual MDevice getWindowDevice() const {
        assert(platformContext);
        return platformContext->getWindowDevice();
    }

    /**
     * Platformへ明示的にパラメータを送信する。
     * Platformコードがtrueを返却した場合、paramsの内容にも反映が行われる。
     * paramsにNULLを渡した場合、Platform側にはString[0]が渡される
     */
    virtual jcboolean postParams(const s32 main_key, const s32 sub_key, string_params *params) {
        return platformContext->postParams(main_key, sub_key, params);
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
     * ウィンドウサイズを取得する
     * これはレンダリング対象のViewサイズと等価である。
     */
    virtual Vector2i getPlatformViewSize() const {
        return checkedSurfaceSize;
    }

    /**
     * レンダリング用コンテキストを取得する
     */
    virtual MRenderingContext getRenderingContext() const {
        assert(renderingContext);
        return renderingContext;
    }

    /**
     * フラグメント管理クラスを取得する
     */
    virtual MApplicationFragmentController getFragmentController() const {
        return fragmentController;
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
     * アプリのメモリ解放を行う
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppDestroy() = 0;

    /**
     * 全てのタスクが終了し、タスク数が0に達した場合に呼び出される。
     * Deviceに関連づいたリソースの解放を想定する。
     */
    virtual void onTaskDestroyed(const ApplicationTaskContext &lastTask) = 0;

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
     * Nativeとの連携
     */

    /**
     * プラットフォームの接続を行う
     * このメソッド呼び出し後、ウィンドウ描画やプラットフォーム固有のAPIを利用できる
     */
    virtual void dispatchBindPlatform(const MPlatformContext context);

    /**
     * 新規タスクを実行する。
     * 実行される度に新たなスレッドとして呼び出される。
     */
    virtual void dispatchNewTask(const ApplicationTaskContext &task);

    /**
     * タッチイベントが呼び出された
     */
    virtual void dispatchTouchEvent(jc_sp<TouchEventProtocol> event);

    /**
     * ステータスの問い合わせを行う
     * Native系との簡単なやり取りに利用する。
     * ちょっとしたパラメータやりとりのためにメソッドを追加するコストを避ける
     */
    virtual jcboolean dispatchQueryParams(const ApplicationQueryKey *key, s32 *result, const s32 result_rength);

    /**
     * ステータスの問い合わせを行う
     * Native系との簡単なやり取りに利用する
     */
    virtual jcboolean dispatchQueryParams(const ApplicationQueryKey *key, String *result, const s32 result_rength);

    /**
     * Platform（Java側、Objective-C側）からのパラメータ受け取りを行う
     * Native系との簡単なやり取りに利用する。
     * ちょっとしたパラメータやりとりのためにメソッドを追加するコストを避ける
     * paramsに書き込んだ値はPlatform側クラスに反映されて戻される
     */
    virtual jcboolean dispatchReceiveParams(const ApplicationQueryKey *key, string_params &params);

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
     * 4. メインループ -> 廃棄
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
    virtual jcboolean dispatchOnStateChangeRequest(const s32 nextState);
};

}
}

#endif /* JOINTAPPLICATIONBASE_H_ */
