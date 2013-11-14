/*
 * jcnew.h
 *
 *  Created on: 2013/11/14
 */
#ifndef JCNEW_H_
#define JCNEW_H_

#include    "jointcoding.h"
#include    <stddef.h>

namespace jc {

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

#define jcmark(ptr)     jc::MemoryCheck::marking(ptr, __FILE__, __LINE__);

#define jcunmark(ptr)   jc::MemoryCheck::unmark(ptr, __FILE__, __LINE__);


#define jcmarkvoid(ptr)     jc::MemoryCheck::mark(ptr, __FILE__, __LINE__);

#define jcunmarkvoid(ptr)     jc::MemoryCheck::unmark(ptr, __FILE__, __LINE__);

}

#endif /* JCNEW_H_ */
