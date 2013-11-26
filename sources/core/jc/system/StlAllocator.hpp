/*
 * StlAllocator.hpp
 *
 *  Created on: 2013/11/26
 */
#ifndef STLALLOCATOR_HPP_
#define STLALLOCATOR_HPP_

#include    "jointcoding.h"

namespace jc {
template<class T>
class StlAllocator: public std::allocator<T> {
public:
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;

    StlAllocator() {
    }
    StlAllocator(const StlAllocator& x) {
    }

    template<class U>
    StlAllocator(const StlAllocator<U>& x) {
    }

    T* allocate(size_type n, const_pointer hint = 0) {
//        return (pointer) std::malloc(n * sizeof(T));
        return mark_new T[n];
    }

    void deallocate(pointer ptr, size_type n) {
//        std::free(ptr);
        SAFE_DELETE(ptr);
    }

    template<class U>
    struct rebind {
        typedef StlAllocator<U> other;
    };
};
}

#endif /* STLALLOCATOR_HPP_ */
