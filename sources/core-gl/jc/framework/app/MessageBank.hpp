/*
 * MessageBank.hpp
 *
 *  Created on: 2013/12/07
 */
#ifndef MESSAGEBANK_HPP_
#define MESSAGEBANK_HPP_

#include    "jointcoding.h"
#include    "jc/framework/app/JointApplicationBase.h"
#include    <map>

namespace jc {

class PendingMessage {
public:
    PendingMessage() {
    }
    ~PendingMessage() {
    }

    /**
     * 配信用key
     */
    gl::ApplicationQueryKey key;

    /**
     * 送信されたメッセージ
     */
    string_vector args;
};
typedef jc_sp<PendingMessage> MPendingMessage;


/**
 * JointApplicationに送信されたメッセージの
 */
class PendingMessages {
    typedef std::list<MPendingMessage> message_list;

    /**
     * 識別用のユニークID
     */
    s32 uniqueId;

    /**
     * 設定されたメッセージ
     */
    message_list messages;

    /**
     * 排他制御
     */
    jcmutex mutex;
public:

    PendingMessages(const s32 uniqueId) {
        this->uniqueId = uniqueId;
    }

    ~PendingMessages() {
    }

    /**
     * メッセージを追加する
     * スレッドセーフに処理される
     */
    void push(const gl::ApplicationQueryKey &key, const unsafe_array<String> &params) {
        MPendingMessage msg(new PendingMessage());
        msg->key = key;

        for (int i = 0; i < params.length; ++i) {
            msg->args.push_back(params[i]);
        }

        {
            MutexLock lock(mutex);
            messages.push_back(msg);
        }
    }

    /**
     * メッセージの先頭を取得する
     * スレッドセーフに処理される
     */
    MPendingMessage pop() {
        MutexLock lock(mutex);

        if (messages.empty()) {
            return MPendingMessage();
        } else {
            MPendingMessage result = messages.front();
            messages.pop_front();
            return result;
        }
    }
};

typedef jc_sp<PendingMessages> MPendingMessages;

/**
 * メッセージを非同期に貯めこんで、必要なときにポーリングするための一時的なキャッシュを形成する。
 */
class MessageBank: public Object {

    typedef std::map<s32, MPendingMessages> message_map;

    /**
     * 取得したメッセージ一覧
     */
    message_map messages;

    /**
     * 排他
     */
    jcmutex mutex;

    MPendingMessages get(const s32 uniqueId) {
        MutexLock lock(mutex);

        message_map::iterator itr = messages.find(uniqueId);
        if (itr != messages.end()) {
            return itr->second;
        } else {
            MPendingMessages pm(new PendingMessages(uniqueId));
            messages.insert(std::pair<s32, MPendingMessages>(uniqueId, pm));
            return pm;
        }
    }
public:
    MessageBank() {

    }
    virtual ~MessageBank() {

    }

    /**
     * バンクIDを指定してプッシュする
     */
    virtual void push(const s32 bankId, const gl::ApplicationQueryKey &key, const unsafe_array<String> &params) {
        get(bankId)->push(key, params);
    }

    /**
     * バンクIDを指定して取得する
     */
    virtual MPendingMessage pop(const u32 bankId) {
        return get(bankId)->pop();
    }
};

}

#endif /* MESSAGEBANK_HPP_ */
