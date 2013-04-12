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
#include    "jc/widget/window/WindowTouchDetector.h"

namespace jc {
namespace view {

class WindowEventHandler {
public:
    virtual ~WindowEventHandler() {
    }

    /**
     * イベントハンドルをインターセプトする。
     * trueを返した場合、ハンドリングに成功したとみなしてWindowManagerはそのeventのハンドリングを行わない。
     */
    virtual jcboolean handleEvent(MEvent event) = 0;
};

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

    /**
     * ウィンドウ専用のタッチ判断クラス
     */
    MWindowTouchDetector windowTouchDetector;

    /**
     * ウィンドウの実解析を行う
     */
    MTouchDetector touchDetector;
protected:
    /**
     * タッチイベントを処理する
     */
    virtual void handleTouchEvent(MEvent event);

public:
    WindowManager();
    virtual ~WindowManager();

    /**
     * キューに溜まっているイベントの処理を行う
     * 適当なスレッドから毎フレームポーリングすることを前提とする。
     * スレッドはプラットフォーム固有のメインスレッドである必要はない。
     *
     * @param listener イベントハンドルのリスナ。NULLを渡した場合は自動チェックを行う。
     */
    virtual void handleEvents(WindowEventHandler *listener = NULL);

    /**
     * イベントを追加する。
     * マルチスレッドでの呼び出しを想定して組まれているため、内部的には安全に排他処理を行う。
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
     * WindowへViewを追加する
     */
    virtual void addView(const MView view);

    /**
     * ウィンドウ位置を描画する
     */
    virtual void setWindowArea(const RectF &area) const {
        window->layout(area);
    }

    /**
     * ウィンドウ位置を描画する
     */
    virtual void setWindowArea(const RectI &area) const {
        window->layout(createRectFromLTRB<float>(area.left, area.top, area.right, area.bottom));
    }

    /**
     * 毎フレームの処理を行わせる
     * @param numPass レンダリングするパス数
     */
    virtual void update(const s32 numPass = 1);

    /**
     * レンダリングを行う
     * @param numPass レンダリングするパス数
     */
    virtual void rendering(const s32 numPass = 1);

    /**
     * 特定のレイアウトへイベントを送信する
     * ハンドルに成功したらtrueを返す
     */
    virtual jcboolean sendEvent(const scene_id layoutId, MEvent event) {
        return window->sendEvent(layoutId, event);
    }

    /**
     * 特定レイアウトへイベントを送信する
     * ハンドルに成功したらtrueを返す
     */
    virtual jcboolean sendEvent(const scene_id layoutId, const s32 eventType, const jc_sp<Object> extension = jc_sp<Object>()) {
        return window->sendEvent(layoutId, eventType, extension);
    }

    /**
     * 特定のレイアウトへイベントを送信する
     * ハンドルに成功したらtrueを返す
     */
    virtual void broadcastEvent(MEvent event) {
        return window->broadcastEvent(event);
    }

    /**
     * 特定レイアウトへイベントを送信する
     * ハンドルに成功したらtrueを返す
     */
    virtual void broadcastEvent(const s32 eventType, const jc_sp<Object> extension = jc_sp<Object>()) {
        return window->broadcastEvent(Event::createEvent(eventType, extension));
    }
};

    /**
     * managed
     */
typedef jc_sp<WindowManager> MWindowManager;

}
}

#endif /* WINDOWMANAGER_H_ */
