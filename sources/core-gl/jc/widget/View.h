/*
 * View.h
 *
 *  Created on: 2013/04/11
 */

#ifndef GLVIEW_VIEW_H_
#define GLVIEW_VIEW_H_

#include    "jc/math/Vec2.h"
#include    "jc/math/Rect.h"
#include    "jc/scene/SceneGraph.h"
#include    "jc/collection/BitFlags.hpp"
#include    "jc/math/Counter.h"

#include    "jc/widget/event/Event.h"
#include    "jc/widget/window/WindowContext.h"
#include    "jc/widget/anim/TransactionCounter.h"
#include    "jc/widget/anim/WindowTimer.h"
#include    "jc/widget/view/ViewListeners.h"

//#include    "jc/ui/TouchPoint.h"

namespace jc {
namespace view {

class WindowManager;
class RegisteredInitializer;
/**
 * managed
 */
typedef jc_sp<RegisteredInitializer> MRegisteredInitializer;

/**
 * Viewの表示状態を取得する
 */
enum ViewMode_e {
    /**
     * 表示状態
     */
    ViewMode_Visible,

    /**
     * 非表示状態
     */
    ViewMode_Invisible,
};

/**
 * UI構築用のView構造を提供する
 *
 * Viewの状態は下記を基本にする
 * * フォーカス:キーやクリック等で「選択済み」の場合のみtrueとなる
 * * ダウン：キーを押している最中、クリックをしている最中のみtrueになる
 *      * フォーカスとダウンは同時に起こりえる
 */
class View: public SceneGraph {
private:
    friend class Window;
    friend class WindowContext;
    friend class WindowManager;
    friend class WindowEventListener;

    /**
     * click listener
     */
    SOnClickListener onClickListener;

    /**
     * フォーカスを持つことが出来る場合true
     */
    jcboolean focusable;

    /**
     * タッチを行える場合true
     */
    jcboolean touchable;

    /**
     * フォーカスを持っている場合true
     */
    jcboolean focus;

    /**
     * タッチイベントでフォーカスを移動させる場合true
     * default = true
     */
    jcboolean focusmove_fromtouch;

    /**
     * ダウンを行なっている途中
     */
    jcboolean down;

    /**
     * ダウン状態のインクリメントを続ける場合はtrue
     */
    jcboolean down_inc;

    /**
     * 有効状態
     */
    jcboolean enable;

    /**
     * 有効なレンダリングパス
     */
    s32 enableRenderingPass;

    /**
     * Viewの表示状態を取得する
     */
    ViewMode_e viewMode;

    /**
     * Window登録された時に初期化するための簡易コールバックリスト
     */
    std::list<MRegisteredInitializer> windowRegisteredInitializer;

protected:

    /**
     * Window登録時のイニシャライザを追加する
     */
    virtual void addRegisteredInitializer(const MRegisteredInitializer regInitializer) {
        // Window登録されていてはいけない
        assert(!isRegisteredWindow());

        windowRegisteredInitializer.push_back(regInitializer);
    }

    /**
     * 押下フォーカスの遷移をリセットする。
     */
    virtual void resetDownFocusCounter() {
        down_inc = jcfalse;
        downCounter.getCounter().setIncrementMode(jctrue);
        downCounter.getCounter().reset();
    }

    /**
     * 遷移カウンターをイニシャライザリストに登録する
     */
    virtual void addTransacationInitializer(const jc_selp<TransactionCounter> counter, const float transaction_sec, const LeapType_e type = LeapType_Ease1);

    /**
     * タイマーをイニシャライザリストに登録する
     */
    virtual void addTimerInitializer(const jc_selp<WindowTimer> timer, const u32 timeMilliSec);

    /**
     * クリック反応するキーの場合trueを返す。
     * 標準ではENTERキー
     */
    virtual jcboolean isClickableKey(const MKeyData key) const;

    /**
     * フォーカス状態を管理する遷移カウンター
     */
    TransactionCounter focusCounter;

    /**
     * ダウン状態を管理する遷移カウンター
     */
    TransactionCounter downCounter;

    /**
     * 可視状態を管理する遷移カウンター
     */
    TransactionCounter visibleCounter;

    /**
     * Viewの配置
     */
    RectF localArea;

    /**
     * ウィンドウの中心情報
     */
    jc_sp<WindowContext> windowContext;

    /**
     * Spriteレンダリングクラスを取得する
     */
    virtual MSpriteManager getSpriteManager() const {
        assert(windowContext.get() != NULL);
        return windowContext->getSpriteManager();
    }

