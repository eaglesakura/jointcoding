/*
 * KeyDetector.cpp
 *
 *  Created on: 2013/04/15
 */

#include    "jc/ui/KeyDetector.h"

namespace jc {


KeyDetector::KeyDetector(SKeyListener listener) {
    this->listener = listener;
    this->keyKeepingMessageTime = 0;
    setKeyKeepingMessageTimeSec(0.25);
}

KeyDetector::~KeyDetector() {

}

/**
 * キーデータを取得する
 */
MKeyData KeyDetector::getKeyData(const s32 keyCode) {
    std::map<s32, MKeyData>::iterator itr = keyDatas.find(keyCode);

    if (itr == keyDatas.end()) {
        // キーが見つからなかったから新規生成
        MKeyData data(new KeyData(keyCode));

        // 登録する
        keyDatas.insert(std::map<s32, MKeyData>::value_type(keyCode, data));

        // そのまま帰す
        return data;
    } else {
        assert(itr->second.get() != NULL);
        assert(itr->second->getKeyCode() == itr->first);

        // 既に登録済みだからitrから返す
        return itr->second;
    }
}

/**
 * キーイベントの解析を行う
 */
void KeyDetector::onKeyEvent(KeyEventProtocol *event) {
    assert(event != NULL);
    MKeyData key = getKeyData(event->getKeyCode());
    assert(key.get() != NULL);

    const s32 EVENT_TYPE = event->getEventType();
    if (EVENT_TYPE == KeyEventProtocol::EVENT_TYPE_DOWN) {
        if (key->onKeyDown()) {
            listener->onKeyDown(this, key);
        }
    } else if (EVENT_TYPE == KeyEventProtocol::EVENT_TYPE_UP) {
        if (key->onKeyUp()) {
            listener->onKeyUp(this, key);
        }
    }
}

/**
 * キーのアップデートを行う
 */
void KeyDetector::onHandleEvent() {

    {
        std::map<s32, MKeyData>::iterator itr = keyDatas.begin(), end = keyDatas.end();
        while (itr != end) {

            MKeyData keyData = itr->second;
            assert(keyData);

            // キー押しっぱなしメッセージを飛ばす
            if (keyData->isKeyPressing(keyKeepingMessageTime)) {
                listener->onKeyKeeping(this, keyData);
            }

            ++itr;
        }
    }
}

}

