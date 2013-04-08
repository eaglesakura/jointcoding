/*
 * RAM.h
 *
 *  Created on: 2013/04/08
 */

#ifndef RAM_H_
#define RAM_H_

#include    "jointcoding.h"

namespace ndk {

/**
 * デバッグ用のRAM情報
 */
class RAM {
public:
    /**
     * RAMのプロセス合計量(bytes)
     */
    static jc::u64 totalMemoryBytes();

    /**
     * RAMのプロセス使用量(bytes)
     * adb shell top | grep "package_name"に相当する値を取得する
     */
    static jc::u64 useRssMemoryBytes();
};

}

#endif /* RAM_H_ */
