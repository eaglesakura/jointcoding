/*
 * WindowManager.h
 *
 *  Created on: 2013/04/11
 */

#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include    "jc/widget/event/EventQueue.h"
#include    "jc/widget/event/KeyEvent.h"
#include    "jc/widget/event/TouchEvent.h"
#include    "jc/widget/event/RequestFocusEvent.h"
#include    "jc/widget/window/Window.h"
#include    "jc/widget/window/WindowContext.h"
#include    "jc/widget/window/WindowEventListener.h"

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

    /**
     * 定期コールイベントのハンドリングを行う
     * elapsed_sec 前回呼び出しからの経過秒
     */
    virtual void handleTickEvent(const double elapsed_sec) {
    }
};

/**
 * selection
 */
typedef jc_selp<WindowEventHandler> SWindowEventHandler;

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
    MWindowEventListener windowEventListener;

    /**
     * タッチイベントの実解析を行う
     */
    MTouchDetector touchDetector;

    /**
     * キーイベントの実解析を行う
     */
    MKeyDetector keyDetector;

    /**
     * 拡張用のウィンドウハンドラ
     */
    SWindowEventHandler eventHandler;

#if 0
    struct TickCall {
        /**
         * 定期呼び出しを行う間隔
         * 負の値の場合、定期コールを行わない
         */
        double tickCallTime;

        /**
         * 最後に定期コールを行った時刻
         * tickCallTimeが負の値の場合、定期コールを行わない
         */
        jctime lastTickTime;

        /**
         * 呼び出しID
         */
        u32 id;
    };

    /**
     * 呼び出しを行うコールバックタイム一覧
     */
    std::list<TickCall> ticks;
#endif

    struct {
        /**
         * 定期呼び出しを行う間隔
         * 負の値の場合、定期コールを行わない
         */
        double tickCallTime;

        /**
         * 最後に定期コールを行った時刻
         * tickCallTimeが負の値の場合、定期コールを行わない
         */
        jctime lastTickTime;
    } tick;
protected:
    /**
     * タッチイベントを処理する
     */
    virtual void handleTouchEvent(MEvent event);

    /**
     * キーイベントを処理する
     */
    virtual void handleKeyEvent(MEvent event);

    /**
     * フォーカスリクエストイベント
     */
    virtual void handleRequestFocusEvent(MEvent event);

    /**
     * 個々のイベント対応を行う
     */
    virtual void dispatchEvent(MEvent event);

public:
    WindowManager();
    virtual ~WindowManager();

    /**
     * イベントハンドラを設定する
     */
    virtual void setWindowEventHandler(const SWindowEventHandler windowEventHandler) {
        this->eventHandler = windowEventHandler;
    }

    /**
     * キューに溜まっているイベントの処理を行う
     * 適当なスレッドから毎フレームポーリングすることを前提とする。
     * スレッドはプラットフォーム固有のメインスレッドである必要はない。
     *
     * @param listener イベントハンドルのリスナ。NULLを渡した場合は自動チェックを行う。
     */
    virtual void handleEvents();

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
     * WindowManagerの処理ループ開始を宣言する
     *
     * loopBegin() -> update(nPass) -> rendering(nPass) -> loopEnd(withPost)
     *
     */
    virtual void loopBegin();

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
     * レンダリングの終了を宣言する。
     * フロントバッファ転送バッファの可否を引数で設定する
     */
    virtual void loopEnd(const jcboolean withPostFrontBuffer = jctrue);

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

    /**
     * フレームレートの範囲を設定する
     */
    virtual void setFramerateRange( const s32 minFrameRate, const s32 maxFrameRate) {
        windowContext->loopController.setFrameRateRange(minFrameRate, maxFrameRate);
    }

    /**
     * 定期コールイベントを発行させる。
     * 負の値を設定した場合、コールを行わない。
     * @param callback_sec 何秒間隔で呼び出すかのチェック
     */
    virtual void setTickCallbackTime( const double callback_sec);

    /**
     * ユーザーイベント（キーハンドリング、タップイベント）からの経過時刻を取得する
     */
    virtual double getElapsedLastUserEventHandleSec() const {
        return windowEventListener->getElapsedLastEventHandleSec();
    }

    /**
     * 最後のユーザーイベントからの経過時刻をリセットする
     */
    virtual void resetElapsedLastUserEventHandleTime() {
        windowEventListener->resetLastEventHandleTime();
    }
};

    /**
     * managed
     */
typedef jc_sp<WindowManager> MWindowManager;

}
}

#endif /* WINDOWMANAGER_H_ */
