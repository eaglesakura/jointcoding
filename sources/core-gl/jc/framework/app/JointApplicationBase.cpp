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
    flags.initialized = flags.destroyed = jcfalse;
    appState = JointApplicationProtocol::State_Initializing;
    // 保留ステートを無効化する
    pendingState = -1;
}

JointApplicationBase::~JointApplicationBase() {
    if (device) {
        device->dispose();
        device.reset();
    }
}

/**
 * ステータスの問い合わせを行う
 */
jcboolean JointApplicationBase::queryParams(const ApplicationQueryKey *key, s32 *result, const s32 result_length) const {
    assert(key);

    if (key->main_key == JointApplicationProtocol::QueryKey_ApplicationState) {
        assert(result && result_length >= 1);
        MutexLock lock(query_mutex);

        *result = getRunningState();
        return jctrue;
    } else if (key->main_key == JointApplicationProtocol::QueryKey_RequestSurfaceSpecs) {
        assert(result && result_length >= 4);

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
    }

    jclogf("drop query main(%d) sub(%d)", key->main_key, key->sub_key);
    return jcfalse;
}

/**
 * ステータスの書き込みを行う
 */
jcboolean JointApplicationBase::postParams(const ApplicationQueryKey *key, const s32 *params, const s32 params_length) {
    assert(key);
    assert(params);

    if (key->main_key == JointApplicationProtocol::PostKey_SurfaceSize) {
        // サーフェイスサイズ変更のリクエストを送る
        assert(params && params_length >= 2);

        MutexLock lock(query_mutex);

        surfaceSize.x = params[0];
        surfaceSize.y = params[1];
        return jctrue;
    } else if (key->main_key == JointApplicationProtocol::PostKey_StateRequest) {
        return dispatchOnStateChangeRequest(key, params, params_length);
    }

    jclogf("drop post main(%d) sub(%d)", key->main_key, key->sub_key);
    return jcfalse;
}

/**
 * プラットフォームの接続を行う
 */
void JointApplicationBase::dispatchBindPlatform(const MPlatformContext context) {
    this->platformContext = context;
}

/**
 * 新規タスクを実行する。
 * 実行される度に新たなスレッドとして呼び出される。
 */
void JointApplicationBase::dispatchTask(const ApplicationTaskContext &task) {
    jclogf("start uid(0x%x) ud(0x%x) task(%s)", task.uniqueId, task.user_data, task.threadId.toString().c_str());
    if (task.uniqueId == JointApplicationProtocol::SystemTask_Mainloop) {
        // システムがデフォルトで用意したタスクを選別する
        dispatchMainloop();
    } else {
        // FIXME その他のタスクのハンドルはアプリに任せる
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

    // デバイスの初期設定が済んでいないなら何もしない
    if (!device) {
        return;
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
        if (!flags.initialized) {
            // 未初期化だから初期化する
            dispatchInitialize();
        } else {
            // 初期化済みだからレジュームする
            dispatchResume();
        }
    } else if (appState == JointApplicationProtocol::State_Paused) {
        if (flags.initialized) {
            // 休止を行う
            dispatchPause();
        }
    } else if (appState == JointApplicationProtocol::State_Destroyed) {
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

    // ビューポート更新
    device->getState()->viewport(0, 0, surfaceSize.x, surfaceSize.y);

    // スプライトマネージャーアスペクト比更新
    getSpriteManager()->setSurfaceAspect(surfaceSize.x, surfaceSize.y);

    // アプリに処理を任せる
    onAppSurfaceResized(surfaceSize.x, surfaceSize.y);
}

/**
 * サーフェイスが廃棄された
 */
void JointApplicationBase::dispatchDestroy() {

    flags.destroyed = jctrue;
    if (!flags.initialized) {
        // 初期化前であれば何もしない
        return;
    }

    {
        // アプリの解放を行う
        onAppDestroy();
    }

    // 各種解放処理を行う
    {
        windowManager.reset();

        {
            MDevice windowDevice = getDevice();
            windowDevice->getVRAM()->gc();
        }
    }
}

/**
 * 初期化処理を行う
 */
void JointApplicationBase::dispatchInitialize() {
// ウィンドウ生成
    {
        windowManager.reset(new WindowManager());
        // 30fps〜60fpsで扱う
        windowManager->setFramerateRange(30, 60);
        windowManager->setWindowEventHandler(this);
        windowManager->getWindowContext()->setSpriteManager(SpriteManager::createInstance(getWindowDevice()));
    }

    onAppInitialize();

    flags.initialized = jctrue;
}

/**
 * ステート変更リクエストが送られた
 */
jcboolean JointApplicationBase::dispatchOnStateChangeRequest(const ApplicationQueryKey *key, const s32 *params, const s32 params_length) {
    // ステート変更のリクエストを送る
    assert(params && params_length >= 1);

    MutexLock lock(query_mutex);

    if (isStateDestroyed() || pendingState == JointApplicationProtocol::State_Destroyed) {
        // 既に廃棄リクエストが送られているならコレ以上何も送る必要はない
        return jctrue;
    }

    // 保留ステートに上書きする
    pendingState = params[0];

    return jctrue;
}

/**
 * アプリケーションの休止処理を行う
 * Activityの休止等
 */
void JointApplicationBase::dispatchPause() {
    onAppPause();
}

/**
 * アプリケーションの再開処理を行う
 * Activity再開等
 */
void JointApplicationBase::dispatchResume() {
    onAppResume();
}

/**
 * レンダリングの休止を求める場合、trueを返す
 */
jcboolean JointApplicationBase::isLoopSleep() const {
    if (!device->lockEnable()) {
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
 * メインループの外部呼び出しを行う
 */
void JointApplicationBase::dispatchMainloop() {
    assert(device);

    while (!isStateDestroyed()) {

        // 休止中ならスリープをかける
        if (isLoopSleep()) {
            Thread::sleep(10);
        } else {
            // 廃棄されるまでループする
            try {
                DeviceLock lock(device, jctrue);

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
                    onAppMainRendering();
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

