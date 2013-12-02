/*
 * jcnew.h
 *
 *  Created on: 2013/11/27
 */
#ifndef JCNEW_H_
#define JCNEW_H_

#include    <stddef.h>
#include    <new>

extern void* operator new(size_t size) throw (std::bad_alloc);
extern void* operator new[](size_t size) throw (std::bad_alloc);

extern void operator delete(void* p) throw ();
extern void operator delete[](void* p) throw ();

/**
 * newを行った際に丸められるバイト境界
 */
#ifndef JC_MEMNEW_BYTE_ALIGNMENT
#define JC_MEMNEW_BYTE_ALIGNMENT   16
#endif

/**
 * newしたメモリをキャッシュする最大サイズ
 */
#ifndef JC_MEMNEW_CACHE_NODESIZE
#define JC_MEMNEW_CACHE_NODESIZE   64
#endif

/**
 * デバッグ用にnewを書き換える
 */
#ifdef  DEBUG
// for debug
extern void* operator new(size_t size, const char * const file, int line) throw (std::bad_alloc);
extern void* operator new[](size_t size, const char * const file, int line) throw (std::bad_alloc);

#define mark_new new(__FILE__, __LINE__)
#else

// 通常newを利用する
#define mark_new new
#endif

namespace jc {

namespace debug {

/**
 * ヒープの現在の情報を取得する
 *
 */
struct AllocatedHeapInfo {
    /**
     * 現在生きているヒープ容量
     */
    u32 heap_bytes;

    /**
     * マークが付いているオブジェクト数
     */
    u32 objects_marked;

    /**
     * マークが付いていないオブジェクト数
     */
    u32 objects_nomarked;

    /**
     * 使用中のノード数
     */
    u32 nodes_using;

    /**
     * キャッシュ中のノード
     */
    u32 nodes_cache;
};

/**
 * メモリの確保状態を取得する
 */
extern void getAllocatedInfo(AllocatedHeapInfo *result);

}

}

#endif /* JCNEW_H_ */
