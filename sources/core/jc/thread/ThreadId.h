/*
 * jcThreadId.h
 *
 *  Created on: 2012/07/11

 */

#ifndef JCTHREADID_H_
#define JCTHREADID_H_
#include    "jointcoding.h"

namespace jc {

/**
 * スレッドごとのIDを管理する
 */
class ThreadID {

    /**
     * ThreadIDの実装を隠蔽する
     */
    typedef void* NativeThreadIdPtr;

    /**
     * 管理する生情報
     */
    NativeThreadIdPtr threadId;

    /**
     * 持っているポインタと交換する
     */
    void swap(NativeThreadIdPtr p);

public:
    /**
     * 現在のスレッドIDを生成する
     */
    ThreadID();

    /**
     * 指定したIDから作成する
     */
    ThreadID(NativeThreadIdPtr _id);

    /**
     * コピーコンストラクタ
     */
    ThreadID(const ThreadID &origin);

    ~ThreadID();

    /**
     * 文字列に直す
     */
    String toString() const;

    /**
     * 代入
     */
    ThreadID& operator=(const ThreadID &origin) {
        swap(origin.threadId);
        return (*this);
    }

    /**
     * 比較
     */
    bool operator==(const ThreadID &id);

    /**
     * 比較
     */
    bool operator!=(const ThreadID &id);

    /**
     * 比較
     */
    jcboolean equals(const ThreadID *id);
    /**
     * 比較
     */
    jcboolean equals(const jc_sp<ThreadID> id) {
        return equals(id.get());
    }

    /**
     * 現在実行されているスレッドとThreadIDが一致した場合、trueを返す。
     */
    jcboolean isCurrent( );
};

    /**
     * managed
     */
typedef jc_sp<ThreadID> MThreadID;

}

#endif /* JCTHREADID_H_ */
