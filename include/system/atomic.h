#pragma once

typedef volatile size_t atomic_size_t;

#if defined(_MSC_VER)
    #include <intrin.h>
    #define ATOMIC_LOAD(ptr)                                                                                           \
        (*(ptr));                                                                                                      \
        _ReadWriteBarrier()

    #define ATOMIC_STORE(ptr, val)                                                                                     \
        _ReadWriteBarrier();                                                                                           \
        (*(ptr)) = (val)

    #if defined(_WIN64)
        #define ATOMIC_FETCH_ADD(ptr, val) _InterlockedExchangeAdd64((volatile __int64 *) (ptr), (val))
    #else
        #define ATOMIC_FETCH_ADD(ptr, val) _InterlockedExchangeAdd((volatile long *) (ptr), (val))
    #endif

#elif defined(__GNUC__) || defined(__clang__)
    #define ATOMIC_LOAD(ptr) __atomic_load_n((ptr), __ATOMIC_ACQUIRE)

    #define ATOMIC_STORE(ptr, val) __atomic_store_n((ptr), (val), __ATOMIC_RELEASE)
    #define ATOMIC_FETCH_ADD(ptr, val) __atomic_fetch_add((ptr), (val), __ATOMIC_RELAXED)
#else
    #error "Compiler not supported"
#endif
