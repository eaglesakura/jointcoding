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
#include    "jc/widget/window/WindowContext.h"

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
 */
class View: public SceneGraph {
private:
    friend class WindowManager;
    /**
     * フォーカスを持つことが出来る場合true
     */
    jcboolean focusable;

    /**
     * フォーカスを持っている場合true
     */
    jcboolean focus;

    /**
     * タッチを行える場合true
     */
    jcboolean touchable;

    /**
     * Viewの表示状態を取得する
     */
    ViewMode_e viewMode;
protected:
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
     * ウィンドウとViewを関連付ける。
     * 必ずWindow（もしくは子View）にaddされた後に呼び出す。
     * 関連付けない場合、WindowContextを得られない。
     */
    virtual void registerWindow();

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
     * フォーカスを持っている場合はtrueを返す
     */
    virtual jcboolean hasFocus() const {
        return focus;
    }

    /**
     * フォーカスを持つことができるViewの場合はtrueを返す
     */
    virtual jcboolean isFocusable() const {
        return focusable && isVisible();
    }

    /**
     * タッチを処理すべきViewである場合はtrueを返す
     */
    virtual jcboolean isTouchable() const {
        return touchable && isVisible();
    }

    /**
     * 配置場所（ローカル位置）を取得する
     */
    virtual RectF getLocalRenderingArea() {
        return localArea;
    }

    /**
     * 衝突判定位置（ローカル座標）を取得する
     */
    virtual RectF getLocalIntersectArea() {
        return localArea;
    }

    /**
     * レイアウトを更新する。
     */
    virtual void layout(const RectF &area);

    /**
     * グローバル座標に変換した表示位置を取得する
     */
    virtual RectF getGlobalRenderingArea() {
        RectF parent;
        if (getParent()) {
            parent = getParentTo<View>()->getGlobalRenderingArea();
        }
        RectF local = getLocalRenderingArea();
        local.offset(parent.left, parent.top);
        return local;
    }

    /**
     * グローバル座標に変換した衝突判定位置を取得する
     */
    virtual RectF getGlobalIntersectArea() {
        RectF parent;
        if (getParent()) {
            parent = getParentTo<View>()->getGlobalRenderingArea();
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
        if (!isFocusable() || !isTouchable()) {
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

            jc_sp<View> check = jc_dynamic_cast<View>(*itr);

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
    /**
     * レイアウトが変更された
     */
    virtual void onLayoutChanged(const RectF &newArea) {
    }

    /**
     * 自分自身のレンダリングを行う
     */
    virtual void onSelfRendering();
};

/**
 * managed
 */
typedef jc_sp<View> MView;

}
}

#endif /* VIEW_H_ */
