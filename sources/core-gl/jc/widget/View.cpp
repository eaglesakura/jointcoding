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
    this->down = this->focus = jcfalse;
    this->focusable = this->touchable = jctrue;
    this->viewMode = ViewMode_Visible;

    // デフォルトのカウンターを整理
    setWeightCounter(createTransitionCounter(60, 0.5));
}

View::~View() {

}

/**
 * クリックされた
 */
void View::onClick() {
    jclogf("onClick(%x)", this);
}

/**
 * フォーカス変更が行われた
 */
void View::onFocusChanged(const jcboolean has) {
    jclogf("onFocusChange(%x) %s", this, has ? "true" : "false");
}

/**
 * フォーカスダウン状態が更新された
 */
void View::onDownChanged(const jcboolean down_now) {
    jclogf("onDownChanged(%x) %s", this, down_now ? "down" : "up");
}

/**
 * Viewのクリック処理が行われた
 */
void View::dispatchClickEvent(const jc_sp<View> clicked) {
    const jcboolean before_focus = hasFocus();

    if(clicked.get() == this) {
        // 自分のクリックが行われた
        if(isFocusable()) {
            // フォーカス当てを行う
            focus = jctrue;
        }

        // クリックメッセージを処理させる
        onClick();
    } else {
        // フォーカスを外す
        focus = jcfalse;
    }

    // フォーカスに違いが出たらメッセージ
    if(before_focus != hasFocus()) {
        onFocusChanged(hasFocus());
    }

}

/**
 * ダウン状態の更新を行う
 */
void View::dispatchDownEvent(const jcboolean down) {
    const jcboolean before_down = this->down;

    if (isTouchable()) {
        this->down = down;
    } else {
        this->down = jcfalse;
    }

    // 違いが生じたらメッセージを送信
    if (before_down != this->down) {
        onDownChanged(down);
    }
}

/**
 * 送信されたイベントを処理する
 */
void View::dispatchEvent(MEvent event) {
    assert(event.get() != NULL);

    const s32 EVENT_TYPE = event->getType();

    switch (EVENT_TYPE) {
        case EventType_Click:
            dispatchClickEvent(event->getExtension<View>());
            break;
        default:
            onEvent(event);
            break;
    }

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
 * 遷移カウンターを更新する
 * フォーカス用、ダウン用が一括で更新される。
 * 現在のvalueは維持される。
 */
void View::setWeightCounter(const CounterF newCounter) {
    // フォーカス用
    {
        const float value = focusCounter.getValue();
        focusCounter = newCounter;
        focusCounter.setValue(value);
    }
    // ダウン用
    {
        const float value = downCounter.getValue();
        downCounter = newCounter;
        downCounter.setValue(value);
    }
}

/**
 * 更新作業を行う
 * trueを返すと「処理完了」とみなして排除する
 */
jcboolean View::update() {
    // 初回パスのみ更新する
    if (isFirstPass()) {
        // フォーカス状態を管理する
        if (hasFocus()) {
            ++focusCounter;
        } else {
            --focusCounter;
        }

        // ダウン状態を管理する
        if (isDown()) {
            ++downCounter;
        } else {
            --downCounter;
        }
    }

    return SceneGraph::update();
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
