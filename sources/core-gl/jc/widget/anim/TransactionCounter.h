/*
 * TransactionCounter.h
 *
 *  Created on: 2013/04/13
 */

#ifndef TRANSACTIONCOUNTER_H_
#define TRANSACTIONCOUNTER_H_

#include    "jc/widget/window/WindowContext.h"

namespace jc {
namespace view {

/**
 * Viewのウェイト（状態遷移）を管理するカウンタ
 */
class TransactionCounter {
    /**
     * 関連付けられたコンテキスト
     */
    MWindowContext windowContext;

    /**
     * 実カウンター
     */
    CounterF counter;

    /**
     * 0.0 -> 1.0まで遷移させたい時間
     * カウンタの遷移量は毎度フレームレートと時間によってコントロールされる。
     */
    float leapTimeSec;

    /**
     * 値を得るための遷移タイプ
     */
    LeapType_e leapType;
public:
    TransactionCounter() {
        this->leapTimeSec = 1.0f;
        this->leapType = LeapType_Ease1;
        this->counter = createTransitionCounter(60, 1.0);
    }

    TransactionCounter(const TransactionCounter &origin) {
        this->windowContext = origin.windowContext;
        this->counter = origin.counter;
        this->leapTimeSec = origin.leapTimeSec;
        this->leapType = origin.leapType;
    }

    TransactionCounter(MWindowContext context, const float leapTimeSec, const LeapType_e leapType = LeapType_Ease1) {
        this->windowContext = context;
        this->leapTimeSec = leapTimeSec;
        this->leapType = leapType;
    }

    /**
     * 初期化を行う
     */
    void initialize(MWindowContext context, const float leapTimeSec, const LeapType_e leapType = LeapType_Ease1) {
        this->windowContext = context;
        this->leapTimeSec = leapTimeSec;
        this->leapType = leapType;
    }

    /**
     * 値のインクリメントを行う
     */
    TransactionCounter& increment() {
        if (windowContext) {
            const float updateValue = jc::minmax<float>(0, 1, windowContext->getFrameElapsedSec() / leapTimeSec);
            counter.setOffset(updateValue);
        }
        counter.increment();

        return *this;
    }

    /**
     * 値のデクリメントを行う
     */
    TransactionCounter& decrement() {
        if (windowContext) {
            const float updateValue = jc::minmax<float>(0, 1, windowContext->getFrameElapsedSec() / leapTimeSec);
            counter.setOffset(updateValue);
        }
        counter.decrement();

        return *this;
    }

    /**
     * 加算
     */
    TransactionCounter& operator++() {
        return increment();
    }

    /**
     * 加算
     */
    TransactionCounter& operator++(int) {
        return increment();
    }

    /**
     * 減算
     */
    TransactionCounter& operator--() {
        return decrement();
    }
    /**
     * 減算
     */
    TransactionCounter& operator--(int) {
        return decrement();
    }

    /**
     * 現在値を取得する
     */
    float getValue() const {
        return counter.getValue(leapType);
    }

    /**
     * 現在値を設定する
     */
    float getValue(const LeapType_e type) const {
        return counter.getValue(type);
    }

    /**
     * 現在値を直接設定する
     */
    void setValueDirect(const float value) {
        counter.setValue(value);
    }

    /**
     * カウンタが停止していたらtrue
     */
    jcboolean isStopped() {
        return counter.isStopped();
    }

    /**
     * 内部カウンターを取得する
     */
    CounterF& getCounter() {
        return counter;
    }

    /**
     * 内部カウンターを取得する
     */
    const CounterF& getCounter() const {
        return counter;
    }
};

}
}

#endif /* TRANSACTIONCOUNTER_H_ */
