/*
 * jcnew.h
 *
 *  Created on: 2013/11/14
 */
#ifndef JCNEW_H_
#define JCNEW_H_

#include    "jointcoding.h"
#include    <stddef.h>
#include    <new>

extern void* operator new(size_t size) throw (std::bad_alloc);
extern void* operator new[](size_t size) throw (std::bad_alloc);

extern void operator delete(void* p) throw ();
extern void operator delete[](void* p) throw ();

// for debug
extern void* operator new(size_t size, char * const file, int line) throw (std::bad_alloc);
extern void* operator new[](size_t size, char * const file, int line) throw (std::bad_alloc);

#define mark_new new(__FILE__, __LINE__)

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
     * 生きているヒープオブジェクト数
     */
    u32 objects;

    /**
     * マークが付いているオブジェクト数
     */
    u32 objects_marked;

    /**
     * マークが付いていないオブジェクト数
     */
    u32 objects_nomarked;
};

/**
 * メモリの確保状態を取得する
 */
void getAllocatedInfo(AllocatedHeapInfo *result);

}

}

namespace jc {

class Object;
class MemoryCheck {
public:
    /**
     * マークを付ける
     */
    static void mark(void* memory, const char* const file, const int line);

    /**
     * リークマークを外す
     */
    static void unmark(void* memory, const char* const file, const int line);

    template<typename T>
    static inline void* getHeader(T* p) {
        Object *head = dynamic_cast<Object*>(p);
        if (head) {
            return head;
        } else {
            return p;
        }
    }
public:

    /**
     * メモリ情報をダンプする
     */
    static void dump();

    template<typename T>
    static inline void marking(T* p, const char* const file, const int line) {
        MemoryCheck::mark(getHeader(p), file, line);
    }

    template<typename T>
    static inline void unmarking(T* p, const char* const file, const int line) {
        MemoryCheck::unmark(getHeader(p), file, line);
    }

private:
    /**
     * new出来ない
     */
    MemoryCheck();
};

#ifdef  DEBUG_MEMORYCHECK

#define jcmark(ptr)     jc::MemoryCheck::marking(ptr, __FILE__, __LINE__)

#define jcunmark(ptr)   jc::MemoryCheck::unmark(ptr, __FILE__, __LINE__)

#define jcmarkvoid(ptr)     jc::MemoryCheck::mark(ptr, __FILE__, __LINE__)

#define jcunmarkvoid(ptr)     jc::MemoryCheck::unmark(ptr, __FILE__, __LINE__)

#else

#define jcmark(ptr)

#define jcunmark(ptr)

#define jcmarkvoid(ptr)

#define jcunmarkvoid(ptr)

#endif

}

#endif /* JCNEW_H_ */
