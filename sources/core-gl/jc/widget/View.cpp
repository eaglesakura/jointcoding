/*
 * View.cpp
 *
 *  Created on: 2013/04/11
 */

#include    "jc/widget/View.h"
#include    "jc/widget/window/Window.h"
#include    "jc/widget/RegisteredInitializer.h"
#include    "jc/widget/event/RequestFocusEvent.h"

namespace jc {
namespace view {

View::View() {
    this->down = this->down_inc = this->focus = jcfalse;
    this->enable = this->focusable = this->touchable = jctrue;
    this->viewMode = ViewMode_Visible;

    // デフォルトのカウンターを整理
    visibleCounter.setValueDirect(1.0f);
}

View::~View() {

}

/**
 * 遷移カウンターをイニシャライザリストに登録する
 */
void View::addTransacationInitializer(const jc_selp<TransactionCounter> counter, const float transaction_sec, const LeapType_e type) {
    addRegisteredInitializer(jc_sp<TransactionInitializer>( new TransactionInitializer(counter, transaction_sec, type)) );
}

/**
 * タイマーをイニシャライザリストに登録する
 */
void View::addTimerInitializer(const jc_selp<WindowTimer> timer, const u32 timeMilliSec) {
    addRegisteredInitializer(jc_sp<RegisteredInitializer>(new TimerInitializer(timer, timeMilliSec)));
}

/**
 * クリック反応するキーの場合trueを返す。
 * 標準ではENTERキー
 */
jcboolean View::isClickableKey(const MKeyData key) const {
    return key->isEnterKey();
}

/**
 * 状態の有効・無効を一括で切り替える
 */
void View::setEnable(const jcboolean enable) {
    if (isEnable() == enable) {
        return;
    }

    this->enable = enable;
    if (hasFocus()) {
        requestFocus(jcfalse);
    }

    if (down) {
        dispatchDownEvent(jcfalse);
    }

    onEnableChanged(enable);

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

    if (!has) {
        dispatchRecuestFocus(jcfalse);
    } else {
        // リクエストを保留させる
        windowContext->interruptionEvent(RequestFocusEventExtension::createInstance(has, getSelfManagedObject()));
    }

}

/**
 * フォーカスを持っている場合はtrueを返す。
 * 子孫を探索して、子孫がフォーカスを持っている場合もtrueを返す。
 */
jcboolean View::hasFocus(const jcboolean recursive) {
    if (hasFocus()) {
        return jctrue;
    }

    if (recursive) {
        std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();
        while (itr != end) {

            MView view = downcast<View>(*itr);
            if (view && view->hasFocus(jctrue)) {
                // 子のフォーカス探索に成功した
                return jctrue;
            }
            ++itr;
        }
    }

    // 見つからなかった
    return jcfalse;
}

/**
 * 親に登録されているView番号を取得する
 */
s32 View::getViewIndex() {
    View *parent = getParentTo<View>();

    // 呼び出し時、親がいなければならない
    assert(isRegisteredWindow());
    assert(parent != NULL);

    s32 result = 0;
    std::list<MSceneGraph>::iterator itr = parent->childs.begin(), end = parent->childs.end();

    while (itr != end) {
        MView view = downcast<View>(*itr);
        if (view) {

            if (view.get() == this) {
                return result;
            }

            // ヒットしなければ次を見る
            ++result;
        }
        ++itr;
    }

    // 親に自分が登録されていないのは異常事態である
    assert(false);
    return -1;
}

/**
 * 兄弟Viewを取得する
 * 親を探索し、自分の兄弟インデックス+offsetのViewを探索する。
 * offset != 0でなければならない。
 * 見つからなかった場合NULLオブジェクトを返す
 */ //
jc_sp<View> View::findSibling(const s32 offset) {
    View *parent = getParentTo<View>();

    // 呼び出し時、親がいなければならない
    assert(isRegisteredWindow());
    assert(parent != NULL);

    // 自身を呼び出す意味は無い
    assert(offset != 0);

    const s32 target_index = getViewIndex() + offset;

    // 無効なインデックスであれば空を返す
    if(target_index < 0 || target_index >= getParent()->getChildrenNum()) {
        return MView();
    }

    jclogf("SiblingTarget(%d)", target_index);

    // インデックスを探す

    s32 check_index = 0;
    std::list<MSceneGraph>::iterator itr = parent->childs.begin(), end = parent->childs.end();

    while (itr != end) {
        MView view = downcast<View>(*itr);
        if (view) {
            // 対象のインデックスだったら返す
            if(check_index == target_index) {
                return view;
            }

            // ヒットしなければ次を見る
            ++check_index;
        }
        ++itr;
    }

    // 探索失敗した
    return MView();
}
/**
 * 特定Indexの子Viewを取得する
 */
MView View::getChildViewAt(const s32 index) {
    // インデックスを探す
    s32 check_index = 0;
    std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();

    while (itr != end) {
        MView view = downcast<View>(*itr);
        if (view) {
            // 対象のインデックスだったら返す
            if (check_index == index) {
                return view;
            }

            // ヒットしなければ次を見る
            ++check_index;
        }
        ++itr;
    }

    // 探索失敗した
    return MView();
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
//    jclogf("onFocusChange(%x) %s", this, has ? "true" : "false");
}

/**
 * フォーカスダウン状態が更新された
 */
void View::onDownChanged(const jcboolean down_now) {
//    jclogf("onDownChanged(%x) %s", this, down_now ? "down" : "up");
}

/**
 * Viewのクリック処理が行われた
 */
void View::dispatchClickEvent(const jc_sp<View> clicked) {
    const jcboolean before_focus = hasFocus();

    if(clicked.get() == this) {
        // クリックメッセージを処理させる
        onClick();

        // 自分にフォーカス属性があるなら、フォーカス変更させる
        if(isFocusable()) {
            requestFocus(jctrue);
        }
    } else {

        // クリック対象にフォーカス属性があるなら、フォーカス変更させる
        if(clicked->isFocusable()) {
            requestFocus(jcfalse);
        }
    }
}

/**
 * どれかのキーが押された
 */
void View::dispatchKeyEvent(const MKeyData keyData) {
    assert(keyData.get() != NULL);

    // フォーカスを持っていなければ何もしない
    if (!hasFocus()) {
        return;
    }

    if (keyData->isPressing()) {
        onKeyDown(keyData);
    } else {
        onKeyUp(keyData);
    }

    if (isClickableKey(keyData)) {
        dispatchDownEvent(keyData->isPressing());
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
        if (this->down) {
            // インクリメントモードをtrueに変更
            down_inc = jctrue;
        }

        onDownChanged(down);
    }
}

#if 0
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
#endif

/**
 * このViewにフォーカスが当たるようにリクエストされた
 */
void View::dispatchRecuestFocus(const jcboolean has) {

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
 * 送信されたイベントを処理する
 */
void View::dispatchEvent(MEvent event) {
    assert(event.get() != NULL);

    const s32 EVENT_TYPE = event->getType();
//    jclogf("dispatch event(%x) (%x)", EVENT_TYPE, this);

    switch (EVENT_TYPE) {
        case BroadcastType_Click:
            dispatchClickEvent(event->getExtension<View>());
            break;
        case BroadcastType_Key:
            dispatchKeyEvent(event->getExtension<KeyData>());
            break;
        default:
            onEvent(event);
            break;
    }
}

/**
 * 自分自身を示すMViewを取得する。
 * 循環参照に注意すること。
 */
MView View::getSelfManagedObject() {
    View *parent = getParentTo<View>();
    assert(parent != NULL);

    std::list<MSceneGraph>::iterator itr = parent->childs.begin(), end = parent->childs.end();
    while (itr != end) {
        if ((*itr).get() == (SceneGraph*) this) {
            return downcast<View>(*itr);
        }
        ++itr;
    }

    // 取得できないのは異常事態である
    assert(false);
    return MView();
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
        // 非表示なら表示を消す
        if (isInvisible()) {
            visibleCounter.setValueDirect(0);
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

    // 初期化実行を行う
    {
        std::list<MRegisteredInitializer>::iterator itr = windowRegisteredInitializer.begin(), end = windowRegisteredInitializer.end();
        while (itr != end) {
            (*itr)->onRegisteredWindow(this, windowContext);
            ++itr;
        }

        // 既存のイニシャライザリストを解放する
        windowRegisteredInitializer.clear();
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
        if (down_inc) {
            ++downCounter;

            // カウンタが一番上まで行ったらダウンチェックしてもとに戻す
            if (downCounter.isMaxValue() && !isDown()) {
                down_inc = jcfalse;
            }

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

            if (result.empty()) {
                result = child_global;
            } else {
                result.left = jc::min<float>(result.left, child_global.left);
                result.top = jc::min<float>(result.top, child_global.top);
                result.right = jc::max<float>(result.right, child_global.right);
                result.bottom = jc::max<float>(result.bottom, child_global.bottom);
            }

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

    const RectF area = getGlobalLayoutArea();
    // レンダリングエリアが設定されてないなら何もしない
    if (area.getAreaSize() == 0.0f) {
        return;
    }

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
