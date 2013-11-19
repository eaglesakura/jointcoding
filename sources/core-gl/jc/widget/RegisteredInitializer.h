/*
 * RegisteredAction.h
 *
 *  Created on: 2013/04/14
 */

#ifndef REGISTEREDACTION_H_
#define REGISTEREDACTION_H_

#include    "jointcoding.h"
#include    "jc/widget/View.h"
#include    "jc/widget/anim/WindowTimer.h"

namespace jc {
namespace view {

class View;
class RegisteredInitializer: public Object {

protected:
    RegisteredInitializer() {
        jcmark(this);
    }
public:
    virtual ~RegisteredInitializer() {
        jcunmark(this);
    }

    virtual void onRegisteredWindow(View *view, MWindowContext windowContext) = 0;
};

/**
 * managed
 */
typedef jc_sp<RegisteredInitializer> MRegisteredAction;

/**
 * トランザクション初期化用のイニシャライザ
 */
class TransactionInitializer: public RegisteredInitializer {
    jc_selp<TransactionCounter> target;

    /**
     * 遷移時間
     */
    float transactionTimeSec;

    /**
     * 遷移タイプ
     */
    LeapType_e leapType;
public:
    TransactionInitializer(const jc_selp<TransactionCounter> target, const float transTimeSec, const LeapType_e type = LeapType_Ease1 ) {
        assert(target.get() != NULL);

        this->target = target;
        this->transactionTimeSec = transTimeSec;
        this->leapType = type;

        jcmark(this);
    }

    virtual ~TransactionInitializer() {
        jcunmark(this);
    }

    virtual void onRegisteredWindow(View *view, MWindowContext windowContext) {
        assert(target.exist());
        target->initialize(windowContext, transactionTimeSec, leapType);
    }
};

/**
 * 時限タイマーの初期化を行う
 */
class TimerInitializer: public RegisteredInitializer {

    /**
     * 設定用タイマー
     */
    jc_selp<WindowTimer> target;
public:
    TimerInitializer(const jc_selp<WindowTimer> target, const u32 timeMilliSec ) {
        assert(target.exist());
        this->target = target;

        WindowTimer *timer = target.get();
        timer->setTimerMS(timeMilliSec);

        jcmark(this);
    }

    virtual ~TimerInitializer() {
        jcunmark(this);
    }

    virtual void onRegisteredWindow(View *view, MWindowContext windowContext) {

    }
};

}
}

#endif /* REGISTEREDACTION_H_ */
