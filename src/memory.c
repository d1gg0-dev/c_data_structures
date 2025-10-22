#include "../include/memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Global memory statistics instance.
 *
 * This structure accumulates information about memory allocation
 * throughout the lifetime of the program. It allows tracking total
 * allocations, frees, peak usage, and other runtime statistics.
 */
static Memory_Stats stats = {0};

/* ============================================================================
 *  BASIC MEMORY ALLOCATION FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Allocates a block of memory of the given size.
 *
 * Wraps the standard `malloc()` but adds memory tracking via `stats`.
 * Updates total allocated bytes, current usage, and peak usage.
 *
 * @param size The number of bytes to allocate.
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
ptr ds_malloc(usize size)
{
    ptr result = malloc(size);
    if(result)
    {
        // Track allocation statistics
        stats.total_allocated += size;
        stats.current_usage += size;
        stats.allocation_count++;

        // Update peak usage if this allocation exceeds it
        if(stats.current_usage > stats.peak_usage)
        {
            stats.peak_usage = stats.current_usage;
        }
    }
    return result;
}

/**
 * @brief Allocates and zero-initializes a block of memory.
 *
 * Equivalent to `calloc()`, ensuring all bytes are set to zero.
 * Tracks memory usage similarly to ds_malloc().
 *
 * @param count Number of elements to allocate.
 * @param size  Size of each element in bytes.
 * @return Pointer to the allocated and zeroed memory.
 */
ptr ds_calloc(usize count, usize size)
{
    usize total_size = count * size;
    ptr result = calloc(count, size);
    if(result)
    {
        stats.total_allocated += total_size;
        stats.current_usage += total_size;
        stats.allocation_count++;

        if(stats.current_usage > stats.peak_usage)
        {
            stats.peak_usage = stats.current_usage;
        }
    }
    return result;
}

/**
 * @brief Reallocates memory to a new size.
 *
 * This wraps `realloc()` but does not track the *old* size of the memory block,
 * meaning statistics are approximate. In a full memory tracker, each pointer’s
 * size would be recorded to accurately adjust stats.
 *
 * @param pointer Pointer to the existing block (may be NULL).
 * @param new_size New size in bytes.
 * @return Pointer to the reallocated memory, or NULL if allocation fails.
 */
ptr ds_realloc(ptr pointer, usize new_size)
{
    // NOTE: For simplicity, old size is not tracked.
    ptr result = realloc(pointer, new_size);
    if(result && new_size > 0)
    {
        stats.total_allocated += new_size;
        stats.current_usage += new_size;
        // Peak usage is automatically updated in malloc/calloc.
    }
    return result;
}

/**
 * @brief Frees a block of memory previously allocated.
 *
 * This wrapper around `free()` updates only the free count, not memory size,
 * since we lack information about the block’s original size.
 *
 * @param pointer Pointer to the memory block to free.
 */
void ds_free(ptr pointer)
{
    if(pointer)
    {
        free(pointer);
        stats.free_count++;

        // NOTE: `current_usage` cannot be decremented correctly
        // without tracking the original block size.
    }
}

/* ============================================================================
 *  ARRAY-BASED MEMORY OPERATIONS
 * ============================================================================
 */

/**
 * @brief Allocates an array with zero-initialized elements.
 *
 * Simplifies calls to calloc by abstracting element count and size.
 */
ptr ds_alloc_array(usize count, usize element_size)
{
    return ds_calloc(count, element_size);
}

/**
 * @brief Reallocates an array to hold a new number of elements.
 */
ptr ds_realloc_array(ptr array, usize new_count, usize element_size)
{
    return ds_realloc(array, new_count * element_size);
}

/* ============================================================================
 *  MEMORY COPYING AND MANIPULATION UTILITIES
 * ============================================================================
 */

/**
 * @brief Copies a block of memory from source to destination.
 *
 * Equivalent to `memcpy()` but wrapped for consistent API style.
 * Use when source and destination memory regions do *not* overlap.
 */
void ds_memcpy(ptr dest, cptr src, usize size) { memcpy(dest, src, size); }

/**
 * @brief Copies memory safely even when regions overlap.
 *
 * Equivalent to `memmove()`. Use when source and destination
 * may overlap in memory.
 */
void ds_memmove(ptr dest, cptr src, usize size) { memmove(dest, src, size); }

/**
 * @brief Compares two memory blocks byte by byte.
 *
 * Equivalent to `memcmp()`. Returns:
 *  - < 0 if ptr1 < ptr2
 *  - 0 if equal
 *  - > 0 if ptr1 > ptr2
 */
int ds_memcmp(cptr ptr1, cptr ptr2, usize size)
{
    return memcmp(ptr1, ptr2, size);
}

/**
 * @brief Fills a block of memory with a specific byte value.
 *
 * Equivalent to `memset()`, commonly used for initialization or reset.
 */
void ds_memset(ptr dest, byte value, usize size) { memset(dest, value, size); }

/* ============================================================================
 *  MEMORY STATISTICS FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the current snapshot of memory statistics.
 *
 * @return A copy of the global Memory_Stats structure.
 */
Memory_Stats ds_get_memory_stats(void) { return stats; }

/**
 * @brief Resets all memory statistics to zero.
 *
 * Useful for benchmarking or testing purposes.
 */
void ds_reset_memory_stats(void) { stats = (Memory_Stats){0}; }

/**
 * @brief Prints current memory usage statistics to stdout.
 *
 * Useful for debugging or profiling. Displays total allocations,
 * frees, and usage metrics in bytes.
 */
void ds_print_memory_stats(void)
{
    printf("Memory Statistics:\n");
    printf("  Total Allocated: %zu bytes\n", stats.total_allocated);
    printf("  Total Freed:     %zu bytes\n", stats.total_freed);
    printf("  Current Usage:   %zu bytes\n", stats.current_usage);
    printf("  Peak Usage:      %zu bytes\n", stats.peak_usage);
    printf("  Allocation Count:%zu\n", stats.allocation_count);
    printf("  Free Count:      %zu\n", stats.free_count);
}