    /**
     * レンダリングデバイスを取得するう
     */
    virtual MDevice getDevice() const {
        assert(windowContext.get() != NULL);
        return windowContext->getDevice();
    }

    /**
     * Context管理クラスを取得する
     */
    virtual MGLState getState() const {
        return getDevice()->getState();
    }

    /**
     * デバッグ用にレンダリングを行う
     */
    virtual void renderingArea();

    /**
     * 秒間パラメーターをフレームパラメーターに変換する
     */
    template<typename T>
    T frameValue(const T secValue) {
        assert(isRegisteredWindow());
        return windowContext->frameValue<T>(secValue);
    }

    /**
     * 自身のレンダリングを許可するパスであるならtrueを返す。
     */
    virtual jcboolean isSelfRenderingPass() const {
        if(!enableRenderingPass) {
            SceneGraph::isSelfRenderingPass();
        }

        return (enableRenderingPass & (0x1 << getCurrentPass())) != 0;
    }

public:
    View();
    virtual ~View();

    /**
     * 状態の有効・無効を一括で切り替える
     */
    virtual void setEnable(const jcboolean enable);

    /**
     * 有効・無効状態を問い合わせる
     * default = true
     */
    virtual jcboolean isEnable() const {
        return enable;
    }

    /**
     * ウィンドウとViewを関連付ける。
     * 必ずWindow（もしくは子View）にaddされた後に呼び出す。
     * 関連付けない場合、WindowContextを得られない。
     */
    virtual void registerWindow();

    /**
     * ウィンドウ位置を取得する
     */
    virtual RectF getWindowArea( );

    /**
     * 子Viewを検索する。
     * 自身は含まないことに注意すること。
     */
    virtual jc_sp<View> findViewById( const scene_id id) const {
        return findSceneTo<View>(id);
    }

    /**
     * 親に登録されているView番号を取得する
     */
    virtual s32 getViewIndex();

    /**
     * 特定Indexの子Viewを取得する
     */
    virtual jc_sp<View> getChildViewAt(const s32 index);

    /**
     * 兄弟Viewを取得する
     * 親を探索し、自分の兄弟インデックス+offsetのViewを探索する。
     * offset != 0でなければならない。
     * 見つからなかった場合NULLオブジェクトを返す
     */
    virtual jc_sp<View> findSibling(const s32 offset);

    /**
     * Windowを一時的に取得する。
     * メンバとして持つと循環参照が行われる恐れがあるため、シーングラフ内では行わないこと。
     */
    virtual jc_sp<Window> lockWindow() {
        assert(windowContext.get() != NULL);
        return windowContext->lockWindow();
    }

    /**
     * ウィンドウと関連付けが済んでいる場合はtrueを返す
     */
    virtual jcboolean isRegisteredWindow() const {
        return windowContext.get() != NULL;
    }

    /**
     * 表示状態の場合trueを返す
     */
    virtual jcboolean isVisible() const {
        return viewMode == ViewMode_Visible;
    }

    /**
     * 非表示の場合trueを返す
     */
    virtual jcboolean isInvisible() const {
        return viewMode == ViewMode_Invisible;
    }

    /**
     * Viewの表示状態を設定する
     */
    virtual void setVisibly( const ViewMode_e mode) {
        viewMode = mode;
    }

    /**
     * 視覚状態を設定する
     */
    virtual void setVisiblyWeight(const float set) {
        visibleCounter.setValueDirect(set);
    }

    /**
     * 可視状態描画用のカウントを取得する
     */
    virtual float getVisibleWeight() const;

    /**
     * フォーカス描画用のカウントを取得する
     */
    virtual float getFocusWeight(const jcboolean withVisibleCounter = jctrue) const {
        return focusCounter.getValue() * (withVisibleCounter ? getVisibleWeight() : 1.0f);
    }

    /**
     * フォーカス状態
     */
    virtual float isFocusWeightStopped() {
        return focusCounter.isStopped();
    }

    /**
     * ダウン描画用のカウントを取得する
     */
    virtual float getDownWeight(const jcboolean withVisibleCounter = jctrue) const {
        return downCounter.getValue() * (withVisibleCounter ? getVisibleWeight() : 1.0f);
    }

    /**
     * フォーカスを持てるかの設定を行う。
     * デフォルトはtrue
     */
    virtual void setFocusable( const jcboolean set);

    /**
     * タッチでフォーカス移動処理を行う場合はtrue
     */
    virtual void setFocusableFromTouch(const jcboolean set);

    /**
     * フォーカス状態を変更する
     */
    virtual void requestFocus(const jcboolean has);

    /**
     * フォーカスを持っている場合はtrueを返す
     */
    virtual jcboolean hasFocus() const {
        return focus;
    }

