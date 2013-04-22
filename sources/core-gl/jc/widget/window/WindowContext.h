/*
 * WindowContext.h
 *
 *  Created on: 2013/04/11
 */

#ifndef WINDOWCONTEXT_H_
#define WINDOWCONTEXT_H_

#include    "jointcoding.h"
#include    "jc/gl/GL2D.h"
#include    "jc/scene/SceneGraph.h"

#include    "jc/ui/TouchDetector.h"
#include    "jc/ui/KeyDetector.h"
#include    "jc/scene/LoopController.h"
#include    "jc/widget/event/EventQueue.h"

namespace jc {
namespace view {

using namespace gl;

class Window;
class View;
/**
 * View階層を維持するために必要なWindow共通情報を設定する
 */
class WindowContext: public Object {
    friend class WindowManager;

    /**
     * Window本体.
     */
    jc_wp<Window> window;

    /**
     * タッチ対象となっているView
     */
    jc_wp<View> touchTarget;

    /**
     * レンダリング用のスプライトマネージャ
     */
    MSpriteManager spriteManager;

    /**
     * ループ状態を管理するカウンタ
     */
    LoopController loopController;

    /**
     * イベント管理
     */
    MEventQueue events;

    /**
     * WindowContextが管理するシステム時間
     * 毎フレームgetFrameElapsedSec()の時間だけ加算される。
     * 可変フレームレートかつ、フレーム落ち無視のチェックに利用する。
     */
    jctime windowTime;

    void onWindowLoopBegin() {
        loopController.beginFrame();
        // 前の時間からの経過時間を記録する
        windowTime = Timer::delayTime(windowTime, loopController.getElapsedSec());
    }

    void onWindowLoopEnd() {
        loopController.endFrame(NULL, NULL); // コントローラーに終了を伝える
    }

public:
    WindowContext() {
        windowTime = Timer::currentTime();
    }

    virtual ~WindowContext() {
        jclogf("delete WindowContext(%x)", this);
    }

    /**
     * 一時的にWindowへアクセスする
     */
    virtual jc_sp<Window> lockWindow() const {
        return window.lock();
    }

    /**
     * ウィンドウを設定する
     */
    virtual void setWindow(jc_wp<Window> window) {
        this->window = window;
    }

    /**
     * イベントキュー管理クラスを設定する
     */
    virtual void setEventQueue(const MEventQueue events) {
        this->events = events;
    }

    /**
     * スプライト管理クラスを取得する
     */
    virtual MSpriteManager getSpriteManager() const {
        return spriteManager;
    }

    /**
     * レンダリングデバイスを取得する
     */
    virtual MDevice getDevice() const {
        return spriteManager->getDevice();
    }

    /**
     * スプライト管理クラスを登録する
     */
    virtual void setSpriteManager(const MSpriteManager spriteManager) {
        this->spriteManager = spriteManager;
    }

    /**
     * タッチを当てられているViewを設定する
     */
    virtual void setTouchTarget( const jc_sp<View> view ) {
        touchTarget = view;
    }

    /**
     * タッチ対象をなくす
     */
    virtual void clearTouchTarget();

    /**
     * タッチイベントのターゲットとなるViewを取得する
     */
    virtual jc_sp<View> lockTouchTarget( ) const {
        return touchTarget.lock();
    }

    /**
     * フォーカスを持っている場合はtrue
     */
    virtual jcboolean hasFocus() {
        return !touchTarget.expired();
    }

    /**
     * 特定のViewを検索する
     * 循環参照に注意をすること
     */
    virtual jc_sp<View> findViewById(const scene_id id);

    /**
     * 特定のViewを検索する
     * 循環参照に注意をすること
     */
    virtual jc_sp<View> findEnableViewById(const scene_id id);

    /**
     * 特定シーンをルートから検索する
     */
    template<typename T>
    jc_sp<T> findViewTo(const scene_id id) {
        return downcast<T>(findViewById(id));
    }

    /**
     * 前のフレームから現在の時間までの経過秒を取得する
     */
    virtual double getFrameElapsedSec() const {
        return loopController.getElapsedSec();
    }

    /**
     * イベントハンドラの最後尾にイベントを投げる
     * 実行されるまではタイムラグがある
     */
    virtual void sendEvent(const MEvent event) {
        events->pushBackEvent(event);
    }
    /**
     * イベントハンドラの最前列にイベントを投げる
     * 実行されるまではタイムラグがある
     */
    virtual void interruptionEvent(const MEvent event) {
        events->pushFrontEvent(event);
    }

    /**
     * ウィンドウコンテキストが保持する現在時刻を取得する。
     * フレーム落ちが発生すると、現実時刻よりも少しずつ遅くなっていく。
     */
    virtual const jctime& systemTime() const {
        return windowTime;
    }

    /**
     * 秒間パラメーターをフレームパラメーターに変換する
     */
    template<typename T>
    T frameValue(const T secValue) {
        return loopController.sec2frame<T>(secValue);
    }
};

/**
 * context
 */
typedef jc_sp<WindowContext> MWindowContext;}
}

#endif /* WINDOWCONTEXT_H_ */
