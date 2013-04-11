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

    MView view = window->findSceneTo<View>(id);
    return view;
}}
}