    /**
     * フォーカスを持っている場合はtrueを返す。
     * 子孫を探索して、子孫がフォーカスを持っている場合もtrueを返す。
     */
    virtual jcboolean hasFocus(const jcboolean recursive);

    /**
     * フォーカスを持つことができるViewの場合はtrueを返す
     */
    virtual jcboolean isFocusable() const {
        return focusable && isVisible() && isEnable();
    }

    /**
     * タッチイベントでフォーカスを移す場合はtrueを返す
     * default = false
     */
    virtual jcboolean isFocusMoveFromTouch() const {
        return focusmove_fromtouch;
    }

    /**
     * タッチの可否を設定する
     */
    virtual void setTouchable(const jcboolean set) {
        touchable = set;
    }

    /**
     * タッチを処理すべきViewである場合はtrueを返す
     */
    virtual jcboolean isTouchable() const {
        return touchable && isVisible() && isEnable();
    }

    /**
     * Viewがタッチかキーダウン状態にある場合true
     */
    virtual jcboolean isDown() const {
        return down;
    }

    /**
     * 配置場所（ローカル位置）を取得する
     */
    virtual RectF getLocalLayoutArea() const {
        return localArea;
    }

    /**
     * レイアウトサイズを取得する
     */
    virtual Vector2f getLocalLayoutSize() const {
        return localArea.wh();
    }

    /**
     * Viewの幅を取得する
     */
    virtual float getViewWidth() const {
        return localArea.width();
    }

    /**
     * Viewの高さを取得する
     */
    virtual float getViewHeight() const {
        return localArea.height();
    }

    /**
     * 衝突判定位置（ローカル座標）を取得する
     */
    virtual RectF getLocalIntersectArea() const {
        return localArea;
    }

    /**
     * 遷移カウンターを更新する
     * View管理用が一括で更新される
     * 現在のvalueは維持される。
     */
    virtual void setWeightCounter(const float leapTimeSec);

    /**
     * 更新作業を行う
     * trueを返すと「処理完了」とみなして排除する
     */
    virtual jcboolean update();

    /**
     * レイアウトを更新する。
     */
    virtual void layout(const RectF &area);

    /**
     * 親と同じ領域になるようにエリアを設定する
     */
    virtual void layoutFillParent(const Vector2f &parentLocalSize);
    /**
     * 親と同じ領域になるようにエリアを設定する
     */
    virtual void layoutFillParent(const RectF &parentLocal) {
        layoutFillParent(parentLocal.wh());
    }
    /**
     * 親と同じ領域になるようにエリアを設定する
     */
    virtual void layoutFillParent() {
        layoutFillParent(getParentTo<View>()->getLocalLayoutSize());
    }

    /**
     * ネストされた小階層も含めた全体のレイアウトエリアを計算する
     */
    virtual RectF getGlobalLayoutAreaNest( ) const;

    /**
     * グローバル位置に変換する
     */
    virtual Vector2f toGlobalPosition(const Vector2f &localPosition) const {
        Vector2f parent_pos;
        if (getParent()) {
            View *parentView = getParentTo<View>();
            if(parentView) {
                parent_pos = parentView->toGlobalPosition(Vector2f());
            }
        }

        return localPosition + parent_pos;
    }

    /**
     * グローバル座標に変換した表示位置を取得する
     */
    virtual RectF getGlobalLayoutArea() const {
        RectF parent;
        if (getParent()) {
            View *parentView = getParentTo<View>();
            if(parentView) {
                parent = parentView->getGlobalLayoutArea();
            }
        }
        RectF local = getLocalLayoutArea();
        local.offset(parent.left, parent.top);
        return local;
    }

    /**
     * ローカルをグローバルに
     */
    virtual RectF toGlobalLayoutArea(const RectF &local) const {
        const RectF selfGlobal = getGlobalLayoutArea();
        RectF temp = local;
        temp.offset(selfGlobal.left, selfGlobal.top);
        return temp;
    }

    /**
     * グローバル座標に変換した衝突判定位置を取得する
     */
    virtual RectF getGlobalIntersectArea() const {
        RectF parent;
        if (getParent()) {
            parent = getParentTo<View>()->getGlobalLayoutArea();
        }
        RectF local = getLocalIntersectArea();
        local.offset(parent.left, parent.top);
        return local;
    }

    /**
     * グローバル座標を指定し、衝突していればtrueを返す
     */
    virtual jcboolean isIntersect(const Vector2f &global) const {
        const RectF globalRect = getGlobalIntersectArea();
        return globalRect.isIntersect(global.x, global.y);
    }

