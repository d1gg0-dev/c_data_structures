#include "../include/error.h"

/**
 * @brief Returns a human-readable description of a given ResultCode.
 *
 * This function translates enumeration values into descriptive strings.
 * It is useful for logging, debugging, and displaying error messages.
 *
 * @param code The ResultCode to describe.
 * @return A constant string describing the error.
 */
const char *result_description(Result_Code code)
{
    switch(code)
    {
    case DS_SUCCESS:
        return "Success.";
    case DS_ERROR_INVALID_ARGUMENT:
        return "Invalid argument.";
    case DS_ERROR_MEMORY_ALLOCATION:
        return "Memory allocation failed.";
    case DS_ERROR_INDEX_OUT_OF_BOUNDS:
        return "Index out of bounds.";
    case DS_ERROR_EMPTY_CONTAINER:
        return "Container is empty.";
    case DS_ERROR_FULL_CONTAINER:
        return "Container is full.";
    case DS_ERROR_NOT_FOUND:
        return "Element not found.";
    case DS_ERROR_DUPLICATE:
        return "Duplicate element.";
    case DS_ERROR_OVERFLOW:
        return "Overflow occurred.";
    case DS_ERROR_UNDERFLOW:
        return "Underflow occurred.";
    case DS_ERROR_NULL_POINTER:
        return "Null pointer.";
    case DS_ERROR_CORRUPTED_DATA:
        return "Data corrupted.";
    case DS_ERROR_NOT_IMPLEMENTED:
        return "Not implemented.";
    default:
        return "Unknown error.";
    }
}

/**
 * @brief Checks whether the result indicates success.
 *
 * @param result A Result structure containing an error code.
 * @return true if the operation succeeded, false otherwise.
 */
bool result_is_success(Result result) { return result.code == DS_SUCCESS; }

/**
 * @brief Checks whether the result indicates an error.
 *
 * @param result A Result structure containing an error code.
 * @return true if the operation failed, false otherwise.
 */
bool result_is_error(Result result) { return result.code != DS_SUCCESS; }
