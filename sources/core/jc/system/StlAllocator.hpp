/*
 * StlAllocator.hpp
 *
 *  Created on: 2013/11/26
 */
#ifndef STLALLOCATOR_HPP_
#define STLALLOCATOR_HPP_

#include    "jointcoding.h"

namespace jc {

/**
 * 排他制御を行った上でメモリを確保する
 */
extern void* heap_alloc(size_t size, const u32 systemSize, const char * const file, const int line);

extern void heap_free(void* p, const u32 systemSize);

template<class T>
class StlAllocator: public std::allocator<T> {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    StlAllocator() {
    }
    StlAllocator(const StlAllocator& x) {
    }

    template<class U>
    StlAllocator(const StlAllocator<U>& x) {
    }

    T* allocate(size_type n, const_pointer hint = 0) {
//        return (pointer) std::malloc(n * sizeof(T));
//        return mark_new T[n];
        return (T*) (heap_alloc(n * sizeof(T), 0, __FILE__, __LINE__));
    }

    void deallocate(pointer ptr, size_type n) {
//        std::free(ptr);
//        jclogf("deallocate (%x)", ptr);
//        u8* p = (u8*) ptr;
//        SAFE_DELETE(p);
        heap_free(ptr, 0);
    }

    template<class U>
    struct rebind {
        typedef StlAllocator<U> other;
    };
};
}

#endif /* STLALLOCATOR_HPP_ */
