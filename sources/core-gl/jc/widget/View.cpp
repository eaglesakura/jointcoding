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
    this->focus = this->focusable = this->touchable = jctrue;
    this->viewMode = ViewMode_Visible;
}

View::~View() {

}

/**
 * Viewのクリック処理が行われた
 */
void View::dispatchClickEvent(const jc_sp<View> clicked) {
}

/**
 * 送信されたイベントを処理する
 */
void View::dispatchEvent(MEvent event) {
    onEvent(event);
}

/**
 * ウィンドウとViewを関連付ける。
 * 必ずWindow（もしくは子View）にaddされた後に呼び出す。
 * 関連付けない場合、WindowContextを得られない。
 */
void View::registerWindow() {

    jcboolean sendMessage = jcfalse;
    // 登録されていなければ登録を行う
    if (!windowContext && getParent()) {
        // 元をたどればWindowを手に入れられるはずである
        Window *window = getRootSceneTo<Window>();
        assert(window != NULL);

        // ウィンドウからコンテキストをコピーする
        this->windowContext = window->windowContext;

        sendMessage = jctrue;
    }

    // 子も登録させる
    {
        std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();
        while (itr != end) {
            MView view = downcast<View>(*itr);
            if (view) {
                view->registerWindow();
            }
            ++itr;
        }
    }

    // 登録完了したことを通知する
    if (sendMessage) {
        // 登録が完了した
        onRegisterdWindow();
    }
}

/**
 * ウィンドウ位置を取得する
 */
RectF View::getWindowArea() {
    assert(windowContext.get() != NULL);
    return windowContext->lockWindow()->getLocalLayoutArea();
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
 * デバッグ用にレンダリングを行う
 */
void View::renderingArea() {

    assert(isRegisteredWindow());

    MSpriteManager spriteManager = getSpriteManager();
    const RectF area = getGlobalLayoutArea().createScaling(0.99f);

    if (isVisible()) {
        Color color = Color::fromRGBAf(getRenderingPriority(), 0.0f, 1.0f, 0.5f);
        spriteManager->renderingRect(area, color.rgba);
    }

    // 周りの線を描画する
    {
        Color color;
        if (isVisible()) {
            color.rgba = 0x00FF00FF;
        } else {
            color.rgba = 0x0000FF7F;
        }
        spriteManager->startLineRendering();
        spriteManager->renderingRect(area, color.rgba);
        spriteManager->startQuadRendering();
    }
}

/**
 * 自分自身のレンダリングを行う
 */
void View::onSelfRendering() {
#ifdef  DEBUG
    renderingArea();
#endif
}

}
}
