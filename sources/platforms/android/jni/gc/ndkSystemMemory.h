/*
 * ndkGC.h
 *
 *  Created on: 2012/10/05
 */

#ifndef NDKSYSTEMMEMORY_H_
#define NDKSYSTEMMEMORY_H_

#include    "ndkObjMonitoring.h"

namespace ndk {

/**
 * Javaはファイナライザが保証されないため、独自の監視システムを作る必要がある。
 */
class SystemMemory {
public:
    /**
     * 監視対象から削除する。
     */
    static void addMonitor(jobject objMonitoring, jc_sp<Object> pointer );

    /**
     * 監視対象から外れているアイテムを削除する。
     */
    static void gc();
};

}

#endif /* NDKOBJMONITOR_H_ */