    /**
     * グローバル座標にタッチされた時に自分が処理すべきViewであるならtrueを返す。
     * 子は考慮しない。
     */
    virtual jcboolean isTouchedView(const Vector2f &global) const {
        if (!isTouchable()) {
            return jcfalse;
        }
        return isIntersect(global);
    }

    /**
     * 位置に適合するViewを子から探して返す。
     * 自分自身は含めない。 isTouchedView()が基準となる。
     */
    virtual jc_sp<View> findTouchedView( const Vector2f &global) const {
        assert(isRegisteredWindow());

        std::list<MSceneGraph>::const_reverse_iterator itr = childs.rbegin(), end = childs.rend();

        // 全チェックを行う
            while(itr != end) {

                jc_sp<View> check = downcast<View>(*itr);

                // 子がViewであるならチェックする
                if(check) {
                    // 子の、更に子をチェックする
                    jc_sp<View> check_child = check->findTouchedView(global);
                    if(check_child) {
                        // 孫以降に見つかったから、それを代理で返す
                        return check_child;
                    }

                    if(check->isTouchedView(global)) {
                        return check;
                    }
                }

                // 次の子をチェックする
                ++itr;
            }

            // 何も見つからなかった
            return jc_sp<View>();
        }

        /**
         * 位置に適合するViewを兄弟から探して返す。
         * 自分自身は含めない。 isTouchedView()が基準となる。
         */
        virtual jc_sp<View> findSiblingTouchedView( const Vector2f &global) const {
            assert(isRegisteredWindow());

            View *parent = getParentTo<View>();
            assert(parent != NULL);

            jc_sp<View> result = parent->findTouchedView(global);
            if(result.get() == this) {
                // 自身だったらNULLオブジェクトを返す
                return jc_sp<View>();
            }

            return result;
        }

        /**
         * フォーカスを持つことができる最初のViewを探索する。
         * 子 -> 子の子孫 -> 次の子 -> 子の子孫 の順番で探索される
         */
        virtual jc_sp<View> findFocusableView() const {
            std::list<MSceneGraph>::const_iterator itr = childs.begin(), end = childs.end();

            // 全チェックを行う
            while(itr != end) {

                jc_sp<View> check = downcast<View>(*itr);

                // 子がViewであるならチェックする
                if(check) {
                    if(check->isFocusable()) {
                        // フォーカス対象になったからそれを返す
                        return check;
                    }
                    // 子の、更に子をチェックする
                    check = check->findFocusableView();
                    if(check) {
                        // 孫以降に見つかったから、それを代理で返す
                        return check;
                    }
                }

                // 次の子をチェックする
                ++itr;
            }

            // 何も見つからなかった
            return jc_sp<View>();
        }

        /**
         * フォーカスを持っているViewを取得する
         */
        virtual jc_sp<View> findFocusedView() const {
            std::list<MSceneGraph>::const_iterator itr = childs.begin(), end = childs.end();

            // 全チェックを行う
            while(itr != end) {

                jc_sp<View> check = downcast<View>(*itr);

                // 子がViewであるならチェックする
                if(check) {
                    if(check->hasFocus()) {
                        // フォーカスを持っていたらtrueを返す
                        return check;
                    }
                    // 子の、更に子をチェックする
                    check = check->findFocusedView();
                    if(check) {
                        // 孫以降に見つかったから、それを代理で返す
                        return check;
                    }
                }

                // 次の子をチェックする
                ++itr;
            }

            // 何も見つからなかった
            return jc_sp<View>();
        }

        /**
         * 一致するIDを全て列挙して返す
         */
        virtual s32 findViewListById(const scene_id id, std::list< jc_sp<View> > *result) const {
            std::list<MSceneGraph>::const_iterator itr = childs.begin(), end = childs.end();
            while(itr != end) {
                jc_sp<View> view = downcast<View>(*itr);
                if(view) {
                    if(view->getUniqueId() == id) {
                        result->push_back(view);
                    }

                    view->findViewListById(id, result);
                }
                ++itr;
            }

            return result->size();
        }

        /**
         * 一致するIDで有効なViewを全て列挙して返す
         */
        virtual s32 findEnableViewListById(const scene_id id, std::list< jc_sp<View> > *result) const {
            std::list<MSceneGraph>::const_iterator itr = childs.begin(), end = childs.end();
            while(itr != end) {
                jc_sp<View> view = downcast<View>(*itr);
                if(view) {
                    if(view->getUniqueId() == id && view->isEnable()) {
                        result->push_back(view);
                    }

                    view->findViewListById(id, result);
                }
                ++itr;
            }

            return result->size();
        }

