/*
 * JointApplicationBase.cpp
 *
 *  Created on: 2013/05/06
 */

#include    "jc/framework/app/JointApplicationBase.h"
#include    "protocol/jcSurfacePixelFormatProtocol.h"

namespace jc {
namespace gl {

JointApplicationBase::JointApplicationBase() {
    appState = JointApplicationProtocol::State_Initializing;
    // 保留ステートを無効化する
    pendingState = -1;

    fragmentController.reset(mark_new ApplicationFragmentController(this));
}

JointApplicationBase::~JointApplicationBase() {
    // 明示的にプラットフォームを廃棄する
    platformContext.reset();
}

/**
 * ステータスの問い合わせを行う
 */
jcboolean JointApplicationBase::dispatchQueryParams(const ApplicationQueryKey *key, s32 *result, const s32 result_length) {
    assert(key);

    if (key->main_key == JointApplicationProtocol::PostKey_QueryApplicationState) {
        assert(result && result_length >= 1);
        MutexLock lock(query_mutex);

        *result = getRunningState();
        return jctrue;
    } else if (key->main_key == JointApplicationProtocol::PostKey_RequestSurfaceSpecs) {
        assert(result && result_length >= JointApplicationProtocol::QueryKey_RequestSurfaceSpecs_length);

        // サーフェイス性能を問い合わせる
        SurfaceSpecs specs = getRenderingSurfaceSpecs();

        s32 index = 0;
        {
            // フォーマット
            switch (specs.surfaceFormat) {
                case PixelFormat_RGB888:
                    result[index] = SurfacePixelFormatProtocol::RGB8;
                    break;

                case PixelFormat_RGBA8888:
                    result[index] = SurfacePixelFormatProtocol::RGBA8;
                    break;
                default:
                    jcalertf("unsupported pixel format(%d)", specs.surfaceFormat)
                    ;
                    assert(false);
                    break;
            }
            ++index;
        }
        {
            // depth
            result[index] = specs.hasDepth;
            ++index;
        }
        {
            // ステンシルバッファ
            result[index] = specs.hasStencil;
            ++index;
        }
        {
            // TextureView request
            // for Android
            result[index] = specs.extensions.isEnable(SurfaceSupecExtension_AndroidTextureView);
            ++index;
        }

        // 正しくリクエストを書き込んでいることを検証する
        assert(index == JointApplicationProtocol::QueryKey_RequestSurfaceSpecs_length);
    }

    jclogf("drop query main(%d) sub(%d)", key->main_key, key->sub_key);
    return jcfalse;
}

/**
 * ステータスの問い合わせを行う
 * Native系との簡単なやり取りに利用する
 */
jcboolean JointApplicationBase::dispatchQueryParams(const ApplicationQueryKey *key, String *result, const s32 result_rength) {
    jclogf("drop query main(%d) sub(%d)", key->main_key, key->sub_key);
    return jcfalse;
}

/**
 * ステータスの書き込みを行う
 * Native系との簡単なやり取りに利用する
 */
jcboolean JointApplicationBase::dispatchReceiveParams(const ApplicationQueryKey *key, string_params &params) {
    assert(key);

    const s32 mainKey = key->main_key;
    if (mainKey == JointApplicationProtocol::PostKey_QueryApplicationState) {
        assert(params && params.length);
        MutexLock lock(query_mutex);

        params[0] = String::valueOfInt(getRunningState());
        return jctrue;
    } else if (mainKey == JointApplicationProtocol::PostKey_RequestSurfaceSpecs) {
        assert(params && params.length >= JointApplicationProtocol::QueryKey_RequestSurfaceSpecs_length);

        // サーフェイス性能を問い合わせる
        SurfaceSpecs specs = getRenderingSurfaceSpecs();

        s32 index = 0;
        {
            // フォーマット
            switch (specs.surfaceFormat) {
                case PixelFormat_RGB888:
                    params[index] = String::valueOfInt(SurfacePixelFormatProtocol::RGB8);
                    break;

                case PixelFormat_RGBA8888:
                    params[index] = String::valueOfInt(SurfacePixelFormatProtocol::RGBA8);
                    break;
                default:
                    jcalertf("unsupported pixel format(%d)", specs.surfaceFormat)
                    ;
                    assert(false);
                    break;
            }
            ++index;
        }
        {
            // depth
            params[index] = String::valueOfBoolean(specs.hasDepth);
            ++index;
        }
        {
            // ステンシルバッファ
            params[index] = String::valueOfBoolean(specs.hasStencil);
            ++index;
        }
        {
            // TextureView request
            // for Android
            params[index] = String::valueOfBoolean(specs.extensions.isEnable(SurfaceSupecExtension_AndroidTextureView));
            ++index;
        }

        // 正しくリクエストを書き込んでいることを検証する
        assert(index == JointApplicationProtocol::QueryKey_RequestSurfaceSpecs_length);

        return jctrue;
    } else if (mainKey == JointApplicationProtocol::PostKey_SurfaceSize) {
        // サーフェイスサイズ変更のリクエストを送る
        assert(params && params.length >= 2);
        MutexLock lock(query_mutex);

        surfaceSize.x = params[0].toInteger();
        surfaceSize.y = params[1].toInteger();
        return jctrue;
    } else if (mainKey == JointApplicationProtocol::PostKey_StateRequest) {
        assert(params.length);
        jclogf("state request(%s)(%d)", params[0].c_str(), params[0].toInteger());
        return dispatchOnStateChangeRequest(params[0].toInteger());
    }
    jclogf("drop receive main(%d) sub(%d)", key->main_key, key->sub_key);

    return jcfalse;
}

/**
 * プラットフォームの接続を行う
 */
void JointApplicationBase::dispatchBindPlatform(const MPlatformContext context) {
    this->platformContext = context;

    // メインループタスクを開始する
    startNewtask(JointApplicationProtocol::SystemTask_Mainloop, 0);

    // 実行開始の予約を行う
    dispatchOnStateChangeRequest(JointApplicationProtocol::State_Running);
}

/**
 * 新規タスクを実行する。
 * 実行される度に新たなスレッドとして呼び出される。
 */
void JointApplicationBase::dispatchNewTask(const ApplicationTaskContext &task) {
    jclogf("start uid(0x%x) ud(0x%x) task(%s)", task.uniqueId, task.user_data, task.threadId.toString().c_str());

    // タスク数をインクリメント
    {
        MutexLock lock(tasks.mutex);
        ++tasks.num;
    }

    // システムが予約したタスク
    if (task.uniqueId == JointApplicationProtocol::SystemTask_Mainloop) {
        // メインループを実行させる
        dispatchMainloop();
    } else {
        // その他のタスクのハンドルはアプリに任せる
        if (!onAppTask(task)) {
            // ハンドリングに失敗した
            jcalertf("task handle error uid(0x%x) ud(0x%x) task(%s)", task.uniqueId, task.user_data, task.threadId.toString().c_str());
        }
    }

    // タスク数をデクリメント
    {
        MutexLock lock(tasks.mutex);
        --tasks.num;
        // タスク数が0になったらクリーンアップを行わせる
        if(tasks.num == 0) {
            jclogf("task all killed :: last task uid(%d)", task.uniqueId);
            onTaskDestroyed(task);
        }
    }

    jclogf("finish uid(0x%x) ud(0x%x) task(%s)", task.uniqueId, task.user_data, task.threadId.toString().c_str());
}

/**
 * アプリの実行ステートを変更する
 */
void JointApplicationBase::changeAppState() {
    if (!hasPendingState()) {
        return;
    }

    {
        // ステート変更ロックをかける
        MutexLock lock(query_mutex);

        if (pendingState == appState) {
            // 保留ステートをリセットして終了
            pendingState = -1;
            return;
        }
    }

    // 古いステートをチェックする
    const s32 oldState = appState;
    this->appState = pendingState;
    this->pendingState = -1;
    assert(oldState != appState);

    // ステート変更を通知する
    onAppStateChanged(oldState, appState);

    jclogf("change state(%d) -> (%d)", oldState, appState);

    // レジューム
    if (appState == JointApplicationProtocol::State_Running) {
        if (flags.isDisable(ApplicationStateFlag_Initialized)) {
            jclog("start initialize");
            // 未初期化だから初期化する
            dispatchInitialize();

        }
        jclog("start resume");
        // レジュームする
        dispatchResume();
    } else if (appState == JointApplicationProtocol::State_Paused) {
        if (flags.isEnable(ApplicationStateFlag_Initialized)) {
            // 休止を行う
            dispatchPause();
        }
    } else if (appState == JointApplicationProtocol::State_Destroyed) {
        // 休止を行う
        dispatchPause();

        // 廃棄処理を行う
        dispatchDestroy();
    }
}

/**
 * サーフェイスのリサイズが行われた
 */
void JointApplicationBase::dispatchSurfaceResized() {
    // 書込み中の可能性があるため、ロックする
    MutexLock lock(query_mutex);

    checkedSurfaceSize = surfaceSize;
    jclogf("Resized Surface(%dx%d)", surfaceSize.x, surfaceSize.y);

    // レンダリングコンテキストにリサイズを伝える
    renderingContext->onPlatformViewResized(surfaceSize.x, surfaceSize.y);

    // アプリに処理を任せる
    onAppSurfaceResized(surfaceSize.x, surfaceSize.y);
    fragmentController->onSurfaceResized(surfaceSize.x, surfaceSize.y);
}

/**
 * サーフェイスが廃棄された
 */
void JointApplicationBase::dispatchDestroy() {

    flags.enable(ApplicationStateFlag_Destroyed);
    if (flags.isDisable(ApplicationStateFlag_Initialized)) {
        // 初期化前であれば何もしない
        return;
    }

    {
        // アプリの解放を行う
        onAppDestroy();
        fragmentController->onDestroy();
    }

    // 各種解放処理を行う
    {
        renderingContext.reset();
    }
}

/**
 * 初期化処理を行う
 */
void JointApplicationBase::dispatchInitialize() {
    // レンダリングコンテキストを生成する
    {
        renderingContext.reset(mark_new RenderingContext());
        renderingContext->initialize(getWindowDevice());
    }

    // アプリの初期化を行わせる
    onAppInitialize();

    flags.enable(ApplicationStateFlag_Initialized);
}

/**
 * ステート変更リクエストが送られた
 */
jcboolean JointApplicationBase::dispatchOnStateChangeRequest(const s32 nextState) {
    // ステート変更のリクエストを送る
    MutexLock lock(query_mutex);

    if (isStateDestroyed() || pendingState == JointApplicationProtocol::State_Destroyed) {
        // 既に廃棄リクエストが送られているならコレ以上何も送る必要はない
        return jctrue;
    }

    // 保留ステートに上書きする
    pendingState = nextState;

    return jctrue;
}

/**
 * アプリケーションの休止処理を行う
 * Activityの休止等
 */
void JointApplicationBase::dispatchPause() {
    onAppPause();
    fragmentController->onPause();
}

/**
 * アプリケーションの再開処理を行う
 * Activity再開等
 */
void JointApplicationBase::dispatchResume() {
    onAppResume();
    fragmentController->onResume();
}

/**
 * レンダリングの休止を求める場合、trueを返す
 */
jcboolean JointApplicationBase::isLoopSleep() const {
    if (!getWindowDevice()->lockEnable()) {
        // ロックが不可能な状態だったらスリープを継続させる
        return jctrue;
    }

    if (hasPendingState()) {
        // pendingされたステートがあるなら実行を行う
        return jcfalse;
    }

    if (isStatePaused() || isStateInitializing()) {
        // ステートが休止中か初期化中ならループを行わない
        return jctrue;
    }

    // それ以外ならば実行
    return jcfalse;
}

/**
 * タッチイベントが呼び出された
 */
void JointApplicationBase::dispatchTouchEvent(jc_sp<TouchEventProtocol> event) {
    jclogf("dispatchTouchEvent type(%d) pos(%f, %f)", event->getEventType(), event->getEventPosX(), event->getEventPosY());
}

/**
 * メインループの外部呼び出しを行う
 */
void JointApplicationBase::dispatchMainloop() {
    while (!isStateDestroyed()) {

        // 休止中ならスリープをかける
        if (isLoopSleep()) {
            Thread::sleep(10);
        } else {
            // 廃棄されるまでループする
            try {
                DeviceLock lock(getWindowDevice(), jctrue);

                // ステートを変更する
                changeAppState();

                // メインループ中
                if (isStateRunning()) {
                    // サーフェイスサイズチェックを行う
                    if (checkedSurfaceSize != surfaceSize) {
                        dispatchSurfaceResized();
                    }

                    // フレーム定形処理を行う
                    onAppMainUpdate();
                    // フラグメント処理
                    {
                        fragmentController->commit();
                        fragmentController->onUpdate();
                    }
                }
            } catch (EGLException &e) {
                jcloge(e);
                if (handleError((EGLException_e) e.getType()) == ApplicationRestoreStatus_Abort) {
                    // レンダリングループを抜ける
                    jcalert("Abort Main Loop");
                    break;
                }

                Thread::sleep(10);
            }
        }
//        jclogf("main loop(%d)", getRunningState());
    }

    jclog("main loop finish");
}

}
}

