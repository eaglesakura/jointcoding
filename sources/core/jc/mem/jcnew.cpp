/*
 * debugnew.cpp
 *
 *  Created on: 2013/11/20
 */
#include    "jointcoding.h"
#include    "jc/math/Math.h"
#include    "jc/mem/AllocChain.hpp"
#include    "jc/mem/jcnew.h"

#include    <pthread.h>
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
 * alloc済みのメモリをキャッシュするノード数
 */
static const u32 ALLOC_CACHE_NODE_NUM = JC_MEMNEW_CACHE_NODESIZE / JC_MEMNEW_BYTE_ALIGNMENT;
/**
 * キャシュノードの最終インデックス
 */
static const u32 ALLOC_CACHE_NODE_LASTINDEX = ALLOC_CACHE_NODE_NUM - 1;

/**
 * alloc済みのヒープチェイン
 */
static AllocChainNode* allocatedChains[ALLOC_CACHE_NODE_NUM] = { NULL };

/**
 * 使用済みのヒープチェイン
 */
static AllocChainNode* usingChains[ALLOC_CACHE_NODE_NUM] = { NULL };

/**
 * 大容量ヒープを要求している場合はtrueを返す
 */
inline bool isLargeHeap(size_t size) {
    return size > JC_MEMNEW_CACHE_NODESIZE;
}

/**
 * ヒープを新規に確保する
 */
static AllocChainNode* heapAlloc(size_t size) {
    // allocサイズを整える
    size = (((size - 1) / JC_MEMNEW_BYTE_ALIGNMENT) + 1) * JC_MEMNEW_BYTE_ALIGNMENT;

    // ノードチェイン番号を生成する
    const u32 selectChainIndex = jc::min(size / JC_MEMNEW_BYTE_ALIGNMENT, ALLOC_CACHE_NODE_LASTINDEX);
    u32 chainIndex = selectChainIndex;

    // 返却可能なサイズのノードを探す
    AllocChainNode* pResult = AllocChain_findNodeLarge(allocatedChains[chainIndex], size);

    if (!pResult) {
        // 初期Indexに戻す
        chainIndex = selectChainIndex;

        // ノードが見つからなければ、新規にノードを作成する
        pResult = AllocChain_newNode(size);
//        jclogf("alloc newnode %d bytes :: %x", pResult->heapSize, pResult);
    } else {
        // ノードが見つかったら、見つかったノードを削除する
        AllocChain_remove(&allocatedChains[chainIndex], pResult);

        // キャッシュをデクリメント
        --gHeapInfo.nodes_cache;

//        jclogf("hit allocated cache %d bytes :: %x", pResult->heapSize, pResult);
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
    const u32 chainIndex = jc::min(node->heapSize / JC_MEMNEW_BYTE_ALIGNMENT, ALLOC_CACHE_NODE_NUM - 1);

    // 使用済みノードから切り離す
//    jclogf("node info(%x) prev(%x) next(%x)", node, node->prev, node->next);
    {
        AllocChain_remove(&usingChains[chainIndex], node);
        // 使用中をデクリメント
        --gHeapInfo.nodes_using;
    }

    // 使用可能ノードへ接続する
    if (isLargeHeap(node->heapSize)) {
        // 大サイズノードは直接解放する
        ::free(node);
    } else {
        allocatedChains[chainIndex] = AllocChain_pushFront(allocatedChains[chainIndex], node);
        // キャッシュをインクリメント
        ++gHeapInfo.nodes_cache;
    }
}

}

namespace jc {

/**
 * 排他制御を行った上でメモリを確保する
 */
void* heap_alloc(size_t size, const u32 systemSize, const char * const file, const int line) {
    // debug info
#ifdef DEBUG
    if (size > 1024) {
        // 1kb以上の大ヒープを要求したら犯人を出力する
        jclogf("request large heap(%d bytes) from(%s) L %d", size, file ? file : "Unknown Source", line);
    }
#endif

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
void heap_free(void* p, const u32 systemSize) {
    if (!p) {
        return;
    }

    pthread_mutex_lock(&alloc_mutex);
    {
        AllocHeader* pHead = (((AllocHeader*) (((u8*) p) - 0)) - 1);

        // 確保済みbytesをデクリメント
        {
            gHeapInfo.heap_bytes -= pHead->size;
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

/**
 * メモリキャッシュをクリーンアップする
 */
void heap_cleanup() {
    jclog("start heap cache clean");

    pthread_mutex_lock(&alloc_mutex);

    for (int i = 0; i < ALLOC_CACHE_NODE_NUM; ++i) {
        while (allocatedChains[i]) {
            AllocChainNode *pFront = allocatedChains[i];
            allocatedChains[i] = AllocChain_popFront(allocatedChains[i]);

            free(pFront);
        }

    }

    gHeapInfo.nodes_cache = 0;
    pthread_mutex_unlock(&alloc_mutex);

    jclog("finish heap cache clean");
}

}

/**
 * メモリを単体確保する
 */
void* operator new(size_t size) throw (std::bad_alloc) {
    return heap_alloc(size, 0, NULL, 0);
}

/**
 * メモリを配列確保する
 */
void* operator new[](size_t size) throw (std::bad_alloc) {
    return heap_alloc(size, ARRAY_SYSTEM_BYTES, NULL, 0);
}

void* operator new(size_t size, const char * const file, int line) throw (std::bad_alloc) {
    return heap_alloc(size, 0, __getFileName(file), line);
}

void* operator new[](size_t size, const char * const file, int line) throw (std::bad_alloc) {
    return heap_alloc(size, ARRAY_SYSTEM_BYTES, __getFileName(file), line);
}

/**
 * メモリを単体解放する
 */
void operator delete(void* p) throw () {
    heap_free(p, 0);
}

/**
 * メモリを配列解放する
 */
void operator delete[](void* p) throw () {
    heap_free(p, ARRAY_SYSTEM_BYTES);
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
