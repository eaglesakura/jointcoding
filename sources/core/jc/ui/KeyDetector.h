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
public:
    KeyDetector(SKeyListener listener);

    virtual ~KeyDetector();

    /**
     * キーデータを取得する
     */
    virtual MKeyData getKeyData(const s32 keyCode);

    /**
     * キーイベントの解析を行う
     */
    virtual void onKeyEvent(KeyEventProtocol *event);
};

/**
 * managed
 */
typedef jc_sp<KeyDetector> MKeyDetector;

}

#endif /* KEYDETECTOR_H_ */
