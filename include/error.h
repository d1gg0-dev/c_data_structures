#ifndef DATA_STRUCTURES_ERROR_H
#define DATA_STRUCTURES_ERROR_H

// ============================================================================
// File: error.h
// Description:
//     Provides a lightweight, consistent error handling system for data
//     structures and general-purpose libraries in C.
//
//     This module defines:
//       - A standardized set of error codes.
//       - A Result structure for carrying both the error type and context.
//       - Macros for easily creating, checking, and propagating errors.
//       - Functions to query error state and description.
//
//     The design mimics the "Result" or "Either" types found in languages
//     like Rust or Go, but adapted for idiomatic and portable C usage.
// ============================================================================

#include "types.h" // Uses bool_t and other basic type aliases defined in types.h

// ---------------------------------------------------------------------------
// SECTION 1: Enumeration of standardized error/result codes.
// ---------------------------------------------------------------------------
// Each enumerator represents a specific type of failure or success condition.
// These codes can be used to provide meaningful feedback to higher-level code.
//
// Prefix "DS_" stands for "Data Structures", ensuring naming consistency.

typedef enum
{
    DS_SUCCESS = 0,               // Operation completed successfully
    DS_ERROR_INVALID_ARGUMENT,    // Invalid parameter passed to a function
    DS_ERROR_MEMORY_ALLOCATION,   // Memory allocation failed (malloc/calloc
                                  // error)
    DS_ERROR_INDEX_OUT_OF_BOUNDS, // Attempted to access an invalid index
    DS_ERROR_EMPTY_CONTAINER, // Operation on an empty container (e.g., pop from
                              // empty stack)
    DS_ERROR_FULL_CONTAINER,  // Operation on a full container (e.g., push to
                              // full queue)
    DS_ERROR_NOT_FOUND,       // Requested element not found
    DS_ERROR_DUPLICATE,      // Duplicate element detected (e.g., in set or map)
    DS_ERROR_OVERFLOW,       // Numeric or buffer overflow detected
    DS_ERROR_UNDERFLOW,      // Numeric or buffer underflow detected
    DS_ERROR_NULL_POINTER,   // Null pointer passed where non-null was expected
    DS_ERROR_CORRUPTED_DATA, // Data structure integrity check failed
    DS_ERROR_NOT_IMPLEMENTED, // Functionality not yet implemented
    DS_ERROR_UNKNOWN          // Catch-all for unexpected errors
} Result_Code;

// ---------------------------------------------------------------------------
// SECTION 2: Result structure definition.
// ---------------------------------------------------------------------------
// The Result type encapsulates an operation’s outcome, including both
// the result code and additional debugging context.
//
// Fields:
//   code     -> The Result_Code describing the error or success state.
//   message  -> Human-readable message describing the situation.
//   file     -> The source file where the result was created (__FILE__).
//   line     -> The source line where the result was created (__LINE__).
//
// This structure allows detailed error tracing during development and
// debugging without adding runtime exceptions or global state.

typedef struct
{
        Result_Code code;
        const char *message;
        const char *file;
        int line;
} Result;

// ---------------------------------------------------------------------------
// SECTION 3: Convenience macros for creating results.
// ---------------------------------------------------------------------------
// These macros simplify creating Result values inline, automatically
// capturing the source file and line number for traceability.
//
// Usage examples:
//     return RESULT_SUCCESS;
//     return RESULT_ERROR(DS_ERROR_NOT_FOUND, "Element not found in hash
//     table");

#define RESULT_SUCCESS ((Result){DS_SUCCESS, "Success.", __FILE__, __LINE__})
#define RESULT_ERROR(code, msg) ((Result){code, msg, __FILE__, __LINE__})

// ---------------------------------------------------------------------------
// SECTION 4: Function declarations for error utilities.
// ---------------------------------------------------------------------------
// Implementations (in error_handling.c) typically use switch statements
// or static lookup tables to provide human-readable descriptions.

const char *
result_description(Result_Code code); // Converts a code to descriptive text
bool result_is_success(
    Result result); // Returns true if result.code == DS_SUCCESS
bool result_is_error(
    Result result); // Returns true if result.code != DS_SUCCESS

// ---------------------------------------------------------------------------
// SECTION 5: Error checking and propagation macros.
// ---------------------------------------------------------------------------
// CHECK_RESULT(expr):
//     Evaluates an expression expected to return a Result.
//     If that result indicates an error, it immediately returns it from
//     the current function. This simplifies error propagation and avoids
//     repetitive boilerplate code.
//
// Example usage:
//     Result res = some_function();
//     CHECK_RESULT(res);  // If res is error, returns it immediately.

#define CHECK_RESULT(expr)                                                     \
    do                                                                         \
    {                                                                          \
        Result _result = (expr);                                               \
        if(result_is_error(_result))                                           \
            return _result;                                                    \
    } while(0)

// ---------------------------------------------------------------------------
// SECTION 6: Assertion macro for validating conditions.
// ---------------------------------------------------------------------------
// DS_ASSERT(condition, message):
//     Checks whether 'condition' is true. If false, returns an error Result
//     with DS_ERROR_INVALID_ARGUMENT and the provided message.
//
// Example usage:
//     DS_ASSERT(ptr != NULL, "Pointer must not be NULL");
//
// This is a safer alternative to the standard 'assert()', as it provides
// controlled error handling rather than abruptly aborting execution.

#define DS_ASSERT(condition, message)                                          \
    do                                                                         \
    {                                                                          \
        if(!(condition))                                                       \
            return RESULT_ERROR(DS_ERROR_INVALID_ARGUMENT, message);           \
    } while(0)

#endif // !DATA_STRUCTURES_ERROR_H
