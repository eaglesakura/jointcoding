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

    jcmarkvoid(this);
}

/**
 * 指定したIDから作成する
 */
ThreadID::ThreadID(void* _id) {
    this->threadId = NULL;
    swap(_id);

    jcmarkvoid(this);
}

/**
 * コピーコンストラクタ
 */
ThreadID::ThreadID(const ThreadID &origin) {
    this->threadId = NULL;
    swap(origin.threadId);

    jcmarkvoid(this);
}

ThreadID::~ThreadID() {
    swap(NULL);
//    if (threadId) {
//        thread_id *p = (thread_id) threadId;
//        SAFE_DELETE(p);
//        threadId = NULL;
//    }

    assert(threadId == NULL);
    jcunmarkvoid(this);
}

/**
 * 持っているポインタと交換する
 */
void ThreadID::swap(void* p) {
    if (p) {
        if (!threadId) {
            threadId = (void*) new thread_id();

            jcmarkvoid(threadId);
        }
        thread_id* pId = (thread_id*) (threadId);

        // ポインタを持っているなら代入する
        (*pId) = *((thread_id*) p);
    } else {
        jcunmarkvoid(threadId);

        thread_id *pId = (thread_id*) (threadId);
        SAFE_DELETE(pId);
        threadId = NULL;
    }
}

/**
 * 文字列に直す
 */
jc::String ThreadID::toString() const {
    const u64 rawId = *((u64*) threadId);
//    jclogf("rawId = %d", rawId);
    charactor str[40] = { 0 };
    sprintf(str, "thread_%llu", rawId);
    return str;
}

/**
 * 比較
 */
bool ThreadID::operator==(const ThreadID &id) {
    thread_id*_this = (thread_id*) (threadId);
    thread_id*_id = (thread_id*) (id.threadId);

    if (_this == NULL || _id == NULL) {
        return jcfalse;
    }

    return (*_this) == (*_id);
}

/**
 * 比較
 */
bool ThreadID::operator!=(const ThreadID &id) {
    const thread_id*_this = (thread_id*) (threadId);
    const thread_id*_id = (thread_id*) (id.threadId);

    if (_this && _id) {
        return (*_this) != (*_id);
    }

    return jcfalse;
}

/**
 * 現在実行されているスレッドとThreadIDが一致した場合、trueを返す。
 */
jcboolean ThreadID::isCurrent() {
    const thread_id *id = (thread_id*) threadId;
    if (id == NULL) {
        return jcfalse;
    }
    return boost::this_thread::get_id() == *id;
}

/**
 * 比較
 */
jcboolean ThreadID::equals(const ThreadID *id) {
    if (!id) {
        return jcfalse;
    }
    const thread_id*_this = (thread_id*) (threadId);
    const thread_id*_id = (thread_id*) (id->threadId);

    if (_this == NULL || _id == NULL) {
        return jcfalse;
    }

    return (*_this) == (*_id);
}

}
