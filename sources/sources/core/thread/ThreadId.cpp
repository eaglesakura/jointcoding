/*
 * ThreadId.cpp
 *
 *  Created on: 2012/07/11

 */

#include    "jc/thread/ThreadId.h"
#include    <stdlib.h>
#include    "prv_Thread.h"

/**
 * 実装内でのルールを適用する
 */
using namespace boost;
typedef thread::id thread_id;

namespace jc {

/**
 * 現在のスレッドIDを生成する
 */
ThreadID::ThreadID() {
    this->threadId = NULL;
    thread_id id = boost::this_thread::get_id();
    swap(&id);
}

/**
 * 指定したIDから作成する
 */
ThreadID::ThreadID(NativeThreadIdPtr _id) {
    this->threadId = NULL;
    swap(_id);
}

/**
 * コピーコンストラクタ
 */
ThreadID::ThreadID(const ThreadID &origin) {
    this->threadId = NULL;
    swap(origin.threadId);
}

ThreadID::~ThreadID() {
    swap(NULL);
}

/**
 * 持っているポインタと交換する
 */
void ThreadID::swap(NativeThreadIdPtr p) {
    if (p) {
        if (!threadId) {
            threadId = (NativeThreadIdPtr) new thread_id();
        }
        thread_id* pId = (thread_id*) (threadId);

        // ポインタを持っているなら代入する
        (*pId) = *((thread::id*) p);
    } else {
        thread_id *pId = (thread_id*) (threadId);
        SAFE_DELETE(pId);
        threadId = NULL;
    }
}

/**
 * 文字列に直す
 */
jc::String ThreadID::toString() const {
    s32 rawId = *((s32*) threadId);
//    jclogf("rawId = %d", rawId);
    charactor str[25] = { 0 };
    sprintf(str, "thread_%u", rawId);
    return str;
}

/**
 * 比較
 */
bool ThreadID::operator==(const ThreadID &id) {
    thread_id*_this = (thread_id*) (threadId);
    thread_id*_id = (thread_id*) (id.threadId);

    return (*_this) == (*_id);
}

/**
 * 比較
 */
bool ThreadID::operator!=(const ThreadID &id) {
    thread_id*_this = (thread_id*) (threadId);
    thread_id*_id = (thread_id*) (id.threadId);

    return (*_this) != (*_id);
}

/**
 * 現在実行されているスレッドとThreadIDが一致した場合、trueを返す。
 */
jcboolean ThreadID::isCurrent() {
    return boost::this_thread::get_id() == *((thread_id*) threadId);
}

/**
 * 比較
 */
jcboolean ThreadID::equals(const ThreadID *id) {
    if (!id) {
        return jcfalse;
    }
    thread_id*_this = (thread_id*) (threadId);
    thread_id*_id = (thread_id*) (id->threadId);

    return (*_this) == (*_id);
}

}
