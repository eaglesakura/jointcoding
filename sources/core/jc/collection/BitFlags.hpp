/*
 * BitFlags.hpp
 *
 *  Created on: 2013/04/24
 */

#ifndef BITFLAGS_HPP_
#define BITFLAGS_HPP_

#include    "jc/system/Macro.h"

namespace jc {

template<u32 MAX_FLAG_NUM = 32>
class BitFlags {
    /**
     * フラグ
     */
    u32 flags[(MAX_FLAG_NUM + 31) / 32];
public:
    BitFlags() {
        reset();
    }

    ~BitFlags() {
    }

    void reset() {
        for (int i = 0; i < (sizeof(flags) / sizeof(u32)); ++i) {
            flags[i] = 0x00000000;
        }
    }

    /**
     * すべてのフラグを有効にする
     */
    void allEnable() {
        for (int i = 0; i < (sizeof(flags) / sizeof(u32)); ++i) {
            flags[i] = 0xFFFFFFFF;
        }
    }

    /**
     * フラグを有効にする
     */
    void enable(const u32 index) {
        assert((index / 32) < ((sizeof(flags) / sizeof(u32))));

        flags[index / 32] |= (0x1 << (index % 32));
    }

    /**
     * フラグを無効化する
     */
    void disable(const u32 index) {
        assert((index / 32) < ((sizeof(flags) / sizeof(u32))));

        flags[index / 32] &= ~(0x1 << (index % 32));
    }

    /**
     * フラグを立てる
     */
    void set(const u32 index, const jcboolean set) {
        if (set) {
            enable(index);
        } else {
            disable(index);
        }
    }

    /**
     * 特定インデックスがtrueであることを確認する
     */
    jcboolean isEnable(const u32 index) const {
        assert((index / 32) < ((sizeof(flags) / sizeof(u32))));

        return (flags[index / 32] & (0x1 << (index % 32))) != 0;
    }

    /**
     * 特定インデックスがfalseであることを確認する
     */
    jcboolean isDisable(const u32 index) const {
        return !isEnable(index);
    }

    /**
     * ビット演算でまとめたフラグがONになっていることを確認する
     * flags[0]に対してのみ対応可能なため、注意すること
     */
    jcboolean isAllEnableFlags(const u32 bits) const {
        return has_flag_all(flags[0], bits);
    }

    /**
     * 全チェック
     */
    jcboolean isAllEnable(const u32 indices_num, const u32* indices) const {
        for (int i = 0; i < indices_num; ++i) {
            if (isDisable(indices[i])) {
                // 一つでもdisableがあったら廃棄
                return jcfalse;
            }
        }
        // 全てenable
        return jctrue;
    }

    /**
     * 全チェック
     */
    jcboolean isAllDisable(const u32 indices_num, const u32* indices) const {
        for (int i = 0; i < indices_num; ++i) {
            if (isEnable(indices[i])) {
                // 一つでもenableがあったら廃棄
                return jcfalse;
            }
        }
        // 全てenable
        return jctrue;
    }
};

typedef BitFlags<32> SimpleBitFlags;

}

#endif /* BITFLAGS_HPP_ */
