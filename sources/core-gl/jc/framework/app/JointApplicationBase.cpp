/*
 * JointApplicationBase.cpp
 *
 *  Created on: 2013/05/06
 */

#include    "jc/framework/app/JointApplicationBase.h"

namespace jc {
namespace gl {

JointApplicationBase::JointApplicationBase() {
    flags.initialized = flags.destroyed = jcfalse;
    appState = JointApplicationProtocol::State_Initializing;
    // 保留ステートを無効化する
    pendingState = -1;
}

/**
 * ステータスの問い合わせを行う
 */
jcboolean JointApplicationBase::queryParams(const ApplicationQueryKey *key, s32 *result, const s32 result_length) const {
    assert(key);

    if (key->main_key == JointApplicationProtocol::QueryKey_ApplicationState) {
        *result = getRunningState();
        return jctrue;
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
        // ステート変更のリクエストを送る
        assert(params && params_length >= 1);

        MutexLock lock(query_mutex);

        if (pendingState == JointApplicationProtocol::State_Destroyed) {
            // 既に廃棄リクエストが送られているならコレ以上何も送る必要はない
            return jctrue;
        }

        const s32 request = params[0];

        // レジュームリクエスト
        if (request == JointApplicationProtocol::State_Resume) {
            // 休止が保留されていたらリセットしてそれ以上何もしない
            if (pendingState == JointApplicationProtocol::State_Paused) {
                pendingState = -1;
                return jctrue;
            }

            if (!isStateInitializing()) {
                // 初期化も終わってないならレジュームの必要ない
                return jctrue;
            }

            pendingState = request;
        }
    }

    jclogf("drop post main(%d) sub(%d)", key->main_key, key->sub_key);
    return jcfalse;
}

/**
 * サーフェイスが作成された
 */
void JointApplicationBase::dispatchSurfaceCreated(MDevice device) {
    this->device = device;
}

/**
 * サーフェイスのリサイズが行われた
 */
void JointApplicationBase::dispatchSurfaceResized(const s32 newWidth, const s32 newHeight) {
    surfaceSize.x = newWidth;
    surfaceSize.y = newHeight;
}

/**
 * サーフェイスが廃棄された
 */
void JointApplicationBase::dispatchDestroy() {

    flags.destroyed = jctrue;
    try {
        DeviceLock lock(device, jctrue);

        if (!flags.initialized) {
            // 初期化前であれば何もしない
            return;
        }

        {
            // アプリの解放を行う
            onAppDestroy();
        }

        windowManager.reset();

        getVRAM()->gc();
    } catch (Exception &e) {
        jcloge(e);
    }
    device.reset();
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
        windowManager->getWindowContext()->setSpriteManager(SpriteManager::createInstance(device));
    }

    onAppInitialize();
}

/**
 * アプリケーションの休止処理を行う
 * Activityの休止等
 */
void JointApplicationBase::dispatchPause() {
    if (!flags.initialized) {
        return;
    }

    try {
        DeviceLock lock(device, jctrue);

        onAppResume();
    } catch (Exception &e) {

        jcloge(e);
    }
}

/**
 * アプリケーションの再開処理を行う
 * Activity再開等
 */
void JointApplicationBase::dispatchResume() {
    if (!flags.initialized) {
        return;
    }

    try {
        DeviceLock lock(device, jctrue);

        onAppResume();
    } catch (Exception &e) {
        jcloge(e);
    }
}

/**
 * メインループの外部呼び出しを行う
 */
void JointApplicationBase::dispatchMainLoop() {
    if (!device || flags.destroyed) {
        // デバイスが無ければ開始できない
        return;
    }

    if (!device->valid()) {
        // デバイスの準備が整っていない
        return;
    }

// デバイスの待ち合わせを行う
    {
        device->waitLockRequest(1, &flags.destroyed);
        if (flags.destroyed) {
            return;
        }
    }

// 再度レンダリングチェックを行う
    if (!device->valid()) {
        // デバイスの準備が整っていない
        return;
    }

// 実処理を行う
    try {
        DeviceLock lock(device, jctrue);

        // 初期化が終わって無ければ初期化を行う
        if (!flags.initialized) {
            dispatchInitialize();
            flags.initialized = jctrue;
        }

        // サーフェイスサイズチェックを行う
        if (checkedSurfaceSize != surfaceSize) {
            // 書込み中の可能性があるため、ロックする
            MutexLock lock(query_mutex);

            checkedSurfaceSize = surfaceSize;
            jclogf("Resized Surface(%dx%d)", surfaceSize.x, surfaceSize.y);

            // ビューポート更新
            getState()->viewport(0, 0, surfaceSize.x, surfaceSize.y);

            // スプライトマネージャーアスペクト比更新
            getSpriteManager()->setSurfaceAspect(surfaceSize.x, surfaceSize.y);

            // アプリに処理を任せる
            onAppSurfaceResized(surfaceSize.x, surfaceSize.y);
        }

        // フレーム定形処理を行う
        {
            {
                onAppMainUpdate();
            }
            {
                onAppMainRendering();
            }
        }
    } catch (EGLException &e) {
        jcloge(e);
    }
}

}
}

