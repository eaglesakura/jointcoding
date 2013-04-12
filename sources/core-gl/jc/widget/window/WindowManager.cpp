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
    windowTouchDetector.reset(new WindowTouchDetector(windowContext));
    touchDetector.reset(new TouchDetector(windowTouchDetector));
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
    MTouchEventExtension ext = event->getExtension<TouchEventExtension>();
    assert(ext);

    jc_sp<TouchEventProtocol> touchEvent = ext->getPlatformEvent();
    assert(touchEvent.get() != NULL);

    touchDetector->onTouchEvent(touchEvent.get());
}

/**
 * キューに溜まっているイベントの処理を行う
 */
void WindowManager::handleEvents() {
// 処理したイベント数
    s32 eventNum = 0;
    while (!events->empty()) {
        // 直近のイベントを取り出す
        MEvent event = events->popEvent();
        assert(event.get() != NULL);

        switch (event->getType()) {
            case EventType_Touch:
                handleTouchEvent(event);
                break;
        }

        ++eventNum;
    }

//    if (eventNum) {
//        jclogf("Handle Events(%d)", eventNum);
//    }
}

/**
 * WindowへViewを追加する
 */
void WindowManager::addView(const MView view) {
    window->pushBackChild(view);
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

}
}
