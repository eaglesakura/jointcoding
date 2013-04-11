/*
 * WindowManager.cpp
 *
 *  Created on: 2013/04/11
 */

#include    "jc/widget/window/WindowManager.h"

namespace jc {
namespace view {

WindowManager::WindowManager() {
    events.reset(new EventQueue());
    window.reset(new Window());
    windowContext.reset(new WindowContext());
    windowContext->setWindow(window);
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
 * キューに溜まっているイベントの処理を行う
 */
void WindowManager::handleEvents() {
    // 処理したイベント数
    s32 eventNum = 0;
    while (!events->empty()) {
        // 直近のイベントを取り出す
        MEvent event = events->popEvent();
        assert(event.get() != NULL);

        ++eventNum;
    }

    if (eventNum) {
        jclogf("Handle Events(%d)", eventNum);
    }
}

/**
 * 毎フレームの処理を行わせる
 * @param numPass レンダリングするパス数
 */
void WindowManager::update(const s32 numPass) {
    for (s32 i = 0; i < numPass; ++i) {
        window->beginPass(ScenePassType_Update, numPass);
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
        window->beginPass(ScenePassType_Rendering, numPass);
        window->rendering();
        window->endPass(ScenePassType_Rendering);
    }
}
}
}
