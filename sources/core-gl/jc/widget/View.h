/*
 * View.h
 *
 *  Created on: 2013/04/11
 */

#ifndef VIEW_H_
#define VIEW_H_

#include    "jc/math/Vec2.h"
#include    "jc/math/Rect.h"
#include    "jc/scene/SceneGraph.h"
#include    "jc/widget/event/Event.h"
#include    "jc/widget/window/WindowContext.h"
#include    "jc/math/Counter.h"
#include    "jc/widget/anim/TransactionCounter.h"
//#include    "jc/ui/TouchPoint.h"

namespace jc {
namespace view {

class WindowManager;

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
    friend class WindowManager;
    friend class WindowTouchDetector;
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
     * ダウンを行なっている途中
     */
    jcboolean down;

    /**
     * 有効状態
     */
    jcboolean enable;

    /**
     * Viewの表示状態を取得する
     */
    ViewMode_e viewMode;

protected:

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
     * デバッグ用にレンダリングを行う
     */
    virtual void renderingArea();
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
     * 可視状態描画用のカウントを取得sルウ
     */
    virtual float getVisibleWeight() const;

    /**
     * フォーカス描画用のカウントを取得する
     */
    virtual float getFocusWeight(const jcboolean withVisibleCounter = jctrue) const {
        return focusCounter.getValue() * (withVisibleCounter ? getVisibleWeight() : 1.0f);
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
    virtual RectF getLocalLayoutArea() {
        return localArea;
    }

    /**
     * 衝突判定位置（ローカル座標）を取得する
     */
    virtual RectF getLocalIntersectArea() {
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
     * ネストされた小階層も含めた全体のレイアウトエリアを計算する
     */
    virtual RectF getGlobalLayoutAreaNest( );

    /**
     * グローバル座標に変換した表示位置を取得する
     */
    virtual RectF getGlobalLayoutArea() {
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
     * グローバル座標に変換した衝突判定位置を取得する
     */
    virtual RectF getGlobalIntersectArea() {
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
    virtual jcboolean isIntersect(const Vector2f &global) {
        const RectF globalRect = getGlobalIntersectArea();
        return globalRect.isIntersect(global.x, global.y);
    }

    /**
     * グローバル座標にタッチされた時に自分が処理すべきViewであるならtrueを返す。
     * 子は考慮しない。
     */
    virtual jcboolean isTouchedView(const Vector2f &global) {
        if (!isTouchable()) {
            return jcfalse;
        }
        return isIntersect(global);
    }

    /**
     * 位置に適合するViewを子から探して返す。
     * 自分自身は含めない。 isTouchedView()が基準となる。
     */
    virtual jc_sp<View> findTouchedView( const Vector2f &global) {
        std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();

        // 全チェックを行う
        while(itr != end) {

            jc_sp<View> check = downcast<View>(*itr);

            // 子がViewであるならチェックする
            if(check) {
                if(check->isTouchedView(global)) {
                    // タッチ対象になったからそれを返す
                    return check;
                }
                // 子の、更に子をチェックする
                check = check->findTouchedView(global);
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
    virtual void onRegisterdWindow() {
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
protected:
    // 基本制御系

    /**
     * どれかのViewがクリックされたらハンドリングを行う
     */
    virtual void dispatchClickEvent(const jc_sp<View> clicked);

    /**
     * ダウン状態の更新を行う
     */
    virtual void dispatchDownEvent(const jcboolean down);

    /**
     * フォーカスを失ってほしいリクエスト
     */
    virtual void dispatchLostFocusRequestEvent(const MEvent event);

    /**
     * 送信されたイベントを処理する
     */
    virtual void dispatchEvent(MEvent event);
};

/**
 * managed
 */
typedef jc_sp<View> MView;

}
}

#endif /* VIEW_H_ */
