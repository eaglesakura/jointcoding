/*
 * KeyData.h
 *
 *  Created on: 2013/04/15
 */

#ifndef KEYDATA_H_
#define KEYDATA_H_

#include    "jointcoding.h"
#include    "jc/platform/Timer.h"
#include    "protocol/jcKeyEventProtocol.h"

namespace jc {

/**
 * 個々のキー入力情報を保存する
 */
class KeyData: public Object {
    friend class KeyDetector;

    enum KeyState_e {
        /**
         * キーが上がっている（押されていない）状態
         */
        KeyState_Up,

        /**
         * キーが押されている状態
         */
        KeyState_Down,
    } state;

    /**
     * 押下を始めた時間
     */
    jctime beginTime;

    /**
     * ボタンから手を離した時間
     */
    jctime endTime;

    /**
     * キーコード
     */
    s32 keyCode;

public:
    KeyData(const s32 keyCode) {
        this->state = KeyState_Up;
        this->keyCode = keyCode;
        beginTime = endTime = Timer::currentTime();
    }

    /**
     * 対応するキーコードを取得する
     */
    virtual s32 getKeyCode() const {
        return keyCode;
    }

    /**
     * 押下中の場合はtrueを返す
     */
    virtual s32 isPressing() const {
        return endTime < beginTime;
    }

    /**
     * 押下された秒数を取得する
     */
    virtual s32 getPressTimeMS() const {
        if (endTime < beginTime) {
            // まだ離されていないため、現在時刻とのチェックを行う
            return Timer::lapseTimeMs(beginTime);
        } else {
            // 既に離されているため、既存の時間とのチェックを行う
            return Timer::lapseTimeMs(beginTime, endTime);
        }
    }

    /**
     * カーソル（上下左右）用のキーだったらtrueを返す
     */
    virtual jcboolean isCursorKey() {
        return keyCode == KeyEventProtocol::KEYCODE_LEFT //
        || keyCode == KeyEventProtocol::KEYCODE_RIGHT //
        || keyCode == KeyEventProtocol::KEYCODE_UP //
        || keyCode == KeyEventProtocol::KEYCODE_DOWN;
    }

    virtual jcboolean isKeyDown() const {
        return keyCode == KeyEventProtocol::KEYCODE_DOWN;
    }

    virtual jcboolean isKeyUp() const {
        return keyCode == KeyEventProtocol::KEYCODE_UP;
    }

    virtual jcboolean isKeyLeft() const {
        return keyCode == KeyEventProtocol::KEYCODE_LEFT;
    }

    virtual jcboolean isKeyRight() const {
        return keyCode == KeyEventProtocol::KEYCODE_RIGHT;
    }

    /**
     * 垂直方向のキー制御ならtrue
     */
    virtual jcboolean isKeyVertical() const {
        return isKeyDown() || isKeyUp();
    }

    /**
     * 水平方向のキー制御ならtrue
     */
    virtual jcboolean isKeyHorizontal() const {
        return isKeyLeft() || isKeyRight();
    }

    /**
     * 決定キーに相当するキー制御ならtrue
     */
    virtual jcboolean isEnterKey() const {
        return keyCode == KeyEventProtocol::KEYCODE_ENTER;
    }

private:
    /**
     * キーが押された
     */
    jcboolean onKeyDown() {
        if (state != KeyState_Down) {
            beginTime = Timer::currentTime();
            state = KeyState_Down;
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * キーが離された
     */
    jcboolean onKeyUp() {
        if (state != KeyState_Up) {
            endTime = Timer::currentTime();
            state = KeyState_Up;
            return jctrue;
        }
        return jcfalse;
    }
};

/**
 * managed
 */
typedef jc_sp<KeyData> MKeyData;

}

#endif /* KEYDATA_H_ */
