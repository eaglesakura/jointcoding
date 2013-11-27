/*
 * AllocChain.h
 *
 *  Created on: 2013/11/22
 */

#include    "jointcoding.h"

namespace jc {

/**
 * 確保済みのメモリ
 */
struct AllocChainNode {
    /**
     * 次のチェイン
     */
    AllocChainNode *next;
    /**
     * 前のチェイン
     */
    AllocChainNode *prev;

    /**
     * 管理しているヒープ量
     */
    u32 heapSize;
};

/**
 * ノードからユーザー領域を取得する
 */
inline void* AllocChain_getUserMemory(AllocChainNode *node) {
    return (void*) (node + 1);
}

/**
 * 新規にノードを作成する
 */
inline AllocChainNode* AllocChain_newNode(const u32 heapSize) {
    AllocChainNode* node = (AllocChainNode*) malloc(sizeof(AllocChainNode) + heapSize);
    node->heapSize = heapSize;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

/**
 * 指定したノードを現在のノードリストから取り除く
 *
 * @result ノードが残っていればjctrue
 */
inline jcboolean AllocChain_remove(AllocChainNode **headNode, AllocChainNode *node) {
    AllocChainNode *prev = node->prev;
    AllocChainNode *next = node->next;

    node->prev = NULL;
    node->next = NULL;

    // 前と後ろを繋ぎ直す
    if (prev) {
        prev->next = next;
    }
    if (next) {
        next->prev = prev;
    }

    // 頭を切り落とそうとしていたら、頭を書き換える
    if ((*headNode) == node) {
        (*headNode) = next;
    }

    return prev != NULL || next != NULL;
}

/**
 * ノードの最前列にリストをつなげる
 */
inline AllocChainNode* AllocChain_pushFront(AllocChainNode *list, AllocChainNode *newNode) {
    newNode->next = list;
    if (list) {
        list->prev = newNode;
    }
    return newNode;
}

/**
 * 先頭ノードを取り出す
 * @result 新たな先頭ノード
 */
inline AllocChainNode* AllocChain_popFront(AllocChainNode *list) {
    AllocChainNode *result = list->next;
    result->prev = NULL;
    list->next = NULL;

    assert(list->prev == NULL);
    return result;
}

/**
 * 指定サイズよりも大きなノードを探す
 */
inline AllocChainNode* AllocChain_findNodeLarge(AllocChainNode *list, const u32 size) {
    while (list) {
        if (list->heapSize >= size && list->heapSize < (size << 1)) {
            return list;
        }

        list = list->next;
    }

    return NULL;
}

}
