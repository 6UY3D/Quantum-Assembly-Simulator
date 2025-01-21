#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * \brief Allocates a block of memory aligned to 'alignment' bytes. 
 * \param size Size in bytes
 * \param alignment Alignment in bytes (must be power of 2)
 * \return Pointer to aligned memory or NULL on failure
 */
void* aligned_malloc(size_t size, size_t alignment);

/**
 * \brief Frees memory allocated by aligned_malloc.
 * \param ptr Pointer to memory block
 */
void aligned_free(void* ptr);

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_MANAGEMENT_H */
