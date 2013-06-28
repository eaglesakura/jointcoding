/*
 * WindowContext.cpp
 *
 *  Created on: 2013/04/11
 */

#include    "jc/widget/window/WindowContext.h"
#include    "jc/widget/window/Window.h"

namespace jc {
namespace view {

/**
 * 特定のViewを検索する
 */jc_sp<View> WindowContext::findViewById(const scene_id id) {
    MWindow window = lockWindow();
    assert(window.get());

    MView view = window->getRootView()->findViewById(id);
    return view;
}

/**
 * 特定のViewを検索する
 * 循環参照に注意をすること
 */
jc_sp<View> WindowContext::findEnableViewById(const scene_id id) {
    MWindow window = lockWindow();
    assert(window.get());

    MView view = window->getRootView()->findEnableViewById(id);
    return view;
}

/**
 * タッチ対象をなくす
 */
void WindowContext::clearTouchTarget() {
    // タッチターゲットがあるならフォーカスを外す
    MView view = touchTarget.lock();
    if (view && view->isDown()) {
        view->dispatchDownEvent(jcfalse);
    }
    touchTarget.reset();
}

}

}
