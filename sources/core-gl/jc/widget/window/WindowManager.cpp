/*
 * WindowManager.cpp
 *
 *  Created on: 2013/04/11
 */

#include    "jc/widget/window/WindowManager.h"
#include    "jc/widget/event/TouchEvent.h"

namespace jc {
namespace view {

WindowManager::WindowManager() {
    events.reset(new EventQueue());
    windowContext.reset(new WindowContext());
    window.reset(new Window(windowContext));
    windowContext->setWindow(window);
    windowContext->setEventQueue(events);
    windowEventListener.reset(new WindowEventListener(windowContext));

    touchDetector.reset(new TouchDetector(windowEventListener));
    keyDetector.reset(new KeyDetector(windowEventListener));

    // 可変フレームレート範囲を設定
    windowContext->loopController.setFrameRateRange(30, 60);

    tick.lastTickTime = Timer::currentTime();
    tick.tickCallTime = -1.0;
}

WindowManager::~WindowManager() {

}

/**
 * イベントを追加する
 */
void WindowManager::addEvent(MEvent event) {
    assert(event.get() != NULL);
    // イベントを末尾に登録する
    events->pushBackEvent(event);
}
/**
 * タッチイベントを処理する
 */
void WindowManager::handleTouchEvent(MEvent event) {
    assert(event->getType() == EventType_Touch);
    MTouchEventExtension ext = event->getExtension<TouchEventExtension>();
    assert(ext.get() != NULL);

    jc_sp<TouchEventProtocol> touchEvent = ext->getPlatformEvent();
    assert(touchEvent.get() != NULL);

    // イベント通知
    touchDetector->onTouchEvent(touchEvent.get());
}

/**
 * キーイベントを処理する
 */
void WindowManager::handleKeyEvent(MEvent event) {
    assert(event->getType() == EventType_Key);

    MKeyEventExtension ext = event->getExtension<KeyEventExtension>();
    assert(ext.get() != NULL);

    jc_sp<KeyEventProtocol> keyEvent = ext->getPlatformEvent();
    assert(keyEvent.get() != NULL);

    // イベント通知
    keyDetector->onKeyEvent(keyEvent.get());
}

/**
 * フォーカスリクエストイベント
 */
void WindowManager::handleRequestFocusEvent(MEvent event) {
    assert(event->getType() == EventType_RequestFocus);

    MRequestFocusEventExtension ext = event->getExtension<RequestFocusEventExtension>();
    assert(ext.get() != NULL);
    assert(ext->target.get() != NULL);

    ext->target->dispatchRecuestFocus(ext->hasFocus);
}

/**
 * 個々のイベント対応を行う
 */
void WindowManager::dispatchEvent(MEvent event) {
    assert(event.get() != NULL);

    if (!eventHandler || !eventHandler->handleEvent(event)) {
        //  拡張ハンドリングを行わなかったら、自前のハンドリングを行う
        switch (event->getType()) {
            case EventType_Touch:
                handleTouchEvent(event);
                break;
            case EventType_Key:
                handleKeyEvent(event);
                break;
            case EventType_RequestFocus:
                handleRequestFocusEvent(event);
                break;
            default:
                if (event->isBroadcast()) {
                    // ウィンドウにブロードキャストを送信する
                    window->broadcastEvent(event);
                } else {
                    jcalertf("drop event handle Type(0x%x) Object(0x%x)", event->getType(), event->getExtension<Object>().get());
                }
                break;
        }
    }
}

/**
 * キューに溜まっているイベントの処理を行う
 */
void WindowManager::handleEvents() {
// 処理したイベント数
    while (!events->empty()) {
        // 直近のイベントを取り出す
        MEvent event = events->popEvent();
        // 実行させる
        dispatchEvent(event);
    }

    // 定期コールイベントを処理する
    if (tick.tickCallTime > 0) {
        double eTime = Timer::lapseTimeSec(tick.lastTickTime);
        if (eTime >= tick.tickCallTime) {
            // 指定秒を超えていたら、コールバックを行う

            if (eventHandler) {
                eventHandler->handleTickEvent(eTime);
            }

            tick.lastTickTime = Timer::currentTime();
        }
    }
}

/**
 * WindowへViewを追加する
 */
void WindowManager::addView(const MView view) {
    window->pushBackChild(view);
    view->registerWindow();
}

/**
 * 毎フレームの処理を行わせる
 * @param numPass レンダリングするパス数
 */
void WindowManager::update(const s32 numPass) {
    for (s32 i = 0; i < numPass; ++i) {
        window->beginPass(ScenePassType_Update, i);
        window->update();
        window->endPass(ScenePassType_Update);
    }
}

/**
 * レンダリングを行う
 * @param numPass レンダリングするパス数
 */
void WindowManager::rendering(const s32 numPass) {
    for (s32 i = 0; i < numPass; ++i) {
        window->beginPass(ScenePassType_Rendering, i);
        window->rendering();
        window->endPass(ScenePassType_Rendering);
    }
}
/**
 * WindowManagerの処理ループ開始を宣言する
 *
 * loopBegin() -> update(nPass) -> rendering(nPass) -> loopEnd(withPost)
 *
 */
void WindowManager::loopBegin() {
    windowContext->onWindowLoopBegin();
}

/**
 * レンダリングの終了を宣言する。
 * フロントバッファ転送バッファの可否を引数で設定する
 */
void WindowManager::loopEnd(const jcboolean withPostFrontBuffer) {
    windowContext->onWindowLoopEnd();
}

/**
 * 定期コールイベントを発行させる。
 * 負の値を設定した場合、コールを行わない。
 * @param callback_sec 何秒間隔で呼び出すかのチェック
 */
void WindowManager::setTickCallbackTime(const double callback_sec) {
    tick.lastTickTime = Timer::currentTime();
    tick.tickCallTime = callback_sec;
}

}
}
