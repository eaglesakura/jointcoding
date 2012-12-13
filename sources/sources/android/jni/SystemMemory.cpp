/*
 * ndkObjMonitor.cpp
 *
 *  Created on: 2012/10/05
 */
#include    "jcandroid/jni/SystemMemory.h"
#include    <list>

namespace ndk {

class _gc_data {
public:
    /**
     * 監視モニター
     */
    jc_sp<ObjMonitoring> monitor;

    /**
     * ターゲット
     */
    jc_sp<Object> target;

    _gc_data( jc_sp<ObjMonitoring> monitor, jc_sp<Object> target) {
        this->monitor = monitor;
        this->target = target;
    }
};

typedef jc_sp<_gc_data> gc_data;

namespace {
/**
 * 操作用ミューテックス
 */
static jcmutex g_mutex;

/**
 * 監視対象一覧
 */
static std::list<gc_data> targets;
}

/**
 * 監視対象から削除する。
 */
void SystemMemory::addMonitor(jobject objMonitoring, jc_sp<Object> pointer ) {
    MutexLock lock(g_mutex);

    jc_sp<ObjMonitoring> monitor = ObjMonitoring::global(objMonitoring);

    targets.push_back(gc_data(new _gc_data(monitor, pointer)));

}

/**
 * 監視対象から外れているアイテムを削除する。
 */
void SystemMemory::gc() {
    MutexLock lock(g_mutex);

    std::list<gc_data>::iterator itr = targets.begin(), end = targets.end();

    while (itr != end) {

        if ((*itr)->monitor->exist()) {
            // まだモニターが生きている
            ++itr;
        } else {
            // 監視対象が死んでいるなら、イテレータ対象を殺す。
            itr = targets.erase(itr);
            end = targets.end();
        }
    }
}

}