        /**
         * 有効なViewを探して返す
         */
        virtual jc_sp<View> findEnableViewById(const scene_id id) const {
            std::list<MSceneGraph>::const_iterator itr = childs.begin(), end = childs.end();
            while(itr != end) {
                jc_sp<View> view = downcast<View>(*itr);
                if(view) {
                    if(view->getUniqueId() == id && view->isEnable()) {
                        return view;
                    }

                    // 子も探索する
                    view = view->findEnableViewById(id);
                    if(view) {
                        // 孫以降がヒットした
                        return view;
                    }
                }
                ++itr;
            }

            return jc_sp<View>();
        }

        /**
         * 有効なViewを探して返す
         */
        virtual jc_sp<View> findDisableViewById(const scene_id id) const {
            std::list<MSceneGraph>::const_iterator itr = childs.begin(), end = childs.end();
            while(itr != end) {
                jc_sp<View> view = downcast<View>(*itr);
                if(view) {
                    if(view->getUniqueId() == id && !view->isEnable()) {
                        return view;
                    }

                    // 子も探索する
                    view = view->findDisableViewById(id);
                    if(view) {
                        // 孫以降がヒットした
                        return view;
                    }
                }
                ++itr;
            }

            return jc_sp<View>();
        }

        /**
         * 自分の子からフォーカスのインデックスを取得する
         */
        virtual s32 getFocusChildViewIndex(const jcboolean recursive) const {
            std::list<MSceneGraph>::const_iterator itr = childs.begin(), end = childs.end();

            s32 index = 0;
            while(itr != end) {
                jc_sp<View> view = downcast<View>(*itr);
                if(view && view->hasFocus(recursive)) {
                    return index;
                }
                ++index;
                ++itr;
            }

            return -1;
        }

        /**
         * 有効になるレンダリングパスを設定する
         * 0〜31までのパスをビットフラグを用いて設定している。
         * default = 0x0
         */
        virtual void addRenderingPass(const s32 set) {
            this->enableRenderingPass |= (0x1 << set);
        }

        /**
         * レンダリングパスを無効にする
         */
        virtual void resetRenderingPass() {
            this->enableRenderingPass = 0;
        }

        /**
         * すべてのViewのフォーカス値がゼロであることを確認する
         */
        virtual jcboolean isAllFocusWeightZero(const jcboolean recursive = jctrue) const;

        /**
         * クリック時のリスナを設定する
         */
        virtual void setOnClickListener(const SOnClickListener listener) {
            this->onClickListener = listener;
        }

    protected:
        // オーバーライドされる

        /**
         * レイアウトが変更された
         */
        virtual void onLayoutChanged(const RectF &newArea) {
        }

        /**
         * 自分自身のレンダリングを行う
         */
        virtual void onSelfRendering();

        /**
         * ウィンドウと関連付けがされた
         */
        virtual void onRegisteredWindow() {
        }

        /**
         * イベントハンドリングを行う
         */
        virtual void onEvent(MEvent event) {
        }

        /**
         * クリックされた
         */
        virtual void onClick( );

        /**
         * フォーカス状態が更新された
         */
        virtual void onFocusChanged(const jcboolean has);

        /**
         * 有効・無効状態が変更された
         */
        virtual void onEnableChanged(const jcboolean enable) {

        }

        /**
         * フォーカスダウン状態が更新された
         */
        virtual void onDownChanged(const jcboolean down_now);

        /**
         * キーが押下された
         */
        virtual void onKeyDown(const MKeyData keyData) {
        }

        /**
         * キーが離された
         */
        virtual void onKeyUp(const MKeyData keyData) {
        }
    protected:
        // 基本制御系

        /**
         * どれかのViewがクリックされたらハンドリングを行う
         */
        virtual void dispatchClickEvent(const jc_sp<View> clicked);

        /**
         * どれかのキーが押された
         */
        virtual void dispatchKeyEvent(const MKeyData keyData);

        /**
         * ダウン状態の更新を行う
         */
        virtual void dispatchDownEvent(const jcboolean down);

        /**
         * このViewにフォーカスが当たるようにリクエストされた
         */
        virtual void dispatchRecuestFocus(const jcboolean has);

        /**
         * 送信されたイベントを処理する
         */
        virtual void dispatchEvent(MEvent event);

        /**
         * 自分自身を示すMViewを取得する。
         * 循環参照に注意すること。
         */
        virtual jc_sp<View> getSelfManagedObject();
    };

    /**
     * managed
     */
typedef jc_sp<View> MView;

}
}

#endif /* VIEW_H_ */
