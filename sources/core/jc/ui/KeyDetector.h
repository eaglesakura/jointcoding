/*
 * KeyDetector.h
 *
 *  Created on: 2013/04/15
 */

#ifndef KEYDETECTOR_H_
#define KEYDETECTOR_H_

#include    "jc/ui/KeyData.h"

namespace jc {

class KeyDetector;
class KeyListener {
public:
    virtual ~KeyListener() {
    }

    /**
     * キーダウンを開始した
     */
    virtual void onKeyDown(KeyDetector *detector, const MKeyData keyData) = 0;

    /**
     * キー押しっぱなしの状態が継続された。
     */
    virtual void onKeyKeeping(KeyDetector *detector, const MKeyData keyData) = 0;

    /**
     * キーを長押した
     */
    virtual void onKeyLongDown(KeyDetector *detector, const MKeyData keyData) = 0;

    /**
     * キーを離した
     */
    virtual void onKeyUp(KeyDetector *detector, const MKeyData keyData) = 0;
};

typedef jc_selp<KeyListener> SKeyListener;

/**
 * キー解析クラス
 */
class KeyDetector: public Object {
    /**
     * キーデータ
     */
    std::map<s32, MKeyData> keyDatas;

    /**
     * リスナ
     */
    SKeyListener listener;

    /**
     * キーの押しっぱなしメッセージ処理間隔
     * デフォルトは0.25秒
     */
    s32 keyKeepingMessageTime;
public:
    KeyDetector(SKeyListener listener);

    virtual ~KeyDetector();

    /**
     * キーデータを取得する
     */
    virtual MKeyData getKeyData(const s32 keyCode);

    /**
     * キー押しっぱなしの処理間隔を設定する
     * デフォルトは0.25秒
     */
    virtual void setKeyKeepingMessageTimeMS(const s32 timeMS) {
        keyKeepingMessageTime = timeMS;
    }

    /**
     * キー押しっぱなしの処理間隔を設定する
     * デフォルトは0.25秒
     */
    virtual void setKeyKeepingMessageTimeSec(const double sec) {
        setKeyKeepingMessageTimeMS((s32) (sec * 1000));
    }

    /**
     * キーイベントの解析を行う
     */
    virtual void onKeyEvent(KeyEventProtocol *event);

    /**
     * キーのアップデートを行う
     */
    virtual void onHandleEvent();
};

/**
 * managed
 */
typedef jc_sp<KeyDetector> MKeyDetector;

}

#endif /* KEYDETECTOR_H_ */
