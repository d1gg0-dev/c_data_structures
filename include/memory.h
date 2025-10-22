#ifndef DATA_STRUCTURES_MEMORY_H
#define DATA_STRUCTURES_MEMORY_H

// ============================================================================
// File: memory.h
// Description:
//     Provides a set of memory management utilities with built-in tracking
//     and error reporting for data structure implementations in C.
//
//     The goal of this module is to wrap the standard C allocation functions
//     (malloc, calloc, realloc, free) and add the following benefits:
//
//       - Unified API across all data structure modules.
//       - Centralized error handling using the Result system.
//       - Memory usage tracking for debugging and profiling.
//       - Helper macros for type-safe allocation.
//
//     This abstraction makes memory management more expressive and safer,
//     while still allowing low-level control when needed.
// ============================================================================

#include "error.h" // For Result and error codes used in allocation checks
#include "types.h" // For ptr, usize, byte, etc.

// ---------------------------------------------------------------------------
// SECTION 1: Memory statistics structure.
// ---------------------------------------------------------------------------
// Tracks memory allocation and release operations over the lifetime of the
// program. Useful for debugging memory leaks, performance issues, and tracking
// general allocation patterns.
//
// Fields:
//   total_allocated -> Cumulative total of bytes ever allocated.
//   total_freed     -> Cumulative total of bytes ever freed.
//   current_usage   -> Currently allocated memory (total_allocated -
//   total_freed). peak_usage      -> Maximum memory usage recorded so far.
//   allocation_count-> Number of allocation calls performed.
//   free_count      -> Number of free calls performed.

typedef struct
{
        usize total_allocated;
        usize total_freed;
        usize current_usage;
        usize peak_usage;
        usize allocation_count;
        usize free_count;
} Memory_Stats;

// ---------------------------------------------------------------------------
// SECTION 2: Generic memory management functions.
// ---------------------------------------------------------------------------
// These functions provide a layer of abstraction over the C standard library’s
// memory routines. They can be instrumented to track allocations and update
// memory statistics internally.
//
// Notes:
//   - ds_malloc() and ds_calloc() allocate new memory blocks.
//   - ds_realloc() resizes an existing block, preserving its contents.
//   - ds_free() releases a previously allocated block.
//
// All these functions should internally update the global Memory_Stats data.

ptr ds_malloc(usize size); // Allocates a block of memory of 'size' bytes
ptr ds_calloc(
    usize count,
    usize size); // Allocates and zero-initializes an array of 'count' elements
ptr ds_realloc(
    ptr pointer,
    usize new_size);       // Resizes memory block, preserving previous contents
void ds_free(ptr pointer); // Frees allocated memory block

// ---------------------------------------------------------------------------
// SECTION 3: Array-specific memory utilities.
// ---------------------------------------------------------------------------
// Convenience wrappers for allocating arrays of typed elements.
//
// These functions automatically multiply count * element_size to avoid
// arithmetic errors and ensure proper type-safe memory management.

ptr ds_alloc_array(usize count,
                   usize element_size); // Allocates memory for 'count' elements
ptr ds_realloc_array(ptr array, usize new_count,
                     usize element_size); // Resizes array memory block

// ---------------------------------------------------------------------------
// SECTION 4: Memory copy and manipulation utilities.
// ---------------------------------------------------------------------------
// Safe and consistent wrappers around standard memory manipulation routines.
// These provide predictable behavior and can be instrumented for logging.
//
// Functions:
//   ds_memcpy()  -> Copies a block of memory (non-overlapping).
//   ds_memmove() -> Copies a block of memory safely, even if overlapping.
//   ds_memcmp()  -> Compares two memory regions.
//   ds_memset()  -> Fills a memory region with a byte value.

void ds_memcpy(ptr dest, cptr src, usize size);   // Equivalent to memcpy()
void ds_memmove(ptr dest, cptr src, usize size);  // Equivalent to memmove()
int ds_memcmp(cptr ptr1, cptr ptr2, usize size);  // Equivalent to memcmp()
void ds_memset(ptr dest, byte value, usize size); // Equivalent to memset()

// ---------------------------------------------------------------------------
// SECTION 5: Memory statistics management.
// ---------------------------------------------------------------------------
// These functions provide introspection and control over internal memory
// tracking. Ideal for debugging, profiling, and testing for memory leaks.
//
// Example usage:
//     Memory_Stats stats = ds_get_memory_stats();
//     printf("Current usage: %zu bytes\n", stats.current_usage);
//
// ds_reset_memory_stats() clears all counters, useful for test isolation.

Memory_Stats
ds_get_memory_stats(void);        // Returns a snapshot of current statistics
void ds_reset_memory_stats(void); // Resets all counters to zero
void ds_print_memory_stats(void); // Prints formatted statistics to stdout

// ---------------------------------------------------------------------------
// SECTION 6: Helper macros for safe, type-aware allocations.
// ---------------------------------------------------------------------------
// These macros simplify allocation syntax while ensuring correct typing.
//
// Examples:
//     MyStruct* obj = ALLOC(MyStruct);
//     int* arr = ALLOC_ARRAY(int, 10);
//     arr = REALLOC_ARRAY(arr, int, 20);
//     FREE(arr);
//
// They improve readability and reduce casting errors.

#define ALLOC(type) ((type *)ds_malloc(sizeof(type)))
#define ALLOC_ARRAY(type, count) ((type *)ds_alloc_array(count, sizeof(type)))
#define REALLOC_ARRAY(ptr, type, new_count)                                    \
    ((type *)ds_realloc_array(ptr, new_count, sizeof(type)))
#define FREE(ptr) ds_free(ptr)

// ---------------------------------------------------------------------------
// SECTION 7: Checked allocation macro with integrated error handling.
// ---------------------------------------------------------------------------
// CHECKED_ALLOC(ptr, type):
//     Attempts to allocate memory for the given type and assigns it to 'ptr'.
//     If allocation fails, returns a Result error immediately.
//
// Example:
//     MyStruct* obj;
//     CHECKED_ALLOC(obj, MyStruct); // Automatically returns
//     DS_ERROR_MEMORY_ALLOCATION if fails
//
// This macro integrates the memory system with the Result-based error
// framework, encouraging safe and explicit error handling.

#define CHECKED_ALLOC(ptr, type)                                               \
    do                                                                         \
    {                                                                          \
        ptr = ALLOC(type);                                                     \
        if(!ptr)                                                               \
            return RESULT_ERROR(DS_ERROR_MEMORY_ALLOCATION,                    \
                                "Failed to allocate " #type);                  \
    } while(0)

#endif // !DATA_STRUCTURES_MEMORY_H
