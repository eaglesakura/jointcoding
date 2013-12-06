/*
 * WindowStack.hpp
 *
 *  Created on: 2013/12/06
 */
#ifndef WINDOWSTACK_HPP_
#define WINDOWSTACK_HPP_

#include    "jc/widget/window/WindowManager.h"

namespace jc {

namespace view {

class WindowStack: public Object {
    typedef typename std::list<MWindowManager> window_stack;

    jcmutex mutex;

    /**
     *
     */
    window_stack stack;
public:
    WindowStack() {
    }

    virtual ~WindowStack() {
    }

    /**
     * 最上部へウィンドウを追加する
     */
    virtual void push(MWindowManager window) {
        MutexLock lock(mutex);
        stack.push_front(window);
    }

    virtual jcboolean empty() const {
        MutexLock lock(mutex);
        return stack.empty();
    }

    /**
     * 最上部のウィンドウを取得する
     */
    virtual const MWindowManager current() const {
        MutexLock lock(mutex);

        if (!stack.empty()) {
            return stack.front();
        } else {
            return MWindowManager();
        }
    }

    /**
     * 最上部のウィンドウを取得する
     */
    virtual MWindowManager current() {
        MutexLock lock(mutex);

        if (!stack.empty()) {
            return stack.front();
        } else {
            return MWindowManager();
        }
    }

    /**
     * 最上部のウィンドウを廃棄する
     */
    virtual MWindowManager pop() {
        MutexLock lock(mutex);
        assert(!stack.empty());

        MWindowManager result = current();
        stack.pop_front();

        return result;
    }
};

}

}

#endif /* WINDOWSTACK_HPP_ */
