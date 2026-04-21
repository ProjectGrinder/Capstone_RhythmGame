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

#elif defined(__GNUC__) || defined(__clang__)
    // GCC/Clang built-in atomics
    #define ATOMIC_LOAD(ptr) __atomic_load_n((ptr), __ATOMIC_ACQUIRE)

    #define ATOMIC_STORE(ptr, val) __atomic_store_n((ptr), (val), __ATOMIC_RELEASE)
#else
    #error "Compiler not supported"
#endif
