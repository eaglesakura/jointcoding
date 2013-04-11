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

}
}
