/*
 * UniqueCache.hpp
 *
 *  Created on: 2014/01/27
 */
#ifndef UNIQUECACHE_HPP_
#define UNIQUECACHE_HPP_

#include    "jointcoding.h"
#include    <map>

namespace jc {

enum CacheState_e {
    /**
     * キャッシュが存在しない
     */
    CacheState_None,

    /**
     * キャッシュが存在する
     */
    CacheState_Exist,

    /**
     * キャッシュを削除済み
     */
    CacheState_Deleted,
};

class UniqueCache {

    std::map<u32, CacheState_e> cacheMap;

    jcmutex mutex;
public:
    UniqueCache() {
    }

    ~UniqueCache() {
    }

    /**
     * キャッシュを挿入する
     */
    void insert(const u32 key) {
        MutexLock lock(mutex);

        std::map<u32, CacheState_e>::iterator itr = cacheMap.find(key);

        if (itr == cacheMap.end()) {
            // キャッシュがまだ存在しない
            cacheMap.insert(std::pair<u32, CacheState_e>(key, CacheState_Exist));
        } else {
            itr->second = CacheState_Exist;
        }
    }

    void remove(const u32 key, const jcboolean clean = jctrue) {
        MutexLock lock(mutex);

        std::map<u32, CacheState_e>::iterator itr = cacheMap.find(key);

        if (itr != cacheMap.end()) {
            if (clean) {
                // キャッシュを廃棄する
                cacheMap.erase(itr);
            } else {
                // キャッシュ状態を上書きする
                itr->second = CacheState_Deleted;
            }
        }
    }

    CacheState_e getStatus(const u32 key) const {
        MutexLock lock(mutex);

        std::map<u32, CacheState_e>::const_iterator itr = cacheMap.find(key);

        if (itr == cacheMap.end()) {
            return CacheState_None;
        } else {
            return itr->second;
        }
    }

    /**
     * キャッシュが存在している場合はtrueを返す
     */
    jcboolean exist(const u32 key) const {
        return getStatus(key) == CacheState_Exist;
    }
};

}

#endif /* UNIQUECACHE_HPP_ */
