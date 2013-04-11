/*
 * View.cpp
 *
 *  Created on: 2013/04/11
 */

#include    "jc/widget/View.h"
#include    "jc/widget/window/Window.h"

namespace jc {
namespace view {

View::View() {
    this->focus = this->focusable = this->touchable = jcfalse;
}

View::~View() {

}

/**
 * ウィンドウとViewを関連付ける。
 * 必ずWindow（もしくは子View）にaddされた後に呼び出す。
 * 関連付けない場合、WindowContextを得られない。
 */
void View::registerWindow() {

    // 登録されていなければ登録を行う
    if (!windowContext) {
        // 元をたどればWindowを手に入れられるはずである
        Window *window = getRootSceneTo<Window>();
        assert(window != NULL);

        // ウィンドウからコンテキストをコピーする
        this->windowContext = window->windowContext;
    }

    // 子も登録させる
    {
        std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();
        while (itr != end) {
            MView view = jc_dynamic_cast<View>(*itr);
            if(view) {
                view->registerWindow();
            }
            ++itr;
        }
    }
}

            /**
             * レイアウトを更新する。
             */
void View::layout(const RectF &area) {

    // 違うエリアが設定されたらレイアウトを変更してメッセージを投げる
    if (area != localArea) {
        this->localArea = area;
        onLayoutChanged(area);
    }
}

/**
 * 自分自身のレンダリングを行う
 */
void View::onSelfRendering() {

}

}
}
