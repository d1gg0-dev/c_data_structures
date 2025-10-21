#ifndef DATA_STRUCTURES_TYPES_H
#define DATA_STRUCTURES_TYPES_H

// ============================================================================
// File: types.h
// Description:
//     This header defines a consistent set of type aliases and generic
//     function pointer types for building data structures in C.
//     It provides semantic clarity, portability across platforms, and
//     improves readability and maintainability of future modules.
//
//     The file also defines macros and small helper types for handling
//     generic data, comparison, printing, hashing, and memory cleanup.
// ============================================================================

#include <stdbool.h> // For the built-in 'bool' type
#include <stddef.h>  // For size_t and ptrdiff_t (used in usize and isize)
#include <stdint.h>  // For fixed-width integer types (int8_t, uint32_t, etc.)

// ---------------------------------------------------------------------------
// SECTION 1: Basic type redefinitions for clarity and cross-platform safety.
// ---------------------------------------------------------------------------
// These aliases ensure a uniform naming convention for primitive types.
// Using short names like i32 or u64 simplifies code and improves readability.

typedef int8_t i8;   // 8-bit signed integer
typedef int16_t i16; // 16-bit signed integer
typedef int32_t i32; // 32-bit signed integer
typedef int64_t i64; // 64-bit signed integer

typedef uint8_t u8;   // 8-bit unsigned integer
typedef uint16_t u16; // 16-bit unsigned integer
typedef uint32_t u32; // 32-bit unsigned integer
typedef uint64_t u64; // 64-bit unsigned integer

typedef float f32;  // 32-bit floating-point number
typedef double f64; // 64-bit floating-point number

typedef char byte; // Alias for a single byte, often used for raw memory

// ---------------------------------------------------------------------------
// SECTION 2: Boolean type for semantic clarity.
// ---------------------------------------------------------------------------
// Although C already provides 'bool' from <stdbool.h>, defining bool_t
// emphasizes the project’s style consistency and allows future customization.

typedef bool bool_t;

// ---------------------------------------------------------------------------
// SECTION 3: Types for sizes, indices, and pointer arithmetic.
// ---------------------------------------------------------------------------
// usize and isize are common conventions (inspired by Rust and Zig) that
// provide semantic clarity when dealing with memory sizes or array indices.

typedef size_t usize;    // Unsigned type for size or array index
typedef ptrdiff_t isize; // Signed type for pointer difference or offset

// ---------------------------------------------------------------------------
// SECTION 4: Pointer aliases for semantic meaning.
// ---------------------------------------------------------------------------
// 'ptr' and 'cptr' make function signatures clearer, expressing intent.
// For example, 'cptr' (const void*) indicates read-only access to data.

typedef void *ptr;        // Generic pointer to mutable data
typedef const void *cptr; // Generic pointer to read-only data

// ---------------------------------------------------------------------------
// SECTION 5: Generic function pointer definitions.
// ---------------------------------------------------------------------------
// These typedefs define interfaces for reusable, generic data structures.
//
// CompareFn: used to compare two elements (return <0, 0, >0).
// DestroyFn: used to release memory or resources held by an element.
// PrintFn:   used for debugging and logging of elements.
// HashFn:    used to generate a hash value for hash-based structures.

typedef int (*CompareFn)(cptr a, cptr b);
typedef void (*DestroyFn)(ptr data);
typedef void (*PrintFn)(cptr data);
typedef u64 (*HashFn)(cptr data, usize size);

// ---------------------------------------------------------------------------
// SECTION 6: Generic data container structure.
// ---------------------------------------------------------------------------
// This structure holds a pointer to raw data and metadata that describes
// its size, capacity, and element size. It is intended to serve as a
// flexible base for implementing arrays, buffers, and other generic
// data containers.
//
// Fields:
//   data         -> Pointer to the allocated data block.
//   size         -> Number of elements currently stored.
//   capacity     -> Total number of elements the container can hold.
//   element_size -> Size in bytes of each element (used for memcpy, etc.)

typedef struct
{
        ptr data;
        usize size;
        usize capacity;
        usize element_size;
} GenericData;

// ---------------------------------------------------------------------------
// SECTION 7: Type-safe array declaration macro.
// ---------------------------------------------------------------------------
// This macro helps create a strongly-typed dynamic array structure for
// any given base type (e.g., int, float, etc.) while maintaining a
// consistent layout and field naming convention.
//
// Example usage:
//     DECLARE_TYPE(i32);
//     -> expands to: typedef struct { i32* data; usize size; usize capacity; }
//     i32Array;
//
// This approach combines the convenience of generic containers with
// type safety, ensuring arrays maintain specific element types.

#define DECLARE_TYPE(T)                                                        \
    typedef struct                                                             \
    {                                                                          \
            T *data;                                                           \
            usize size;                                                        \
            usize capacity;                                                    \
    } T##Array

// ---------------------------------------------------------------------------
// SECTION 8: Example array type declarations.
// ---------------------------------------------------------------------------
// These are predeclared typed arrays commonly used in many applications.

DECLARE_TYPE(i32);  // Array of 32-bit integers
DECLARE_TYPE(f64);  // Array of 64-bit floating-point numbers
DECLARE_TYPE(byte); // Array of bytes (useful for buffers or raw data)

#endif // DATA_STRUCTURES_TYPES_H
