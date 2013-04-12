/*
 * Window.h
 *
 *  Created on: 2013/04/11
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include    "jc/widget/View.h"

namespace jc {
namespace view {

/**
 * すべてのViewを入れるためのRootとして機能する
 */
class Window: public View {

    void broadCastEvent(MView view, MEvent event);
public:
    Window(const MWindowContext windowContext) {
        this->windowContext = windowContext;
    }

    virtual ~Window() {
    }

    /**
     * 自分自身のレンダリングを行う
     */
    virtual void onSelfRendering() {
    }

    /**
     * 全Viewにイベントを伝える
     */
    virtual void broadcastEvent(MEvent event);

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

};

/**
 * managed
 */
typedef jc_sp<Window> MWindow;

}
}

#endif /* WINDOW_H_ */
