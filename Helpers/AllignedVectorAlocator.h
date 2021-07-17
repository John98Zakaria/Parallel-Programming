//
// Created by John on 18.07.2021.
//
// Source :https://stackoverflow.com/questions/67906479/what-does-memory-32bit-alignement-constraint-mean-for-avx/67906583#67906583

#ifndef HPC_ALLIGNEDVECTORALOCATOR_H
#define HPC_ALLIGNEDVECTORALOCATOR_H
#include <cstdlib>
#include <memory>

// Following includes for tests only
#include <vector>
#include <iostream>
#include <cmath>


template <typename T, std::size_t N>
class AlignmentAllocator {
public:
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T & reference;
    typedef const T & const_reference;

public:
    inline AlignmentAllocator() throw() {}
    template <typename T2> inline AlignmentAllocator(const AlignmentAllocator<T2, N> &) throw() {}
    inline ~AlignmentAllocator() throw() {}
    inline pointer adress(reference r) { return &r; }
    inline const_pointer adress(const_reference r) const { return &r; }
    inline pointer allocate(size_type n);
    inline void deallocate(pointer p, size_type);
    inline void construct(pointer p, const value_type & wert);
    inline void destroy(pointer p) { p->~value_type(); }
    inline size_type max_size() const throw() { return size_type(-1) / sizeof(value_type); }
    template <typename T2> struct rebind { typedef AlignmentAllocator<T2, N> other; };
    bool operator!=(const AlignmentAllocator<T, N> & other) const { return !(*this == other); }
    bool operator==(const AlignmentAllocator<T, N> & other) const { return true; }
};

template <typename T, std::size_t N>
inline typename AlignmentAllocator<T, N>::pointer AlignmentAllocator<T, N>::allocate(size_type n) {
#if _MSC_VER
    return (pointer)_aligned_malloc(n * sizeof(value_type), N);
#else
    void * p0 = nullptr;
    int r = posix_memalign(&p0, N, n * sizeof(value_type));
    if (r != 0) return 0;
    return (pointer)p0;
#endif
}
template <typename T, std::size_t N>
inline void AlignmentAllocator<T, N>::deallocate(pointer p, size_type) {
#if _MSC_VER
    _aligned_free(p);
#else
    std::free(p);
#endif
}
template <typename T, std::size_t N>
inline void AlignmentAllocator<T, N>::construct(pointer p, const value_type & wert) {
    new (p) value_type(wert);
}

template<typename T, size_t N = 32>
using Aligned32Vector = std::vector<T, AlignmentAllocator<T, N>>;


#endif //HPC_ALLIGNEDVECTORALOCATOR_H
