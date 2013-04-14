/*
 * Window.cpp
 *
 *  Created on: 2013/04/12
 */

#include    "jc/widget/window/Window.h"

namespace jc {
namespace view {

/**
 *
 */
void Window::broadCastEvent(MView view, MEvent event) {
    view->dispatchEvent(event);

    std::list<MSceneGraph>::iterator itr = view->childs.begin(), end = view->childs.end();

    while (itr != end) {
        MView child = downcast<View>(*itr);
        if (child) {
            broadCastEvent(child, event);
        }
        ++itr;
    }
}

/**
 * 全Viewにイベントを伝える
 */
void Window::broadcastEvent(MEvent event) {
    // 子をチェックする
    std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();

    while (itr != end) {
        MView child = downcast<View>(*itr);

        // 子にメッセージを贈る
        if (child) {
            broadCastEvent(child, event);
        }

        ++itr;
    }
}

/**
 * 特定のレイアウトへイベントを送信する
 * ハンドルに成功したらtrueを返す
 */
jcboolean Window::sendEvent(const scene_id layoutId, MEvent event) {
    MView view = findSceneTo<View>(layoutId);
    if (view) {
        view->dispatchEvent(event);
        return jctrue;
    }
    return jcfalse;
}
/**
 * 遷移カウンターを更新する
 * View管理用が一括で更新される
 * 現在のvalueは維持される。
 */
void Window::setWeightCounter(const float leapTimeSec) {
    // Windowは特殊なクラスになるため、遷移設定を行わない
}

MView Window::findFocusView(MView view) {
    if (!view) {
        return MView();
    }

    if (view->hasFocus()) {
        return view;
    }

    std::list<MSceneGraph>::iterator itr = view->childs.begin(), end = view->childs.end();
    while (itr != end) {
        findFocusView(downcast<View>(*itr));
        ++itr;
    }

    return MView();
}

/**
 * フォーカスを持っているViewを取得する
 */
MView Window::findFocusedView() {

    std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();
    while (itr != end) {
        findFocusView(downcast<View>(*itr));
        ++itr;
    }

    return MView();
}

}
}
