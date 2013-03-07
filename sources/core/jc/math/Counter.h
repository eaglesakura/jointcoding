/*
 * Counter.h
 *
 *  Created on: 2013/03/06
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include    "jointcoding.h"
#include    "jc/math/Math.h"

namespace jc {

enum CounterType_e {
    /**
     *
     */
    CounterType_Loop,

    /**
     * 最大値へ停止する
     */
    CounterType_MaxStop,

    /**
     * 最小値まで戻ったら停止する
     */
    CounterType_MinStop,
};

template<typename T>
class CounterT {
    /**
     * 現在値
     */
    T value;

    /**
     * オフセット値
     */
    T offset;

    /**
     * 最小値
     */
    T minValue;

    /**
     * 最大値
     */
    T maxValue;

    /**
     * 何度反転しているか
     */
    s32 revertCount;

    /**
     * カウンターの種類
     */
    CounterType_e type;

    /**
     * インクリメント状態の場合true
     */
    jcboolean inc;

    /**
     * 更新停止したらtrue
     */
    jcboolean stopped;

public:
    CounterT(const T start = 0, const T offset = 1, const T minValue = 0, const T maxValue = 1, const CounterType_e type = CounterType_Loop) {
        this->revertCount = 0;
        this->inc = jctrue;
        this->stopped = jcfalse;
        this->type = type;
        this->offset = jc::abs(offset);
        this->maxValue = jc::max(maxValue, minValue);
        this->minValue = jc::min(minValue, maxValue);
        this->value = jc::minmax(this->minValue, this->maxValue, start);
    }

    ~CounterT() {
    }

    /**
     * 初期状態に戻す
     */
    void reset() {
        this->value = minValue;
        this->revertCount = 0;
        this->stopped = jcfalse;
        this->inc = jctrue;
    }

    /**
     * カウンターを次へ進める
     */
    CounterT<T>& increment() {
        if (stopped) {
            return *this;
        }

        if (inc) {
            // インクリメント処理
            value += offset;
            if (value > maxValue) {
                // 最大値を超えたら
                value = maxValue;
                if (type != CounterType_MaxStop) {
                    inc = !inc;
                    ++revertCount;
                } else {
                    // 最大停止モードだったら止める
                    stopped = jctrue;
                }
            }
        } else {
            // デクリメント処理
            value -= offset;
            if (value < minValue) {
                // 最低値を下回ったら
                value = minValue;
                if (type != CounterType_MinStop) {
                    inc = !inc;
                    ++revertCount;
                } else {
                    // 最低停止モードだったら止める
                    stopped = jctrue;
                }
            }
        }

        return *this;
    }

    /**
     * カウンターを戻す
     */
    CounterT<T>& decrement() {
        if (stopped) {
            return *this;
        }

        if (!inc) {
            // インクリメント処理
            value += offset;
            if (value > maxValue) {
                // 最大値を超えたら
                value = maxValue;
                if (type != CounterType_MaxStop) {
                    inc = !inc;
                    ++revert;
                } else {
                    // 最大停止モードだったら止める
                    stopped = jctrue;
                }
            }
        } else {
            // デクリメント処理
            value -= offset;
            if (value < minValue) {
                // 最低値を下回ったら
                value = minValue;
                if (type != CounterType_MinStop) {
                    inc = !inc;
                    ++revert;
                } else {
                    // 最低停止モードだったら止める
                    stopped = jctrue;
                }
            }
        }

        return *this;
    }

    /**
     * 進行方向を反転する
     */
    void revert() {
        inc = !inc;
    }

    /**
     * 制御タイプを設定する
     */
    void setType(const CounterType_e type) {
        this->type = type;
    }

    /**
     * カウンター範囲を設定する
     */
    void setRange(const T minValue, const T maxValue) {
        this->maxValue = jc::max(maxValue, minValue);
        this->minValue = jc::min(minValue, maxValue);
        setValue(value);
    }

    /**
     * オフセットを設定する
     */
    void setOffset(const T offset) {
        assert(offset <= (maxValue - minValue));

        this->offset = offset;
    }

    /**
     * 現在地を設定する
     */
    void setValue(const T value) {
        this->value = jc::minmax(minValue, maxValue, value);
    }

    /**
     * インクリメント処理
     */
    CounterT<T>& operator++(int) {
        return increment();
    }

    /**
     * インクリメント処理
     */
    CounterT<T>& operator++() {
        return increment();
    }

    /**
     * デクリメント処理
     */
    CounterT<T>& operator--(int) {
        return decrement();
    }

    /**
     * デクリメント処理
     */
    CounterT<T>& operator--() {
        return decrement();
    }

    /**
     * 現在値を取得する
     */
    const T getValue() const {
        return value;
    }

    /**
     * 停止状態ならtrue
     */
    const jcboolean isStopped() const {
        return stopped;
    }

    /**
     * 反転した回数を取得する
     */
    const u32 getRevertCount() const {
        return revertCount;
    }

    /**
     * 現在値を取得する
     */
    operator T() const {
        return value;
    }
};

/**
 * 整数カウンタ
 */
typedef CounterT<s32> CounterI;

/**
 * 浮動少数カウンタ
 */
typedef CounterT<float> CounterF;

}

#endif /* COUNTER_H_ */
