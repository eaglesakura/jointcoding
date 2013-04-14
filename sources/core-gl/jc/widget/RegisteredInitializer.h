/*
 * RegisteredAction.h
 *
 *  Created on: 2013/04/14
 */

#ifndef REGISTEREDACTION_H_
#define REGISTEREDACTION_H_

#include    "jointcoding.h"
#include    "jc/widget/View.h"

namespace jc {
namespace view {

class View;
class RegisteredInitializer: public Object {
public:
    virtual ~RegisteredInitializer() {
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
        this->target = target;
        this->transactionTimeSec = transTimeSec;
        this->leapType = type;
    }

    virtual ~TransactionInitializer() {
    }

    virtual void onRegisteredWindow(View *view, MWindowContext windowContext) {
        assert(target.expired());
        target->initialize(windowContext, transactionTimeSec, leapType);
    }
};

}
}

#endif /* REGISTEREDACTION_H_ */
