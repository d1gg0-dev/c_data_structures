#ifndef DATA_STRUCTURES_UTILS_H
#define DATA_STRUCTURES_UTILS_H

// ============================================================================
// File: utils.h
// Description:
//     A collection of common utility functions and macros for use throughout
//     the Data Structures library. These utilities provide:
//
//       - Standard comparison, printing, and hashing functions for common
//       types.
//       - Mathematical helper functions for power-of-two operations and
//       capacity growth.
//       - Memory manipulation utilities (byte swapping, reversing).
//       - Macros for safe type conversions and array iteration.
//
//     This module centralizes low-level helper routines to avoid redundancy
//     and ensure consistent behavior across all data structures.
// ============================================================================

#include "error.h" // For error types if needed by some utility functions
#include "types.h" // Fundamental type definitions (i32, f64, usize, etc.)

// ---------------------------------------------------------------------------
// SECTION 1: Standard comparison functions.
// ---------------------------------------------------------------------------
// These functions implement basic comparison logic for fundamental data types.
// They return:
//     < 0  if a < b
//     = 0  if a == b
//     > 0  if a > b
//
// Each function follows the CompareFn signature defined in types.h, making
// them directly usable as function pointers in generic data structures like
// trees, heaps, and sorted arrays.

int compare_int(cptr a, cptr b);
int compare_float(cptr a, cptr b);
int compare_double(cptr a, cptr b);
int compare_string(cptr a, cptr b); // Uses strcmp-like semantics
int compare_char(cptr a, cptr b);

// ---------------------------------------------------------------------------
// SECTION 2: Standard printing functions.
// ---------------------------------------------------------------------------
// These functions print the value pointed to by 'data' using a consistent
// format suitable for debugging. They can be passed as PrintFn function
// pointers to data structure display routines.
//
// Example usage:
//     print_int(&value);
//     print_string("Hello");

void print_int(cptr data);
void print_float(cptr data);
void print_double(cptr data);
void print_string(cptr data);
void print_char(cptr data);
void print_hex(cptr data); // Prints value in hexadecimal (useful for addresses)

// ---------------------------------------------------------------------------
// SECTION 3: Standard hash functions.
// ---------------------------------------------------------------------------
// These functions compute hash values for various data types. They are designed
// to be simple, portable, and consistent. Used for implementing hash-based
// data structures such as hash maps or sets.
//
// The 'size' parameter is kept for API consistency, though it may not always
// be necessary depending on the data type.
//
// Expected return: 64-bit unsigned hash value (u64).

u64 hash_int(cptr data, usize size);
u64 hash_float(cptr data, usize size);
u64 hash_double(cptr data, usize size);
u64 hash_string(cptr data,
                usize size); // Typically uses a polynomial or FNV hash

// ---------------------------------------------------------------------------
// SECTION 4: Mathematical utility functions.
// ---------------------------------------------------------------------------
// These small helpers are often used in dynamic data structures such as
// dynamic arrays, hash tables, and memory managers.
//
// Functions:
//   next_power_of_two() -> Returns the smallest power of two >= n.
//   is_power_of_two()   -> Returns true if n is a power of two.
//   calculate_growth()  -> Calculates next capacity based on current usage.
//
// Example usage:
//     usize cap = next_power_of_two(37);   // -> 64
//     bool ok = is_power_of_two(128);      // -> true

usize next_power_of_two(usize n);
bool is_power_of_two(usize n);
usize calculate_growth(usize current, usize min_growth);

// ---------------------------------------------------------------------------
// SECTION 5: Byte manipulation utilities.
// ---------------------------------------------------------------------------
// These functions perform low-level memory transformations.
//
// swap_bytes():
//     Swaps the contents of two memory blocks of equal size.
//
// reverse_bytes():
//     Reverses the order of elements in a block of memory. Useful for
//     reversing arrays or implementing byte-level data reordering.
//
// Example usage:
//     int a = 1, b = 2;
//     swap_bytes(&a, &b, sizeof(int));
//     // now a == 2, b == 1
//
//     reverse_bytes(array, size, sizeof(int));

void swap_bytes(ptr a, ptr b, usize size);
void reverse_bytes(ptr data, usize size, usize element_size);

// ---------------------------------------------------------------------------
// SECTION 6: Safe type conversion macros.
// ---------------------------------------------------------------------------
// These macros perform safe, explicit casts from generic pointers to typed
// values. They improve code readability and reduce casting mistakes in
// generic data structures.
//
// Example usage:
//     int value = INT_VALUE(ptr);
//     double val = DOUBLE_VALUE(ptr);

#define INT_VALUE(ptr) (*(int *)(ptr))
#define FLOAT_VALUE(ptr) (*(float *)(ptr))
#define DOUBLE_VALUE(ptr) (*(double *)(ptr))
#define CHAR_VALUE(ptr) (*(char *)(ptr))

// ---------------------------------------------------------------------------
// SECTION 7: Array iteration macro.
// ---------------------------------------------------------------------------
// Simplifies iteration over a contiguous array in a type-safe manner.
//
// Example usage:
//     FOR_EACH(int, x, arr, size) {
//         printf("%d\n", *x);
//     }
//
// Expands to a standard for-loop with pointer arithmetic.

#define FOR_EACH(type, var, array, size)                                       \
    for(type *var = (type *)(array); var != (type *)(array) + (size); ++var)

// ---------------------------------------------------------------------------
// SECTION 8: Type-safe comparison macro generator.
// ---------------------------------------------------------------------------
// This macro creates a strongly-typed comparison function for a specific type.
//
// Usage example:
//     DEFINE_COMPARE_FN(int, myint)
//     // Expands into:
//     // int compare_myint(cptr a, cptr b) { ... }
//
// The function uses the standard pattern (*a_ > *b_) - (*a_ < *b_) which
// ensures the result is -1, 0, or +1 depending on the comparison outcome.

#define DEFINE_COMPARE_FN(type, name)                                          \
    int compare_##name(cptr a, cptr b)                                         \
    {                                                                          \
        const type *a_ = (const type *)a;                                      \
        const type *b_ = (const type *)b;                                      \
        return (*a_ > *b_) - (*a_ < *b_);                                      \
    }

#endif // !DATA_STRUCTURES_UTILS_H
