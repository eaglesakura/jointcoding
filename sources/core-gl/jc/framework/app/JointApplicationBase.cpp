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
        DeviceLock  lock(device, jctrue);

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

    if(!device->valid()) {
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
            checkedSurfaceSize = surfaceSize;

            // ビューポート更新
            getState()->viewport(0, 0, surfaceSize.x, surfaceSize.y);

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
    } catch (Exception &e) {
        jcloge(e);
    }
}

}
}

