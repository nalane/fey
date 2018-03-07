#pragma once

#include <stdlib.h>

inline void* aligned_malloc(size_t size, size_t align) {
#ifdef _WIN32
    return _aligned_malloc(size, align);
#else
    void* memory;
    posix_memalign(&memory, align, size);
    return memory;
#endif
}

inline void aligned_free(void* pointer) {
#ifdef _WIN32
    _aligned_free(pointer);
#else
    free(pointer);
#endif
}