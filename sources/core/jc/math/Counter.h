/*
 * Counter.h
 *
 *  Created on: 2013/03/06
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include    "jointcoding.h"
#include    "jc/math/Math.h"
#include    "jc/math/Angle.h"

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

    /**
     * 最大・最小のどちらかで停止する
     */
    CounterType_EdgeStop,
};

enum LeapType_e {
    /**
     * 直値を取得する
     */
    LeapType_Direct,

    /**
     * イーズ式（１）
     * ゆるやかに加速、ゆるやかに減速
     */
    LeapType_Ease1,

    /**
     * イーズ式（２）
     * ゆるやかに加速、ゆるやかに減速
     */
    LeapType_Ease2,

    /**
     * 指数式
     * 急加速、急減速
     */
    LeapType_Exp1,

    /**
     * 指数式
     * 急加速、急減速
     */
    LeapType_Exp2,
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
        stopped = jcfalse;
        if (inc) {
            // インクリメント処理
            value += offset;
            if (value > maxValue) {
                // 最大値を超えたら
                value = maxValue;

                if (type == CounterType_MaxStop || type == CounterType_EdgeStop) {
                    // 最大停止モードだったら止める
                    stopped = jctrue;
                } else {
                    inc = !inc;
                    ++revertCount;
                }
            }
        } else {
            // デクリメント処理
            value -= offset;
            if (value < minValue) {
                // 最低値を下回ったら
                value = minValue;
                if (type == CounterType_MinStop || type == CounterType_EdgeStop) {
                    // 最低停止モードだったら止める
                    stopped = jctrue;
                } else {
                    inc = !inc;
                    ++revertCount;
                }
            }
        }

        return *this;
    }

    /**
     * カウンターを戻す
     */
    CounterT<T>& decrement() {
        stopped = jcfalse;

        if (!inc) {
            // インクリメント処理
            value += offset;
            if (value > maxValue) {
                // 最大値を超えたら
                value = maxValue;
                if (type == CounterType_MaxStop || type == CounterType_EdgeStop) {
                    // 最大停止モードだったら止める
                    stopped = jctrue;
                } else {
                    inc = !inc;
                    ++revertCount;
                }
            }
        } else {
            // デクリメント処理
            value -= offset;
            if (value < minValue) {
                // 最低値を下回ったら
                value = minValue;
                if (type == CounterType_MinStop || type == CounterType_EdgeStop) {
                    // 最低停止モードだったら止める
                    stopped = jctrue;
                } else {
                    inc = !inc;
                    ++revertCount;
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
     * 範囲の長さを取得する
     */
    T getRangeLength() const {
        return maxValue - minValue;
    }

    /**
     * オフセットを設定する
     */
    void setOffset(const T offset) {
        assert(offset <= getRangeLength());

        this->offset = offset;
    }

    /**
     * 指定した秒数で1.0になるようなオフセット値を決定する
     */
    void calcOffset(const s32 frameRate, const double sec) {
        setOffset(1.0 / sec / (double) frameRate);
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
     * 最小値であればtrue
     */
    const jcboolean isMinValue() const {
        return minValue == value;
    }

    /**
     * 最大値であればtrue
     */
    const jcboolean isMaxValue() const {
        return maxValue == value;
    }

    /**
     * 適当な式で修正を行った値を取得する
     *
     * @seel http://game.g.hatena.ne.jp/Nao_u/20110505
     */
    const T getValue(const LeapType_e type) const {
        switch (type) {
            case LeapType_Direct:
                return value;
            case LeapType_Ease1:
                return (T) (0.5 - cos((double) value * jc::PI) * 0.5);
            case LeapType_Ease2:
                return (T) (3.0 * (double) value * (double) value - 2 * (double) value * (double) value);
            case LeapType_Exp1:
                return (T) (1.0 - exp(-3.0 * (double) value));
            case LeapType_Exp2: {
                const double temp = (1.0 - (double) value);
                return (T) (1.0 - temp * temp * temp);
            }
            default:
                assert(false);
                break;
        }
        return (T) 0;
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

    /**
     * インクリメントモードの場合true
     */
    jcboolean isIncrement() const {
        return inc;
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
