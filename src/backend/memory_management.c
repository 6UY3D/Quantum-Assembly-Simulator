c

#include "memory_management.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#  include <malloc.h>
#endif

void* aligned_malloc(size_t size, size_t alignment) {
    void* ptr = NULL;
#if defined(_WIN32)
    ptr = _aligned_malloc(size, alignment);
    if (!ptr) {
        fprintf(stderr, "aligned_malloc: allocation failed (Windows).\n");
    }
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
    int ret = posix_memalign(&ptr, alignment, size);
    if (ret != 0) {
        fprintf(stderr, "aligned_malloc: allocation failed (posix_memalign returned %d).\n", ret);
        ptr = NULL;
    }
#else
    // Fallback: allocate extra space and store the offset
    void* raw = malloc(size + alignment - 1 + sizeof(void*));
    if (!raw) {
        fprintf(stderr, "aligned_malloc: fallback allocation failed.\n");
        return NULL;
    }
    // Align the address
    char* aligned = (char*)(((size_t)(raw) + sizeof(void*) + alignment - 1) & ~(alignment - 1));
    ((void**)aligned)[-1] = raw;
    ptr = aligned;
#endif
    return ptr;
}

void aligned_free(void* ptr) {
    if (!ptr) return;
#if defined(_WIN32)
    _aligned_free(ptr);
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
    free(ptr);
#else
    // Fallback
    free(((void**)ptr)[-1]);
#endif
}
