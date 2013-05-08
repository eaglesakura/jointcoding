/*
 * PullSelectionBox.hpp
 *
 *  Created on: 2013/05/08
 */

#ifndef PULLSELECTIONBOX_HPP_
#define PULLSELECTIONBOX_HPP_

#include    "jointcoding.h"

namespace jc {

/**
 * 抽選箱を作成する
 *
 * 一定の範囲の整数をランダムに取り出す.
 * 同じ整数は１度しか出てこない.
 * すべての整数を出し終わったら、再度抽選を開始する
 */
class PullSelectionBox: public Object {
    s32 rangeStart;
    s32 rangeEnd;

    /**
     * 乱数を並べた箱
     */
    std::list<s32> randomBox;

    /**
     * 指定範囲内ランダムな数値を作成する
     */
    s32 makeRandomValue() {
        const s32 range = (rangeEnd - rangeStart) + 1;
        return (rand() % range) + rangeStart;
    }

public:

    /**
     * ランダム箱を作り直す
     */
    virtual void reset() {
        // 既存のボックスを削除する
        randomBox.clear();

        const s32 value_num = (rangeEnd - rangeStart + 1);

        s32 checks = 0;
        std::map<s32, s32> temp;
        // すべての数が揃うまで繰り返す
        while (temp.size() < value_num) {
            const s32 random_value = makeRandomValue();
            // まだ乱数が登録されて無ければ、登録を行う
            if (temp.find(random_value) == temp.end()) {
                temp.insert(std::map<s32, s32>::value_type(random_value, random_value));
                randomBox.push_back(random_value);
            }

            // 無限ループチェック
            assert(++checks < 1000000);
        }
    }

    /**
     * 特定レンジの整数を作成する
     */
    PullSelectionBox(const s32 rangeStart, const s32 rangeEnd) {
        this->rangeStart = jc::min(rangeStart, rangeEnd);
        this->rangeEnd = jc::max(rangeStart, rangeEnd);

        this->reset();
    }

    virtual ~PullSelectionBox() {
    }

    /**
     * 抽選箱が空になったらtrueを返す
     */
    virtual jcboolean empty() const {
        return randomBox.empty();
    }

    /**
     * 乱数を取り出す
     * 箱が空の場合はfalseを返し、何もしない
     */
    virtual jcboolean nextRandomValue(s32 *result) {
        assert(result);

        if (randomBox.empty()) {
            // 既にボックスは空になっている
            return jcfalse;
        } else {
            // 最初から取り出す
            *result = randomBox.front();
            randomBox.pop_front();
            return jctrue;
        }
    }

    /**
     * 乱数を返す
     * 箱が空の場合は再度抽選箱を用意する
     */
    virtual s32 nextRandomValue() {
        if (randomBox.empty()) {
            reset();
        }

        s32 result = 0;
        nextRandomValue(&result);
        return result;
    }

};

/**
 * managed
 */
typedef jc_sp<PullSelectionBox> MPullSelectionBox;

}

#endif /* PULLSELECTIONBOX_HPP_ */
