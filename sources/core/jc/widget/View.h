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

namespace jc {
namespace view {

class View: SceneGraph {
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

protected:
    /**
     * Viewの配置
     */
    RectF localArea;
public:
    View();
    virtual ~View();

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
        return focusable;
    }

    /**
     * タッチを処理すべきViewである場合はtrueを返す
     */
    virtual jcboolean isTouchable() const {
        return touchable;
    }

    /**
     * 配置場所（ローカル位置）を取得する
     */
    virtual RectF getLocalArea() {
        return localArea;
    }

    /**
     * グローバル座標に変換した位置を取得する
     */
    virtual RectF getGlobalArea() {
        RectF parent;
        if (getParent()) {
            parent = getParentTo<View>()->getGlobalArea();
        }
        RectF local = getLocalArea();
        local.offset(parent.left, parent.top);
        return local;
    }

    /**
     * グローバル座標を指定し、衝突していればtrueを返す
     */
    virtual jcboolean isIntersect(const Vector2f &global) {
        const RectF globalRect = getGlobalArea();
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

            // 次の子をチェックする
            ++itr;
        }

        // 何も見つからなかった
        return jc_sp<View>();
    }
protected:
};

}
}

#endif /* VIEW_H_ */
