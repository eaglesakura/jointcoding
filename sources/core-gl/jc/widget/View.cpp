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
    visibleCounter.setValueDirect(1.0f);
}

View::~View() {

}

/**
 * フォーカスを持てるかの設定を行う。
 * デフォルトはtrue
 */
void View::setFocusable(const jcboolean set) {
    const jcboolean _hasFocus = hasFocus();
    this->focusable = set;

    if (_hasFocus && !set) {
        // フォーカスを持っていたのに、フォーカスを失ってしまった

        // フォーカスアウトをリクエストする
        requestFocus(jcfalse);
    }
}

/**
 * フォーカス状態を変更する
 */
void View::requestFocus(const jcboolean has) {
    assert(isRegisteredWindow());

    // フォーカスを持てないならそもそも何もしない
    if (!isFocusable() && has) {
        return;
    }

    // 状態が同じだったら何もしない
    if (focus == has) {
        return;
    }

    // 新たなフォーカスをONにするには、古いViewのフォーカスを消す必要がある
    // Windowにフォーカスロストをブロードキャスト
    if (has) {
        MView focusView = lockWindow()->findFocusedView();
        if (focusView && focusView.get() != this) {
            focusView->requestFocus(jcfalse);
        }
    }

    // 自分のフォーカス状態を書き換える
    {
        focus = has;
    }

    // メッセージを送る
    onFocusChanged(has);
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
        // クリックメッセージを処理させる
        onClick();

#if 0
        // 自分のクリックが行われた
        if(isFocusable()) {
            // フォーカス当てを行う
            focus = jctrue;
        }
#else
        requestFocus(jctrue);
#endif

    } else {
#if 0
        // フォーカスを外す
        focus = jcfalse;
#else
        requestFocus(jcfalse);
#endif
    }

#if 0
        // フォーカスに違いが出たらメッセージ
        if(before_focus != hasFocus()) {
            onFocusChanged(hasFocus());
        }
#endif

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
 * フォーカスを失ってほしいリクエスト
 */
void View::dispatchLostFocusRequestEvent(const MEvent event) {
    if (!focus) {
        return;
    }

    focus = jcfalse;
    onFocusChanged(jcfalse);
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
        case EventType_LostFocusRequest:
            dispatchEvent(event);
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

        // コントローラーを作成する
        {
            setWeightCounter(0.2f);
        }

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
void View::setWeightCounter(const float leapTimeSec) {
    assert(isRegisteredWindow());

    // フォーカス用
    {
        const float value = focusCounter.getValue();
        focusCounter.initialize(windowContext, leapTimeSec);
        focusCounter.setValueDirect(value);
    }
    // ダウン用
    {
        const float value = downCounter.getValue();
        downCounter.initialize(windowContext, leapTimeSec);
        downCounter.setValueDirect(value);
    }
    // 可視状態用
    {
        const float value = visibleCounter.getValue();
        visibleCounter.initialize(windowContext, leapTimeSec);
        visibleCounter.setValueDirect(value);
    }
}

/**
 * 可視状態描画用のカウントを取得sルウ
 */
float View::getVisibleWeight() const {
    return visibleCounter.getValue();
}

/**
 * 更新作業を行う
 * trueを返すと「処理完了」とみなして排除する
 */
jcboolean View::update() {
    // 初回パスのみ更新する
    if (getCurrentPass() == 0) {
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

        // 透過度を管理する
        if (isVisible()) {
            ++visibleCounter;
        } else {
            --visibleCounter;
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
 * ネストされた小階層も含めた全体のレイアウトエリアを計算する
 */
RectF View::getGlobalLayoutAreaNest() {
    RectF result = getGlobalLayoutArea();

    std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();

    while (itr != end) {

        MView view = downcast<View>(*itr);
        if (view) {
            RectF child_global = view->getGlobalLayoutAreaNest();

            result.left = jc::min<float>(result.left, child_global.left);
            result.top = jc::min<float>(result.top, child_global.top);

            if (result.left == 0) {
                result.left = child_global.left;
            }

            if (result.top == 0) {
                result.top = child_global.top;
            }

            result.right = jc::max<float>(result.right, child_global.right);
            result.bottom = jc::max<float>(result.bottom, child_global.bottom);
        }

        ++itr;
    }

    return result;
}

/**
 * デバッグ用にレンダリングを行う
 */
void View::renderingArea() {

    assert(isRegisteredWindow());

    MSpriteManager spriteManager = getSpriteManager();

    Color color_fill;
    Color color_line;
    if (isVisible()) {
        color_fill = Color::fromRGBAf(getRenderingPriority(), 0.0f, 1.0f, 0.5f);
        color_line = Color::fromRGBAi(0, 255, 0, 255);
    } else {
        color_fill = Color::fromRGBAf(0, 0, 0, 0);
        color_line = Color::fromRGBAi(0, 0, 255, 255);
    }

    spriteManager->renderingArea(getGlobalLayoutArea(), color_fill, color_line);
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
