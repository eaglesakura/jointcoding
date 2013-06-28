/*
 * Window.h
 *
 *  Created on: 2013/04/11
 */

#ifndef GLVIEW_WINDOW_H_
#define GLVIEW_WINDOW_H_

#include    "jc/widget/View.h"

namespace jc {
namespace view {

/**
 * すべてのViewを入れるためのRootとして機能する
 */
class Window: public Object {
    static void broadCastEvent(MView view, MEvent event);

    /**
     * root view
     */
    MView root;
public:
    Window() {
    }

    virtual ~Window() {
        jclogf("delete Window(%x)", this);
    }

    virtual void initialize(MWindowContext context) {
        this->root.reset(new View(context));
        root->setUniqueId(ViewID_ROOT);
    }

    /**
     * 全Viewにイベントを伝える
     */
    virtual void broadcastEvent(MEvent event);

    /**
     * 管理用のRootViewを取得する
     */
    virtual MView getRootView() const {
        return root;
    }

    /**
     * レイアウトを変更する
     */
    virtual void layout(const RectF &area);

    /**
     * 特定のレイアウトへイベントを送信する
     * ハンドルに成功したらtrueを返す
     */
    virtual jcboolean sendEvent(const scene_id layoutId, MEvent event);

    /**
     * 特定レイアウトへイベントを送信する
     * ハンドルに成功したらtrueを返す
     */
    virtual jcboolean sendEvent(const scene_id layoutId, const s32 eventType, const jc_sp<Object> extension = jc_sp<Object>()) {
        return sendEvent(layoutId, Event::createEvent(eventType, extension));
    }

    /**
     * フォーカス付きのViewを取得する
     */
    virtual MView findFocusedView() {
        return root->findFocusedView();
    }

    virtual RectF getLocalArea() const {
        return root->getLocalLayoutArea();
    }
};

/**
 * managed
 */
typedef jc_sp<Window> MWindow;

}
}

#endif /* WINDOW_H_ */
