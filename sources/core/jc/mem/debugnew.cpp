/*
 * debugnew.cpp
 *
 *  Created on: 2013/11/20
 */
#include    "jointcoding.h"
#include    "pthread.h"

using namespace jc;

namespace {

struct AllocHeader {
    /**
     * 取得したいサイズ
     */
    u32 size;

    char* file;

    int line;
};

static u32 gAllocatedBytes = 0;
static u32 gAllocatedObjects = 0;

static pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * 配列確保の際にシステムが要求するサイズ
 */
static const u32 ARRAY_SYSTEM_BYTES = 4;

/**
 * 排他制御を行った上でメモリを確保する
 */
void* alloc_mem(const size_t size, const u32 systemSize) {
    pthread_mutex_lock(&alloc_mutex);

    void* result = NULL;

    {
        AllocHeader *pHead = (AllocHeader*) malloc(size + sizeof(AllocHeader));

        pHead->file = NULL;
        pHead->line = 0;
        pHead->size = (size - systemSize);

        // 確保済みbytesをインクリメント
        {
            gAllocatedBytes += pHead->size;
            ++gAllocatedObjects;
        }

        // 返却値はヘッダを無視する
        result = (void*) (pHead + 1);
    }

    pthread_mutex_unlock(&alloc_mutex);

    return result;
}

/**
 * 排他制御を行った上でメモリを解放する
 */
void free_mem(void* p, const u32 systemSize) {
    pthread_mutex_lock(&alloc_mutex);
    {
        AllocHeader* pHead = (((AllocHeader*) (((u8*) p) - 0)) - 1);

        // 確保済みbytesをデクリメント
        {
            gAllocatedBytes -= pHead->size;
            --gAllocatedObjects;
        }
        free(pHead);
    }

    pthread_mutex_unlock(&alloc_mutex);
}

}

/**
 * メモリを単体確保する
 */
void* operator new(size_t size) throw (std::bad_alloc) {
    return alloc_mem(size, 0);
}

/**
 * メモリを配列確保する
 */
void* operator new[](size_t size) throw (std::bad_alloc) {
    return alloc_mem(size, ARRAY_SYSTEM_BYTES);
}

/**
 * メモリを単体解放する
 */
void operator delete(void* p) throw () {
    free_mem(p, 0);
}

/**
 * メモリを配列解放する
 */
void operator delete[](void* p) throw () {
    free_mem(p, ARRAY_SYSTEM_BYTES);
}

namespace jc {

namespace debug {

/**
 * ヒープに確保したメモリをbyte単位で取得する
 */
u32 getAllocatedHeapBytes() {
    return gAllocatedBytes;
}

/**
 * ヒープに確保したメモリを個数単位で取得する
 */
u32 getAllocatedHeapObjects() {
    return gAllocatedObjects;
}

}

}
