/*
 * WindowManager.h
 *
 *  Created on: 2013/04/11
 */

#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include    "jc/widget/event/EventQueue.h"
#include    "jc/widget/event/TouchEvent.h"
#include    "jc/widget/window/Window.h"
#include    "jc/widget/window/WindowContext.h"

namespace jc {
namespace view {

class WindowManager: public Object {
    /**
     * イベント管理
     */
    MEventQueue events;

    /**
     * 描画ウィンドウのROOT
     */
    MWindow window;

    /**
     * レンダリング用のコンテキスト
     */
    MWindowContext windowContext;

public:
    WindowManager();
    virtual ~WindowManager();

    /**
     * キューに溜まっているイベントの処理を行う
     */
    virtual void handleEvents();

    /**
     * イベントを追加する
     */
    virtual void addEvent(MEvent event);

    /**
     * イベントリストを取得する
     */
    virtual MEventQueue getEvents() const {
        return events;
    }

    /**
     * ウィンドウ管理補助クラスを取得する
     */
    virtual MWindowContext getWindowContext() const {
        return windowContext;
    }

    /**
     * ウィンドウを取得する
     */
    virtual MWindow getWindow() const {
        return window;
    }

    /**
     * ウィンドウ位置を描画する
     */
    virtual void setWindowArea(const RectF area) const {
        window->layout(area);
    }

    /**
     * 毎フレームの処理を行わせる
     * @param numPass レンダリングするパス数
     */
    virtual void update(const s32 numPass);

    /**
     * レンダリングを行う
     * @param numPass レンダリングするパス数
     */
    virtual void rendering(const s32 numPass);
};

/**
 * managed
 */
typedef jc_sp<WindowManager> MWindowManager;

}
}

#endif /* WINDOWMANAGER_H_ */
