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
class Window: public View {
    static void broadCastEvent(MView view, MEvent event);
public:
    Window(const MWindowContext windowContext) {
        this->windowContext = windowContext;
    }

    virtual ~Window() {
        jclogf("delete Window(%x)", this);
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

    /**
     * 遷移カウンターを更新する
     * View管理用が一括で更新される
     * 現在のvalueは維持される。
     */
    virtual void setWeightCounter(const float leapTimeSec);

};

    /**
     * managed
     */
typedef jc_sp<Window> MWindow;

}
}

#endif /* WINDOW_H_ */
