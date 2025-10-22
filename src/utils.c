#include "../include/utils.h"
#include <math.h>
#include <string.h>

/* ============================================================================
 *  COMPARISON FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Compares two integers.
 *
 * @param a Pointer to the first integer.
 * @param b Pointer to the second integer.
 * @return -1 if a < b, 0 if a == b, 1 if a > b.
 */
int compare_int(cptr a, cptr b)
{
    const int *a_int = (const int *)a;
    const int *b_int = (const int *)b;
    return (*a_int > *b_int) - (*a_int < *b_int);
}

/**
 * @brief Compares two floats with precision tolerance.
 *
 * Uses an epsilon comparison to account for floating-point imprecision.
 *
 * @param a Pointer to the first float.
 * @param b Pointer to the second float.
 * @return -1 if a < b, 0 if nearly equal, 1 if a > b.
 */
int compare_float(cptr a, cptr b)
{
    const float *a_float = (const float *)a;
    const float *b_float = (const float *)b;

    // Small epsilon to consider very close values as equal
    if(fabsf(*a_float - *b_float) < 1e-6f)
        return 0;

    return (*a_float > *b_float) ? 1 : -1;
}

/**
 * @brief Compares two double-precision floating point values.
 *
 * Uses a smaller epsilon for better precision.
 */
int compare_double(cptr a, cptr b)
{
    const double *a_double = (const double *)a;
    const double *b_double = (const double *)b;

    if(fabs(*a_double - *b_double) < 1e-12)
        return 0;

    return (*a_double > *b_double) ? 1 : -1;
}

/**
 * @brief Compares two C-style strings lexicographically.
 *
 * Uses the standard library function strcmp().
 *
 * @return <0 if a < b, 0 if equal, >0 if a > b.
 */
int compare_string(cptr a, cptr b)
{
    return strcmp((const char *)a, (const char *)b);
}

/**
 * @brief Compares two characters by their ASCII values.
 */
int compare_char(cptr a, cptr b)
{
    const char *a_char = (const char *)a;
    const char *b_char = (const char *)b;
    return (*a_char > *b_char) - (*a_char < *b_char);
}

/* ============================================================================
 *  MATHEMATICAL UTILITIES
 * ============================================================================
 */

/**
 * @brief Returns the next power of two greater than or equal to `n`.
 *
 * Uses bitwise operations to efficiently compute the result.
 * For example: next_power_of_two(5) → 8
 */
usize next_power_of_two(usize n)
{
    if(n == 0)
        return 1;

    // Fill all bits to the right of the highest set bit
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
#ifdef __LP64__
    n |= n >> 32; // For 64-bit systems
#endif
    return n + 1;
}

/**
 * @brief Checks if a number is a power of two.
 *
 * A number `n` is a power of two if it has exactly one bit set.
 */
bool is_power_of_two(usize n) { return n != 0 && (n & (n - 1)) == 0; }

/**
 * @brief Calculates the new capacity for a dynamically growing structure.
 *
 * Doubles the current capacity unless that’s smaller than the minimum growth.
 * Ensures controlled growth for structures like dynamic arrays.
 */
usize calculate_growth(usize current, usize min_growth)
{
    if(current == 0)
        return min_growth > 1 ? min_growth : 1;

    usize new_capacity = current * 2;
    if(new_capacity < current + min_growth)
        new_capacity = current + min_growth;

    return new_capacity;
}

/* ============================================================================
 *  MEMORY AND BYTE MANIPULATION
 * ============================================================================
 */

/**
 * @brief Swaps two memory regions byte-by-byte.
 *
 * This is a low-level operation used in sorting algorithms or buffer
 * management. Works for any data type as long as the total byte size is
 * provided.
 */
void swap_bytes(ptr a, ptr b, usize size)
{
    byte *a_bytes = (byte *)a;
    byte *b_bytes = (byte *)b;

    for(usize i = 0; i < size; i++)
    {
        byte temp = a_bytes[i];
        a_bytes[i] = b_bytes[i];
        b_bytes[i] = temp;
    }
}
