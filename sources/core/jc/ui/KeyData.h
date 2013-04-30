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
     * 押しっぱなしチェックを行った時間
     */
    jctime checkedTime;

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
    virtual jcboolean isPressing() const {
        return endTime < beginTime;
    }

    /**
     * キーを押しっぱなしにしている場合はtrue
     */
    virtual jcboolean isKeeping() const {
        if(!isPressing()) {
            // 押されていなければKEEPもされてない
            return jcfalse;
        }

        // 押し始めた時間がチェック時間よりも先なら、経過している
        return beginTime < checkedTime;
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

    virtual jcboolean isDownKey() const {
        return keyCode == KeyEventProtocol::KEYCODE_DOWN;
    }

    virtual jcboolean isUpKey() const {
        return keyCode == KeyEventProtocol::KEYCODE_UP;
    }

    virtual jcboolean isLeftKey() const {
        return keyCode == KeyEventProtocol::KEYCODE_LEFT;
    }

    virtual jcboolean isRightKey() const {
        return keyCode == KeyEventProtocol::KEYCODE_RIGHT;
    }

    /**
     * 垂直方向のキー制御ならtrue
     */
    virtual jcboolean isVerticalKey() const {
        return isDownKey() || isUpKey();
    }

    /**
     * 水平方向のキー制御ならtrue
     */
    virtual jcboolean isHorizontalKey() const {
        return isLeftKey() || isRightKey();
    }

    /**
     * 決定キーに相当するキー制御ならtrue
     */
    virtual jcboolean isEnterKey() const {
        return keyCode == KeyEventProtocol::KEYCODE_ENTER;
    }

    /**
     * 戻るキーに相当するキー制御ならtrue
     */
    virtual jcboolean isBackKey() const {
        return keyCode == KeyEventProtocol::KEYCODE_BACK;
    }

private:
    /**
     * キーが押された
     */
    jcboolean onKeyDown() {
        if (state != KeyState_Down) {
            checkedTime = beginTime = Timer::currentTime();
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
            checkedTime = endTime = Timer::currentTime();
            state = KeyState_Up;
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * キーの押しっぱなしチェックを行う
     * checkTimeMSよりも時間経過していたらtrueを返してチェックタイムをリセットする
     */
    jcboolean isKeyPressing(const s32 checkTimeMS) {
        if (!isPressing()) {
            // 押されていなければ何もしない
            return jcfalse;
        }

        if (Timer::lapseTimeMs(checkedTime) >= checkTimeMS) {
            checkedTime = Timer::currentTime();
            return jctrue;
        } else {
            return jcfalse;
        }
    }
};

/**
 * managed
 */
typedef jc_sp<KeyData> MKeyData;

}

#endif /* KEYDATA_H_ */
