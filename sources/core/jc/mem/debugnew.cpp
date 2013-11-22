/*
 * debugnew.cpp
 *
 *  Created on: 2013/11/20
 */
#include    "jointcoding.h"
#include    "jc/math/Math.h"
#include    "pthread.h"

#include    "jc/mem/AllocChain.h"

using namespace jc;

namespace {

struct AllocHeader {
    /**
     * 関連付けされているノード
     */
    AllocChainNode *node;

    /**
     * 取得したいサイズ
     */
    u32 size;

    /**
     * タグ付けされているファイル
     */
    char* file;

    /**
     * タグ付けされている行
     */
    int line;
};

static jc::debug::AllocatedHeapInfo gHeapInfo = { 0 };

static pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * 配列確保の際にシステムが要求するサイズ
 */
static const u32 ARRAY_SYSTEM_BYTES = 4;

/**
 * allocする際のバイト境界を設定する
 */
static const u32 BYTE_ALIGN = 32;

/**
 * alloc済みのメモリをキャッシュするノード数
 */
static const u32 ALLOC_CACHE_NODE_NUM = 256;

/**
 * alloc済みのヒープチェイン
 */
static AllocChainNode* allocatedChains[ALLOC_CACHE_NODE_NUM] { NULL };

/**
 * 使用済みのヒープチェイン
 */
static AllocChainNode* usingChains[ALLOC_CACHE_NODE_NUM] { NULL };

/**
 * ヒープを新規に確保する
 */
static AllocChainNode* heapAlloc(size_t size) {
    // allocサイズを整える
    size = ((size / BYTE_ALIGN) + 1) * BYTE_ALIGN;

    // ノードチェイン番号を生成する
    const u32 chainIndex = jc::min(size / BYTE_ALIGN, ALLOC_CACHE_NODE_NUM - 1);

    // 返却可能なサイズのノードを探す
    AllocChainNode* pResult = AllocChain_findNodeLarge(allocatedChains[chainIndex], size);

    if (!pResult) {
        // ノードが見つからなければ、新規にノードを作成する
        pResult = AllocChain_newNode(size);
        // 確保済みノードをインクリメント
        ++gHeapInfo.nodes_allocated;
        jclogf("hit allocated cache :: %x", pResult);
    } else {
        // ノードが見つかったら、見つかったノードを削除する
        AllocChain_remove(&allocatedChains[chainIndex], pResult);

        // キャッシュをデクリメント
        --gHeapInfo.nodes_cache;
    }

    // 使用済みノードに追加する
    usingChains[chainIndex] = AllocChain_pushFront(usingChains[chainIndex], pResult);

    // 使用中ノードをインクリメント
    ++gHeapInfo.nodes_using;

    // 確保したノードのヒープを返す
    return pResult;
}

/**
 * ノードを利用可能状態にする
 */
static void heapFree(AllocChainNode *node) {
    // ノードチェイン番号を生成する
    const u32 chainIndex = jc::min(node->heapSize / BYTE_ALIGN, ALLOC_CACHE_NODE_NUM - 1);

    // 使用済みノードから切り離す
    AllocChain_remove(&usingChains[chainIndex], node);

    // 使用可能ノードへ接続する
    allocatedChains[chainIndex] = AllocChain_pushFront(allocatedChains[chainIndex], node);

    {
        // キャッシュをインクリメント
        ++gHeapInfo.nodes_cache;
        // 使用中をデクリメント
        --gHeapInfo.nodes_using;
    }
}

/**
 * 排他制御を行った上でメモリを確保する
 */
void* alloc_mem(size_t size, const u32 systemSize, const char * const file, const int line) {

    pthread_mutex_lock(&alloc_mutex);

    void* result = NULL;

    {
        AllocChainNode *pNode = heapAlloc(size + sizeof(AllocHeader));
        AllocHeader *pHead = (AllocHeader*) AllocChain_getUserMemory(pNode);

        pHead->file = (char*) file;
        pHead->line = line;
        pHead->size = (size - systemSize);
        pHead->node = pNode;

        // 確保済みbytesをインクリメント
        {
            gHeapInfo.heap_bytes += pHead->size;
            ++gHeapInfo.objects;

            if (file) {
                ++gHeapInfo.objects_marked;
            } else {
                ++gHeapInfo.objects_nomarked;
            }
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
            gHeapInfo.heap_bytes -= pHead->size;
            --gHeapInfo.objects;

            if (pHead->file) {
                --gHeapInfo.objects_marked;
            } else {
                --gHeapInfo.objects_nomarked;
            }
        }

        heapFree(pHead->node);
    }

    pthread_mutex_unlock(&alloc_mutex);
}

}

/**
 * メモリを単体確保する
 */
void* operator new(size_t size) throw (std::bad_alloc) {
    return alloc_mem(size, 0, NULL, 0);
}

/**
 * メモリを配列確保する
 */
void* operator new[](size_t size) throw (std::bad_alloc) {
    return alloc_mem(size, ARRAY_SYSTEM_BYTES, NULL, 0);
}

void* operator new(size_t size, const char * const file, int line) throw (std::bad_alloc) {
    return alloc_mem(size, 0, __getFileName(file), line);
}

void* operator new[](size_t size, const char * const file, int line) throw (std::bad_alloc) {
    return alloc_mem(size, ARRAY_SYSTEM_BYTES, __getFileName(file), line);
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
 * メモリの確保状態を取得する
 */
void getAllocatedInfo(AllocatedHeapInfo *result) {
    assert(result);

    pthread_mutex_lock(&alloc_mutex);
    {
        (*result) = gHeapInfo;
    }
    pthread_mutex_unlock(&alloc_mutex);
}

}
}
