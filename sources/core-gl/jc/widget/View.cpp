/*
 * View.cpp
 *
 *  Created on: 2013/04/11
 */

#include    "jc/widget/View.h"
#include    "jc/widget/window/Window.h"
#include    "jc/widget/event/RequestFocusEvent.h"

namespace jc {
namespace view {

View::View(MWindowContext context) {
    assert(context);

    this->context = context;
    this->enableRenderingPass = 0x1;

    flags.enable(ViewFlags_Enable);
    flags.enable(ViewFlags_Focusable);
    flags.enable(ViewFlags_Touchable);
    flags.enable(ViewFlags_FocusMoveFromTouch);
    flags.enable(ViewFlags_Enable);
    flags.enable(ViewFlags_VisibleMultParent);

    this->viewMode = ViewMode_Visible;

    // デフォルトのカウンターを整理
    visibleCounter.setValueDirect(1.0f);
}

View::~View() {

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

    flags.set(ViewFlags_Enable, enable);

    if (hasFocus()) {
        requestFocus(jcfalse);
    }

    if (flags.isEnable(ViewFlags_Down)) {
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
    flags.set(ViewFlags_Focusable, set);

    if (_hasFocus && !set) {
        // フォーカスを持っていたのに、フォーカスを失ってしまった

        // フォーカスアウトをリクエストする
        requestFocus(jcfalse);
    }
}

/**
 * タッチでフォーカス移動処理を行う場合はtrue
 */
void View::setFocusableFromTouch(const jcboolean set) {
    flags.set(ViewFlags_FocusMoveFromTouch, set);
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
        context->interruptionEvent(RequestFocusEventExtension::createInstance(has, getSelfManagedObject()));
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
        std::list<MSceneGraph>::iterator itr = children.begin(), end = children.end();
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
    std::list<MSceneGraph>::iterator itr = parent->children.begin(), end = parent->children.end();

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
    std::list<MSceneGraph>::iterator itr = parent->children.begin(), end = parent->children.end();

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
    std::list<MSceneGraph>::iterator itr = children.begin(), end = children.end();

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
#ifdef DEBUG
//    jclogf("onClick(%x)", this);
#endif
}

/**
 * ドラッグが行われた
 */
void View::onDrag(const Vector2f &currentPos, const Vector2f &quantity, const Vector2f &beginPos) {
//    layoutDirect(localArea.createOffset(quantity.x, quantity.y));
}

/**
 * ドラッグ終了した
 */
void View::onDragEnd(const Vector2f &currentPos, const Vector2f &beginPos) {
    jclogf("onDragEnd (%f, %f) -> (%f, %f)", beginPos.x, beginPos.y, currentPos.x, currentPos.y);
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
//    const jcboolean before_focus = hasFocus();

    if(clicked.get() == this) {
        // クリックメッセージを処理させる
        onClick();
        // リスナがあるなら呼び出す
        if(onClickListener) {
            onClickListener->onClick(getSelfManagedObject());
        }

        // タッチでフォーカス移動の許可を持っている
        if( isFocusMoveFromTouch() ) {

            // 自分にフォーカス属性があるなら、フォーカス変更させる
            if(isFocusable()) {
                requestFocus(jctrue);
            }
        }
    } else {

        // クリック対象にフォーカス属性がある
        // クリック対象がフォーカス移動の権利を持つ
        // なら、フォーカス変更させる
        if(clicked->isFocusable() && clicked->isFocusMoveFromTouch()) {
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

    jclogf("keyCheck(%d)", keyData->getKeyCode());
    if (keyData->isKeeping()) {
        jclogf("isKeeping(%d)", keyData->getKeyCode());
        // キー押しっぱなしはこの時点でチェックしない
        onKeyKeeping(keyData);
        return;
    }

    if (keyData->isPressing()) {
        onKeyDown(keyData);
    } else {
        onKeyUp(keyData);
    }

    if (isClickableKey(keyData)) {
        dispatchDownEvent(keyData->isPressing());
    } else {
        // keyUp
        if (!keyData->isPressing()) {
            scene_id moveTarget = 0;
            if (getNextFocusSceneId(keyData->getKeyCode(), &moveTarget)) {
                dispatchFocusMove(keyData, moveTarget);
            }
        }
    }
}

/**
 * フォーカス移動の制御を行う
 */
void View::dispatchFocusMove(const MKeyData keyData, const scene_id target_view) {
    MView view = context->findViewById(target_view);
    if (!view) {
        // 対象のViewが見つからなかった
        return;
    }

    // 対象がフォーカスを持てなかったら何もしない
    if (!view->isFocusable()) {
        return;
    }

    // 自身のフォーカスを外す
    this->requestFocus(jcfalse);
    // 対象のフォーカスを入れる
    view->requestFocus(jctrue);
}

/**
 * ダウン状態の更新を行う
 */
void View::dispatchDownEvent(const jcboolean down) {
    const jcboolean before_down = flags.isEnable(ViewFlags_Down);

    if (isTouchable()) {
        flags.set(ViewFlags_Down, down);
    } else {
        flags.disable(ViewFlags_Down);
    }

    // 違いが生じたらメッセージを送信
    if (before_down != flags.isEnable(ViewFlags_Down)) {
        if (flags.isEnable(ViewFlags_Down)) {
            // インクリメントモードをtrueに変更
            flags.isEnable(ViewFlags_DownIncrement);
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
    if (flags.isEnable(ViewFlags_HasFocus) == has) {
        return;
    }

    // 新たなフォーカスをONにするには、古いViewのフォーカスを消す必要がある
    // Windowにフォーカスロストをブロードキャスト
    if (has) {
        MView focusView = lockWindow()->findFocusedView();
        if (focusView && focusView.get() != this) {
            focusView->requestFocus(jcfalse);
        }
    } else {
        // フォーカスを失ったら、downイベントも無くなる
        if (isDown()) {
            dispatchDownEvent(jcfalse);
        }
    }

    // 自分のフォーカス状態を書き換える
    {
        flags.set(ViewFlags_HasFocus, has);
    }

    // メッセージを送る
    onFocusChanged(has);
}

/**
 * ドラッグイベントをハンドリングする
 * @param drag ドラッグ中のView
 * @param currentPos 現在の指の位置
 * @param quantity 前回のイベントからの移動量
 * @param beginPos ドラッグ開始時の指の位置
 */
void View::dispatchDragEvent(const jc_sp<View> drag, const Vector2f &currentPos, const Vector2f quantity, const Vector2f beginPos) {
    if(drag.get() != this) {
        return;
    }

    onDrag(currentPos, quantity, beginPos);
}

/**
 * ドラッグイベント終了をハンドリングする
 * @param drag ドラッグ中のView
 * @param currentPos 現在の指の位置
 * @param beginPos ドラッグ開始時の指の位置
 */
void View::dispatchDragEndEvent(const jc_sp<View> drag, const Vector2f &currentPos, const Vector2f beginPos) {
    if(drag.get() != this) {
        return;
    }

    onDragEnd(currentPos, beginPos);
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
        case BroadcastType_Drag:
        case BroadcastType_DragEnd: {
            jc_sp<DragEventExtension> ext = event->getExtension<DragEventExtension>();
            assert(ext);

            if(EVENT_TYPE == BroadcastType_Drag ) {
                dispatchDragEvent(ext->getView(), ext->getCurrentPosition(), ext->getQuantity(), ext->getBeginPosition());
            } else {
                dispatchDragEndEvent(ext->getView(), ext->getCurrentPosition(), ext->getBeginPosition());
            }
            break;
        }
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

    std::list<MSceneGraph>::iterator itr = parent->children.begin(), end = parent->children.end();
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
    if (!isRegisteredWindow() && getParent()) {
        // コントローラーを作成する
        {
            setWeightCounter(0.2f);
        }
        // 非表示なら表示を消す
        if (isInvisible()) {
            visibleCounter.setValueDirect(0);
        }

        sendMessage = jctrue;
        flags.enable(ViewFlags_Registerd);
    }

// 子も登録させる
    {
        std::list<MSceneGraph>::iterator itr = children.begin(), end = children.end();
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
        onRegisteredWindow();
    }
}

/**
 * 子Viewを追加する
 */
void View::addSubView(const jc_sp<View> subView, const jcboolean withRegisterWindow ) {
    pushBackChild(subView);

    if(withRegisterWindow) {
        subView->registerWindow();
    }
}

/**
 * ウィンドウ位置を取得する
 */
RectF View::getWindowArea() {
    return context->lockWindow()->getLocalArea();
}
/**
 * 遷移カウンターを更新する
 * フォーカス用、ダウン用が一括で更新される。
 * 現在のvalueは維持される。
 */
void View::setWeightCounter(const float leapTimeSec) {
    // フォーカス用
    {
        const float value = focusCounter.getValue();
        focusCounter.initialize(context, leapTimeSec);
        focusCounter.setValueDirect(value);
    }
    // ダウン用
    {
        const float value = downCounter.getValue();
        downCounter.initialize(context, leapTimeSec);
        downCounter.setValueDirect(value);
    }
    // 可視状態用
    {
        const float value = visibleCounter.getValue();
        visibleCounter.initialize(context, leapTimeSec);
        visibleCounter.setValueDirect(value);
    }
}

/**
 * 可視状態描画用のカウントを取得sルウ
 */
float View::getVisibleWeight() const {
    float parentVisible = 1.0f;
    if (flags.isEnable(ViewFlags_VisibleMultParent) && getParent()) {
        View *parent = getParentTo<View>();
        if (parent) {
            parentVisible = parent->getVisibleWeight();
        }
    }
    return parentVisible * visibleCounter.getValue();
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
        if (flags.isEnable(ViewFlags_DownIncrement)) {
            ++downCounter;

            // カウンタが一番上まで行ったらダウンチェックしてもとに戻す
            if (downCounter.isMaxValue() && !isDown()) {
                flags.disable(ViewFlags_DownIncrement);
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
void View::layoutDirect(const RectF &area) {

// 違うエリアが設定されたらレイアウトを変更してメッセージを投げる
    if (area != localArea) {
        this->localArea = area;
        onLayoutChanged(area);
    }
}

/**
 * 親のレイアウトエリア内で移動を行う
 */
void View::moveTo(const LayoutParams &params) {

    LayoutParams temp = params;
    if (temp.layout_width <= 0) {
        temp.layout_width = localArea.width();
    }
    if (temp.layout_height <= 0) {
        temp.layout_height = localArea.height();
    }

    assert(temp.layout_width > 0);
    assert(temp.layout_height > 0);

    View *parentView = getParentTo<View>();
    assert(parentView);

    RectF area;
    temp.layout(&area, parentView->getLocalLayoutSize());

    this->layoutDirect(area);
}

/**
 * 親レイアウトの配置内で移動を行う
 */
void View::moveToGravity(const u32 gravity_flags, const Vector2f &margins) {
    LayoutParams params;
    params.gravity = gravity_flags;
    params.margin_left = params.margin_right = margins.x;
    params.margin_top = params.margin_bottom = margins.y;
    moveTo(params);
}

/**
 * 親と同じ領域になるようにエリアを設定する
 */
void View::layoutFillParent(const Vector2f &parentLocalSize) {
    layoutDirect(createRectFromLTRB(0.0f, 0.0f, parentLocalSize.x, parentLocalSize.y));
}

/**
 * 親を辿って可視状態である場合trueを返す
 */
jcboolean View::isVisible() const {
    // 自身が不可視だったらfalseを返して終了
    if (viewMode != ViewMode_Visible) {
        return jcfalse;
    }

    View *parent = getParentTo<View>();
    // 親がいるなら、親の決定に従う
    if (parent) {
        return parent->isVisible();
    }

    // 親がいないから、自身はtrueである
    return jctrue;
}

/**
 * すべてのViewのフォーカス値がゼロであることを確認する
 */
jcboolean View::isAllFocusWeightZero(const jcboolean recursive) const {
    if (getFocusWeight() > 0) {
        return jcfalse;
    }

    std::list<MSceneGraph>::const_iterator itr = children.begin(), end = children.end();

    while (itr != end) {
        MView view = downcast<View>(*itr);
        if (view) {
            if (!view->isAllFocusWeightZero(recursive)) {
                return jcfalse;
            }
        }

        ++itr;
    }

    return jctrue;
}

/**
 * 配置場所（ローカル位置）を取得する
 */
RectF View::getLocalLayoutAreaNest() const {

    RectF globalArea = getGlobalLayoutAreaNest();

    // グローバルとの差分チェック
    Vector2f offset = getGlobalOffset();
    globalArea.offset(-offset.x, -offset.y);
    return globalArea;

}

/**
 * ネストされた小階層も含めた全体のレイアウトエリアを計算する
 */
RectF View::getGlobalLayoutAreaNest() const {
    RectF result = getGlobalLayoutArea();

    std::list<MSceneGraph>::const_iterator itr = children.begin(), end = children.end();

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
// デバッグ用に自身のエリアを描画する場合はtrue
#ifdef  RENDERING_VIEWAREA
    renderingArea();
#endif
}

/**
 * ボタンが押されたときと同じアクションを行わせる
 */
void View::emulateButtonDown() {
    if (!isDown()) {
        dispatchDownEvent(jctrue);
        dispatchDownEvent(jcfalse);
    }
}

}
}
