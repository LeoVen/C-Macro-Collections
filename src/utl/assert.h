/**
 * assert.h
 *
 * Creation Date: 27/06/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Definition of many typed non-abortive assertions. Every assertion that fails
 * will print a message to stderr but will not abort the program execution.
 * There are 9 main macros:
 *
 * - cmc_assert
 * - cmc_assert_equals
 * - cmc_assert_not_equals
 * - cmc_assert_greater
 * - cmc_assert_greater_equals
 * - cmc_assert_lesser
 * - cmc_assert_lesser_equals
 * - cmc_assert_in_range
 * - cmc_assert_not_in_range
 *
 * Data types include:
 * - int8_t
 * - int16_t
 * - int32_t
 * - int64_t
 * - uint8_t
 * - uint16_t
 * - uint32_t
 * - uint64_t
 * - intmax_t
 * - uintmax_t
 * - size_t
 * - float
 * - double
 * - ptr
 * - bool (only for equals and not_equals)
 *
 * Specialized assertions that work with any data type:
 *
 * - cmc_assert_array_equals_any
 * - cmc_assert_array_within_any
 * - cmc_assert_array_outside_any
 * - cmc_assert_array_sorted_any
 */

#ifndef CMC_ASSERT_H
#define CMC_ASSERT_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CMC_ASSERT_GLUE_(dtype, assertion) cmc_assert_##assertion##_##dtype
#define CMC_ASSERT_GLUE(dtype, assertion) CMC_ASSERT_GLUE_(dtype, assertion)

/**
 * This is a global variable that can be used to test if all assertions passed.
 * If a single assertion fails, this variable will be set to false.
 *
 * This variable is also used in test.h utility to automatically pass or fail a
 * unit test. Once the unit test finishes, this variable is set back to true.
 */
static bool cmc_assert_state = true;

/**
 * expression : An expression that is expected to be evaluated to true.
 */
#define cmc_assert(expression)                                   \
    do                                                           \
    {                                                            \
        const char *str = #expression;                           \
        if (!(expression))                                       \
        {                                                        \
            cmc_assert_state = false;                            \
                                                                 \
            fprintf(stderr,                                      \
                    "Assertion Failed at %s:%s:%u for { %s }\n", \
                    __FILE__, __func__, __LINE__, str);          \
        }                                                        \
    } while (0)

/**
 * dtype    : One of the data types listed at the beggining of this file.
 * expected : Expected value for the assertion to pass.
 * actual   : Actual value tested against 'expected'.
 */
#define cmc_assert_equals(dtype, expected, actual) \
    CMC_ASSERT_GLUE(dtype, equals)                 \
    (expected, actual)

/**
 * dtype    : One of the data types listed at the beggining of this file.
 * expected : Expected value for the assertion to pass.
 * actual   : Actual value tested against 'expected'.
 */
#define cmc_assert_not_equals(dtype, not_expected, actual) \
    CMC_ASSERT_GLUE(dtype, not_equals)                     \
    (not_expected, actual)

/**
 * dtype    : One of the data types listed at the beggining of this file (excluding ptr).
 * boundary : Expected lowest possible value (excluded).
 * actual   : Actual value tested against the given boundary.
 */
#define cmc_assert_greater(dtype, boundary, actual) \
    CMC_ASSERT_GLUE(dtype, greater)                 \
    (boundary, actual)

/**
 * dtype    : One of the data types listed at the beggining of this file (excluding ptr).
 * boundary : Expected lowest possible value (included).
 * actual   : Actual value tested against the given boundary.
 */
#define cmc_assert_greater_equals(dtype, boundary, actual) \
    CMC_ASSERT_GLUE(dtype, greater_equals)                 \
    (boundary, actual)

/**
 * dtype    : One of the data types listed at the beggining of this file (excluding ptr).
 * boundary : Expected highest possible value (excluded).
 * actual   : Actual value tested against the given boundary.
 */
#define cmc_assert_lesser(dtype, boundary, actual) \
    CMC_ASSERT_GLUE(dtype, lesser)                 \
    (boundary, actual)

/**
 * dtype    : One of the data types listed at the beggining of this file (excluding ptr).
 * boundary : Expected highest possible value (included).
 * actual   : Actual value tested against the given boundary.
 */
#define cmc_assert_lesser_equals(dtype, boundary, actual) \
    CMC_ASSERT_GLUE(dtype, lesser_equals)                 \
    (boundary, actual)

/**
 * dtype       : One of the data types listed at the beggining of this file (excluding ptr).
 * lower_bound : Smallest value of the expected range.
 * upper_bound : Highest value of the expected range.
 * actual      : Actual value tested against the given range.
 *
 * Range is inclusive on both ends.
 */
#define cmc_assert_in_range(dtype, lower_bound, upper_bound, actual) \
    CMC_ASSERT_GLUE(dtype, in_range)                                 \
    (lower_bound, upper_bound, actual)

/**
 * dtype       : One of the data types listed at the beggining of this file (excluding ptr).
 * lower_bound : Smallest value of the not expected range.
 * upper_bound : Highest value of the not expected range.
 * actual      : Actual value tested against the given range.
 *
 * Range is inclusive on both ends.
 */
#define cmc_assert_not_in_range(dtype, lower_bound, upper_bound, actual) \
    CMC_ASSERT_GLUE(dtype, not_in_range)                                 \
    (lower_bound, upper_bound, actual)

/** ---------------------------------------------------------------------------
 *
 * Any data type
 *
 * ------------------------------------------------------------------------ **/

/**
 * Assert that two arrays are equal element by element.
 *
 * dtype       : The type of the array.
 * array1      : First array to be compared.
 * array2      : Second array to be compared.
 * comparator  : A function that has two 'dtype' arguments and returns -1 if the
 *               first is less then the second, 0 if both are equal or 1 if the
 *               first is greater then the second.
 * from_index  : First index from both arrays to compare (inclusive).
 * to_index    : Last index from both arrays to compare (inclusive).
 *
 * This macro can be used for any data type. Both bounds are inclusive.
 */
#define cmc_assert_array_equals_any(dtype, array1, array2, comparator, from_index, to_index)                                   \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str1__ = #array1;                                                                                          \
        const char *str2__ = #array2;                                                                                          \
        dtype *arr1__ = array1;                                                                                                \
        dtype *arr2__ = array2;                                                                                                \
        size_t from__ = from_index;                                                                                            \
        size_t to__ = to_index;                                                                                                \
        int (*cmp__)(dtype, dtype) = comparator;                                                                               \
                                                                                                                               \
        for (size_t i__ = from__; i__ <= to__; i__++)                                                                          \
        {                                                                                                                      \
            if (cmp__(arr1__[i__], arr2__[i__]) != 0)                                                                          \
            {                                                                                                                  \
                cmc_assert_state = false;                                                                                      \
                                                                                                                               \
                fprintf(stderr,                                                                                                \
                        "Assertion Failed at %s:%s:%u for { %s and %s }: Arrays values are not equal at index %" PRIuMAX "\n", \
                        __FILE__, __func__, __LINE__, str1__, str2__, i__);                                                    \
                                                                                                                               \
                break;                                                                                                         \
            }                                                                                                                  \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

/**
 * Assert that each element in the array are within bounds.
 *
 * dtype       : The type of the array.
 * array       : Array to be checked if its elements are within a given range
 * comparator  : A function that has two 'dtype' arguments and returns -1 if the
 *               first is less then the second, 0 if both are equal or 1 if the
 *               first is greater then the second.
 * lower_bound : Smallest value of the given range.
 * upper_bound : Highest value of the given range.
 * from_index  : First index from the array to compare (inclusive).
 * to_index    : Last index from the array to compare (inclusive).
 *
 * This macro can be used for any data type. Both bounds are inclusive.
 */
#define cmc_assert_array_within_any(dtype, array, comparator, lower_bound, upper_bound, from_index, to_index)             \
    do                                                                                                                    \
    {                                                                                                                     \
        const char *str__ = #array;                                                                                       \
        dtype *arr__ = array;                                                                                             \
        dtype lower_bound___ = lower_bound;                                                                               \
        dtype upper_bound___ = upper_bound;                                                                               \
        size_t from__ = from_index;                                                                                       \
        size_t to__ = to_index;                                                                                           \
        int (*cmp__)(dtype, dtype) = comparator;                                                                          \
                                                                                                                          \
        for (size_t i__ = from__; i__ <= to__; i__++)                                                                     \
        {                                                                                                                 \
            if (cmp__(arr__[i__], lower_bound___) < 0 || cmp__(arr__[i__], upper_bound___) > 0)                           \
            {                                                                                                             \
                cmc_assert_state = false;                                                                                 \
                                                                                                                          \
                fprintf(stderr,                                                                                           \
                        "Assertion Failed at %s:%s:%u for { %s }: Array value not within bounds at index %" PRIuMAX "\n", \
                        __FILE__, __func__, __LINE__, str__, i__);                                                        \
                                                                                                                          \
                break;                                                                                                    \
            }                                                                                                             \
        }                                                                                                                 \
                                                                                                                          \
    } while (0)

/**
 * Assert that each element in the array are within bounds.
 *
 * dtype       : The type of the array.
 * array       : Array to be checked if its elements are outside of a given range
 * comparator  : A function that has two 'dtype' arguments and returns -1 if the
 *               first is less then the second, 0 if both are equal or 1 if the
 *               first is greater then the second.
 * lower_bound : Smallest value of the given range.
 * upper_bound : Highest value of the given range.
 * from_index  : First index from the array to compare (inclusive).
 * to_index    : Last index from the array to compare (inclusive).
 *
 * This macro can be used for any data type. Both bounds are inclusive.
 */
#define cmc_assert_array_outside_any(dtype, array, comparator, lower_bound, upper_bound, from_index, to_index)             \
    do                                                                                                                     \
    {                                                                                                                      \
        const char *str__ = #array;                                                                                        \
        dtype *arr__ = array;                                                                                              \
        dtype lower_bound___ = lower_bound;                                                                                \
        dtype upper_bound___ = upper_bound;                                                                                \
        size_t from__ = from_index;                                                                                        \
        size_t to__ = to_index;                                                                                            \
        int (*cmp__)(dtype, dtype) = comparator;                                                                           \
                                                                                                                           \
        for (size_t i__ = from__; i__ <= to__; i__++)                                                                      \
        {                                                                                                                  \
            if (cmp__(arr__[i__], lower_bound___) >= 0 && cmp__(arr__[i__], upper_bound___) <= 0)                          \
            {                                                                                                              \
                cmc_assert_state = false;                                                                                  \
                                                                                                                           \
                fprintf(stderr,                                                                                            \
                        "Assertion Failed at %s:%s:%u for { %s }: Array value not outside bounds at index %" PRIuMAX "\n", \
                        __FILE__, __func__, __LINE__, str__, i__);                                                         \
                                                                                                                           \
                break;                                                                                                     \
            }                                                                                                              \
        }                                                                                                                  \
                                                                                                                           \
    } while (0)

/**
 * Assert that the array is sorted.
 *
 * dtype      : The type of the array.
 * array      : Array to be checked if it is sorted.
 * comparator : A function that has two 'dtype' arguments and returns -1 if the
 *              first is less then the second, 0 if both are equal or 1 if the
 *              first is greater then the second.
 * from_index : First index from the array to compare (inclusive).
 * to_index   : Last index from the array to compare (inclusive).
 *
 * This macro can be used for any data type.
 */
#define cmc_assert_array_sorted_any(dtype, array, comparator, from_index, to_index)                                                   \
    do                                                                                                                                \
    {                                                                                                                                 \
        const char *str__ = #array;                                                                                                   \
        dtype *arr__ = array;                                                                                                         \
        size_t from__ = from_index;                                                                                                   \
        size_t to__ = to_index;                                                                                                       \
        int (*cmp__)(dtype, dtype) = comparator;                                                                                      \
                                                                                                                                      \
        size_t iprev__ = from__;                                                                                                      \
                                                                                                                                      \
        for (size_t i__ = from__ + 1; i__ <= to__; i__++)                                                                             \
        {                                                                                                                             \
            if (cmp__(arr__[iprev__], arr__[i__]) > 0)                                                                                \
            {                                                                                                                         \
                cmc_assert_state = false;                                                                                             \
                                                                                                                                      \
                fprintf(stderr,                                                                                                       \
                        "Assertion Failed at %s:%s:%u for { %s }: Not sorted when comparing indexes %" PRIuMAX " and %" PRIuMAX "\n", \
                        __FILE__, __func__, __LINE__, str__, iprev__, i__);                                                           \
                                                                                                                                      \
                break;                                                                                                                \
            }                                                                                                                         \
                                                                                                                                      \
            iprev__ = i__;                                                                                                            \
        }                                                                                                                             \
                                                                                                                                      \
    } while (0)

/** ---------------------------------------------------------------------------
 *
 * Equals
 *
 * ------------------------------------------------------------------------ **/

#define cmc_assert_equals_int8_t(expected, actual)                                                        \
    do                                                                                                    \
    {                                                                                                     \
        const char *str = #actual;                                                                        \
        int8_t expected__ = (expected);                                                                   \
        int8_t actual__ = (actual);                                                                       \
                                                                                                          \
        if (expected__ != actual__)                                                                       \
        {                                                                                                 \
            cmc_assert_state = false;                                                                     \
                                                                                                          \
            fprintf(stderr,                                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                             \
        }                                                                                                 \
                                                                                                          \
    } while (0)

#define cmc_assert_equals_int16_t(expected, actual)                                                         \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        int16_t expected__ = (expected);                                                                    \
        int16_t actual__ = (actual);                                                                        \
                                                                                                            \
        if (expected__ != actual__)                                                                         \
        {                                                                                                   \
            cmc_assert_state = false;                                                                       \
                                                                                                            \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                               \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_int32_t(expected, actual)                                                         \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        int32_t expected__ = (expected);                                                                    \
        int32_t actual__ = (actual);                                                                        \
                                                                                                            \
        if (expected__ != actual__)                                                                         \
        {                                                                                                   \
            cmc_assert_state = false;                                                                       \
                                                                                                            \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                               \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_int64_t(expected, actual)                                                         \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        int64_t expected__ = (expected);                                                                    \
        int64_t actual__ = (actual);                                                                        \
                                                                                                            \
        if (expected__ != actual__)                                                                         \
        {                                                                                                   \
            cmc_assert_state = false;                                                                       \
                                                                                                            \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                               \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_uint8_t(expected, actual)                                                       \
    do                                                                                                    \
    {                                                                                                     \
        const char *str = #actual;                                                                        \
        uint8_t expected__ = (expected);                                                                  \
        uint8_t actual__ = (actual);                                                                      \
                                                                                                          \
        if (expected__ != actual__)                                                                       \
        {                                                                                                 \
            cmc_assert_state = false;                                                                     \
                                                                                                          \
            fprintf(stderr,                                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                             \
        }                                                                                                 \
                                                                                                          \
    } while (0)

#define cmc_assert_equals_uint16_t(expected, actual)                                                        \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        uint16_t expected__ = (expected);                                                                   \
        uint16_t actual__ = (actual);                                                                       \
                                                                                                            \
        if (expected__ != actual__)                                                                         \
        {                                                                                                   \
            cmc_assert_state = false;                                                                       \
                                                                                                            \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                               \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_uint32_t(expected, actual)                                                        \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        uint32_t expected__ = (expected);                                                                   \
        uint32_t actual__ = (actual);                                                                       \
                                                                                                            \
        if (expected__ != actual__)                                                                         \
        {                                                                                                   \
            cmc_assert_state = false;                                                                       \
                                                                                                            \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                               \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_uint64_t(expected, actual)                                                        \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        uint64_t expected__ = (expected);                                                                   \
        uint64_t actual__ = (actual);                                                                       \
                                                                                                            \
        if (expected__ != actual__)                                                                         \
        {                                                                                                   \
            cmc_assert_state = false;                                                                       \
                                                                                                            \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                               \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_intmax_t(expected, actual)                                                          \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        intmax_t expected__ = (expected);                                                                     \
        intmax_t actual__ = (actual);                                                                         \
                                                                                                              \
        if (expected__ != actual__)                                                                           \
        {                                                                                                     \
            cmc_assert_state = false;                                                                         \
                                                                                                              \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                                 \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_equals_uintmax_t(expected, actual)                                                         \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        uintmax_t expected__ = (expected);                                                                    \
        uintmax_t actual__ = (actual);                                                                        \
                                                                                                              \
        if (expected__ != actual__)                                                                           \
        {                                                                                                     \
            cmc_assert_state = false;                                                                         \
                                                                                                              \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                                 \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_equals_size_t(expected, actual)                                                            \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        size_t expected__ = (expected);                                                                       \
        size_t actual__ = (actual);                                                                           \
                                                                                                              \
        if (expected__ != actual__)                                                                           \
        {                                                                                                     \
            cmc_assert_state = false;                                                                         \
                                                                                                              \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                                 \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_equals_float(expected, actual)                                         \
    do                                                                                    \
    {                                                                                     \
        const char *str = #actual;                                                        \
        float expected__ = (expected);                                                    \
        float actual__ = (actual);                                                        \
                                                                                          \
        if (expected__ != actual__)                                                       \
        {                                                                                 \
            cmc_assert_state = false;                                                     \
                                                                                          \
            fprintf(stderr,                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);             \
        }                                                                                 \
                                                                                          \
    } while (0)

#define cmc_assert_equals_double(expected, actual)                                          \
    do                                                                                      \
    {                                                                                       \
        const char *str = #actual;                                                          \
        double expected__ = (expected);                                                     \
        double actual__ = (actual);                                                         \
                                                                                            \
        if (expected__ != actual__)                                                         \
        {                                                                                   \
            cmc_assert_state = false;                                                       \
                                                                                            \
            fprintf(stderr,                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);               \
        }                                                                                   \
                                                                                            \
    } while (0)

#define cmc_assert_equals_ptr(expected, actual)                                           \
    do                                                                                    \
    {                                                                                     \
        const char *str = #actual;                                                        \
        void *expected__ = (expected);                                                    \
        void *actual__ = (actual);                                                        \
                                                                                          \
        if (expected__ != actual__)                                                       \
        {                                                                                 \
            cmc_assert_state = false;                                                     \
                                                                                          \
            fprintf(stderr,                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %p Actual: %p\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);             \
        }                                                                                 \
                                                                                          \
    } while (0)

#define cmc_assert_equals__Bool(expected, actual)                                         \
    do                                                                                    \
    {                                                                                     \
        const char *str = #actual;                                                        \
        bool expected__ = (expected);                                                     \
        bool actual__ = (actual);                                                         \
                                                                                          \
        if (expected__ != actual__)                                                       \
        {                                                                                 \
            cmc_assert_state = false;                                                     \
                                                                                          \
            fprintf(stderr,                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %d Actual: %d\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);             \
        }                                                                                 \
                                                                                          \
    } while (0)

/** ---------------------------------------------------------------------------
 *
 * Not equals
 *
 * ------------------------------------------------------------------------ **/

#define cmc_assert_not_equals_int8_t(not_expected, actual)                                                    \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        int8_t not_expected__ = (not_expected);                                                               \
        int8_t actual__ = (actual);                                                                           \
                                                                                                              \
        if (not_expected__ == actual__)                                                                       \
        {                                                                                                     \
            cmc_assert_state = false;                                                                         \
                                                                                                              \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                             \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_not_equals_int16_t(not_expected, actual)                                                     \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        int16_t not_expected__ = (not_expected);                                                                \
        int16_t actual__ = (actual);                                                                            \
                                                                                                                \
        if (not_expected__ == actual__)                                                                         \
        {                                                                                                       \
            cmc_assert_state = false;                                                                           \
                                                                                                                \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                               \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_int32_t(not_expected, actual)                                                     \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        int32_t not_expected__ = (not_expected);                                                                \
        int32_t actual__ = (actual);                                                                            \
                                                                                                                \
        if (not_expected__ == actual__)                                                                         \
        {                                                                                                       \
            cmc_assert_state = false;                                                                           \
                                                                                                                \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                               \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_int64_t(not_expected, actual)                                                     \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        int64_t not_expected__ = (not_expected);                                                                \
        int64_t actual__ = (actual);                                                                            \
                                                                                                                \
        if (not_expected__ == actual__)                                                                         \
        {                                                                                                       \
            cmc_assert_state = false;                                                                           \
                                                                                                                \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                               \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_uint8_t(not_expected, actual)                                                   \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        uint8_t not_expected__ = (not_expected);                                                              \
        uint8_t actual__ = (actual);                                                                          \
                                                                                                              \
        if (not_expected__ == actual__)                                                                       \
        {                                                                                                     \
            cmc_assert_state = false;                                                                         \
                                                                                                              \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                             \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_not_equals_uint16_t(not_expected, actual)                                                    \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        uint16_t not_expected__ = (not_expected);                                                               \
        uint16_t actual__ = (actual);                                                                           \
                                                                                                                \
        if (not_expected__ == actual__)                                                                         \
        {                                                                                                       \
            cmc_assert_state = false;                                                                           \
                                                                                                                \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                               \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_uint32_t(not_expected, actual)                                                    \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        uint32_t not_expected__ = (not_expected);                                                               \
        uint32_t actual__ = (actual);                                                                           \
                                                                                                                \
        if (not_expected__ == actual__)                                                                         \
        {                                                                                                       \
            cmc_assert_state = false;                                                                           \
                                                                                                                \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                               \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_uint64_t(not_expected, actual)                                                    \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        uint64_t not_expected__ = (not_expected);                                                               \
        uint64_t actual__ = (actual);                                                                           \
                                                                                                                \
        if (not_expected__ == actual__)                                                                         \
        {                                                                                                       \
            cmc_assert_state = false;                                                                           \
                                                                                                                \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                               \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_intmax_t(not_expected, actual)                                                      \
    do                                                                                                            \
    {                                                                                                             \
        const char *str = #actual;                                                                                \
        intmax_t not_expected__ = (not_expected);                                                                 \
        intmax_t actual__ = (actual);                                                                             \
                                                                                                                  \
        if (not_expected__ == actual__)                                                                           \
        {                                                                                                         \
            cmc_assert_state = false;                                                                             \
                                                                                                                  \
            fprintf(stderr,                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                                 \
        }                                                                                                         \
                                                                                                                  \
    } while (0)

#define cmc_assert_not_equals_uintmax_t(not_expected, actual)                                                     \
    do                                                                                                            \
    {                                                                                                             \
        const char *str = #actual;                                                                                \
        uintmax_t not_expected__ = (not_expected);                                                                \
        uintmax_t actual__ = (actual);                                                                            \
                                                                                                                  \
        if (not_expected__ == actual__)                                                                           \
        {                                                                                                         \
            cmc_assert_state = false;                                                                             \
                                                                                                                  \
            fprintf(stderr,                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                                 \
        }                                                                                                         \
                                                                                                                  \
    } while (0)

#define cmc_assert_not_equals_size_t(not_expected, actual)                                                        \
    do                                                                                                            \
    {                                                                                                             \
        const char *str = #actual;                                                                                \
        size_t not_expected__ = (not_expected);                                                                   \
        size_t actual__ = (actual);                                                                               \
                                                                                                                  \
        if (not_expected__ == actual__)                                                                           \
        {                                                                                                         \
            cmc_assert_state = false;                                                                             \
                                                                                                                  \
            fprintf(stderr,                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);                                 \
        }                                                                                                         \
                                                                                                                  \
    } while (0)

#define cmc_assert_not_equals_float(not_expected, actual)                                     \
    do                                                                                        \
    {                                                                                         \
        const char *str = #actual;                                                            \
        float not_expected__ = (not_expected);                                                \
        float actual__ = (actual);                                                            \
                                                                                              \
        if (not_expected__ == actual__)                                                       \
        {                                                                                     \
            cmc_assert_state = false;                                                         \
                                                                                              \
            fprintf(stderr,                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);             \
        }                                                                                     \
                                                                                              \
    } while (0)

#define cmc_assert_not_equals_double(not_expected, actual)                                      \
    do                                                                                          \
    {                                                                                           \
        const char *str = #actual;                                                              \
        double not_expected__ = (not_expected);                                                 \
        double actual__ = (actual);                                                             \
                                                                                                \
        if (not_expected__ == actual__)                                                         \
        {                                                                                       \
            cmc_assert_state = false;                                                           \
                                                                                                \
            fprintf(stderr,                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);               \
        }                                                                                       \
                                                                                                \
    } while (0)

#define cmc_assert_not_equals_ptr(not_expected, actual)                                       \
    do                                                                                        \
    {                                                                                         \
        const char *str = #actual;                                                            \
        void *not_expected__ = (not_expected);                                                \
        void *actual__ = (actual);                                                            \
                                                                                              \
        if (not_expected__ == actual__)                                                       \
        {                                                                                     \
            cmc_assert_state = false;                                                         \
                                                                                              \
            fprintf(stderr,                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %p Actual: %p\n", \
                    __FILE__, __func__, __LINE__, str, not_expected__, actual__);             \
        }                                                                                     \
                                                                                              \
    } while (0)

#define cmc_assert_not_equals__Bool(expected, actual)                                         \
    do                                                                                        \
    {                                                                                         \
        const char *str = #actual;                                                            \
        bool expected__ = (expected);                                                         \
        bool actual__ = (actual);                                                             \
                                                                                              \
        if (expected__ == actual__)                                                           \
        {                                                                                     \
            cmc_assert_state = false;                                                         \
                                                                                              \
            fprintf(stderr,                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %d Actual: %d\n", \
                    __FILE__, __func__, __LINE__, str, expected__, actual__);                 \
        }                                                                                     \
                                                                                              \
    } while (0)

/** ---------------------------------------------------------------------------
 *
 * Greater
 *
 * ------------------------------------------------------------------------ **/

#define cmc_assert_greater_int8_t(boundary, actual)                                                               \
    do                                                                                                            \
    {                                                                                                             \
        const char *str = #actual;                                                                                \
        int8_t boundary__ = (boundary);                                                                           \
        int8_t actual__ = (actual);                                                                               \
                                                                                                                  \
        if (actual__ <= boundary__)                                                                               \
        {                                                                                                         \
            cmc_assert_state = false;                                                                             \
                                                                                                                  \
            fprintf(stderr,                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                     \
        }                                                                                                         \
                                                                                                                  \
    } while (0)

#define cmc_assert_greater_int16_t(boundary, actual)                                                                \
    do                                                                                                              \
    {                                                                                                               \
        const char *str = #actual;                                                                                  \
        int16_t boundary__ = (boundary);                                                                            \
        int16_t actual__ = (actual);                                                                                \
                                                                                                                    \
        if (actual__ <= boundary__)                                                                                 \
        {                                                                                                           \
            cmc_assert_state = false;                                                                               \
                                                                                                                    \
            fprintf(stderr,                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                       \
        }                                                                                                           \
                                                                                                                    \
    } while (0)

#define cmc_assert_greater_int32_t(boundary, actual)                                                                \
    do                                                                                                              \
    {                                                                                                               \
        const char *str = #actual;                                                                                  \
        int32_t boundary__ = (boundary);                                                                            \
        int32_t actual__ = (actual);                                                                                \
                                                                                                                    \
        if (actual__ <= boundary__)                                                                                 \
        {                                                                                                           \
            cmc_assert_state = false;                                                                               \
                                                                                                                    \
            fprintf(stderr,                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                       \
        }                                                                                                           \
                                                                                                                    \
    } while (0)

#define cmc_assert_greater_int64_t(boundary, actual)                                                                \
    do                                                                                                              \
    {                                                                                                               \
        const char *str = #actual;                                                                                  \
        int64_t boundary__ = (boundary);                                                                            \
        int64_t actual__ = (actual);                                                                                \
                                                                                                                    \
        if (actual__ <= boundary__)                                                                                 \
        {                                                                                                           \
            cmc_assert_state = false;                                                                               \
                                                                                                                    \
            fprintf(stderr,                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                       \
        }                                                                                                           \
                                                                                                                    \
    } while (0)

#define cmc_assert_greater_uint8_t(boundary, actual)                                                              \
    do                                                                                                            \
    {                                                                                                             \
        const char *str = #actual;                                                                                \
        uint8_t boundary__ = (boundary);                                                                          \
        uint8_t actual__ = (actual);                                                                              \
                                                                                                                  \
        if (actual__ <= boundary__)                                                                               \
        {                                                                                                         \
            cmc_assert_state = false;                                                                             \
                                                                                                                  \
            fprintf(stderr,                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                     \
        }                                                                                                         \
                                                                                                                  \
    } while (0)

#define cmc_assert_greater_uint16_t(boundary, actual)                                                               \
    do                                                                                                              \
    {                                                                                                               \
        const char *str = #actual;                                                                                  \
        uint16_t boundary__ = (boundary);                                                                           \
        uint16_t actual__ = (actual);                                                                               \
                                                                                                                    \
        if (actual__ <= boundary__)                                                                                 \
        {                                                                                                           \
            cmc_assert_state = false;                                                                               \
                                                                                                                    \
            fprintf(stderr,                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                       \
        }                                                                                                           \
                                                                                                                    \
    } while (0)

#define cmc_assert_greater_uint32_t(boundary, actual)                                                               \
    do                                                                                                              \
    {                                                                                                               \
        const char *str = #actual;                                                                                  \
        uint32_t boundary__ = (boundary);                                                                           \
        uint32_t actual__ = (actual);                                                                               \
                                                                                                                    \
        if (actual__ <= boundary__)                                                                                 \
        {                                                                                                           \
            cmc_assert_state = false;                                                                               \
                                                                                                                    \
            fprintf(stderr,                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                       \
        }                                                                                                           \
                                                                                                                    \
    } while (0)

#define cmc_assert_greater_uint64_t(boundary, actual)                                                               \
    do                                                                                                              \
    {                                                                                                               \
        const char *str = #actual;                                                                                  \
        uint64_t boundary__ = (boundary);                                                                           \
        uint64_t actual__ = (actual);                                                                               \
                                                                                                                    \
        if (actual__ <= boundary__)                                                                                 \
        {                                                                                                           \
            cmc_assert_state = false;                                                                               \
                                                                                                                    \
            fprintf(stderr,                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                       \
        }                                                                                                           \
                                                                                                                    \
    } while (0)

#define cmc_assert_greater_intmax_t(boundary, actual)                                                                 \
    do                                                                                                                \
    {                                                                                                                 \
        const char *str = #actual;                                                                                    \
        intmax_t boundary__ = (boundary);                                                                             \
        intmax_t actual__ = (actual);                                                                                 \
                                                                                                                      \
        if (actual__ <= boundary__)                                                                                   \
        {                                                                                                             \
            cmc_assert_state = false;                                                                                 \
                                                                                                                      \
            fprintf(stderr,                                                                                           \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                         \
        }                                                                                                             \
                                                                                                                      \
    } while (0)

#define cmc_assert_greater_uintmax_t(boundary, actual)                                                                \
    do                                                                                                                \
    {                                                                                                                 \
        const char *str = #actual;                                                                                    \
        uintmax_t boundary__ = (boundary);                                                                            \
        uintmax_t actual__ = (actual);                                                                                \
                                                                                                                      \
        if (actual__ <= boundary__)                                                                                   \
        {                                                                                                             \
            cmc_assert_state = false;                                                                                 \
                                                                                                                      \
            fprintf(stderr,                                                                                           \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                         \
        }                                                                                                             \
                                                                                                                      \
    } while (0)

#define cmc_assert_greater_size_t(boundary, actual)                                                                   \
    do                                                                                                                \
    {                                                                                                                 \
        const char *str = #actual;                                                                                    \
        size_t boundary__ = (boundary);                                                                               \
        size_t actual__ = (actual);                                                                                   \
                                                                                                                      \
        if (actual__ <= boundary__)                                                                                   \
        {                                                                                                             \
            cmc_assert_state = false;                                                                                 \
                                                                                                                      \
            fprintf(stderr,                                                                                           \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                         \
        }                                                                                                             \
                                                                                                                      \
    } while (0)

#define cmc_assert_greater_float(boundary, actual)                                                \
    do                                                                                            \
    {                                                                                             \
        const char *str = #actual;                                                                \
        float boundary__ = (boundary);                                                            \
        float actual__ = (actual);                                                                \
                                                                                                  \
        if (actual__ <= boundary__)                                                               \
        {                                                                                         \
            cmc_assert_state = false;                                                             \
                                                                                                  \
            fprintf(stderr,                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                     \
        }                                                                                         \
                                                                                                  \
    } while (0)

#define cmc_assert_greater_double(boundary, actual)                                                 \
    do                                                                                              \
    {                                                                                               \
        const char *str = #actual;                                                                  \
        double boundary__ = (boundary);                                                             \
        double actual__ = (actual);                                                                 \
                                                                                                    \
        if (actual__ <= boundary__)                                                                 \
        {                                                                                           \
            cmc_assert_state = false;                                                               \
                                                                                                    \
            fprintf(stderr,                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater: %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                       \
        }                                                                                           \
                                                                                                    \
    } while (0)

/** ---------------------------------------------------------------------------
 *
 * Greater equals
 *
 * ------------------------------------------------------------------------ **/

#define cmc_assert_greater_equals_int8_t(boundary, actual)                                                                  \
    do                                                                                                                      \
    {                                                                                                                       \
        const char *str = #actual;                                                                                          \
        int8_t boundary__ = (boundary);                                                                                     \
        int8_t actual__ = (actual);                                                                                         \
                                                                                                                            \
        if (actual__ < boundary__)                                                                                          \
        {                                                                                                                   \
            cmc_assert_state = false;                                                                                       \
                                                                                                                            \
            fprintf(stderr,                                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                               \
        }                                                                                                                   \
                                                                                                                            \
    } while (0)

#define cmc_assert_greater_equals_int16_t(boundary, actual)                                                                   \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int16_t boundary__ = (boundary);                                                                                      \
        int16_t actual__ = (actual);                                                                                          \
                                                                                                                              \
        if (actual__ < boundary__)                                                                                            \
        {                                                                                                                     \
            cmc_assert_state = false;                                                                                         \
                                                                                                                              \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                 \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_greater_equals_int32_t(boundary, actual)                                                                   \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int32_t boundary__ = (boundary);                                                                                      \
        int32_t actual__ = (actual);                                                                                          \
                                                                                                                              \
        if (actual__ < boundary__)                                                                                            \
        {                                                                                                                     \
            cmc_assert_state = false;                                                                                         \
                                                                                                                              \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                 \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_greater_equals_int64_t(boundary, actual)                                                                   \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int64_t boundary__ = (boundary);                                                                                      \
        int64_t actual__ = (actual);                                                                                          \
                                                                                                                              \
        if (actual__ < boundary__)                                                                                            \
        {                                                                                                                     \
            cmc_assert_state = false;                                                                                         \
                                                                                                                              \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                 \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_greater_equals_uint8_t(boundary, actual)                                                                 \
    do                                                                                                                      \
    {                                                                                                                       \
        const char *str = #actual;                                                                                          \
        uint8_t boundary__ = (boundary);                                                                                    \
        uint8_t actual__ = (actual);                                                                                        \
                                                                                                                            \
        if (actual__ < boundary__)                                                                                          \
        {                                                                                                                   \
            cmc_assert_state = false;                                                                                       \
                                                                                                                            \
            fprintf(stderr,                                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                               \
        }                                                                                                                   \
                                                                                                                            \
    } while (0)

#define cmc_assert_greater_equals_uint16_t(boundary, actual)                                                                  \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint16_t boundary__ = (boundary);                                                                                     \
        uint16_t actual__ = (actual);                                                                                         \
                                                                                                                              \
        if (actual__ < boundary__)                                                                                            \
        {                                                                                                                     \
            cmc_assert_state = false;                                                                                         \
                                                                                                                              \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                 \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_greater_equals_uint32_t(boundary, actual)                                                                  \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint32_t boundary__ = (boundary);                                                                                     \
        uint32_t actual__ = (actual);                                                                                         \
                                                                                                                              \
        if (actual__ < boundary__)                                                                                            \
        {                                                                                                                     \
            cmc_assert_state = false;                                                                                         \
                                                                                                                              \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                 \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_greater_equals_uint64_t(boundary, actual)                                                                  \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint64_t boundary__ = (boundary);                                                                                     \
        uint64_t actual__ = (actual);                                                                                         \
                                                                                                                              \
        if (actual__ < boundary__)                                                                                            \
        {                                                                                                                     \
            cmc_assert_state = false;                                                                                         \
                                                                                                                              \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                 \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_greater_equals_intmax_t(boundary, actual)                                                                    \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        intmax_t boundary__ = (boundary);                                                                                       \
        intmax_t actual__ = (actual);                                                                                           \
                                                                                                                                \
        if (actual__ < boundary__)                                                                                              \
        {                                                                                                                       \
            cmc_assert_state = false;                                                                                           \
                                                                                                                                \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                   \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_greater_equals_uintmax_t(boundary, actual)                                                                   \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        uintmax_t boundary__ = (boundary);                                                                                      \
        uintmax_t actual__ = (actual);                                                                                          \
                                                                                                                                \
        if (actual__ < boundary__)                                                                                              \
        {                                                                                                                       \
            cmc_assert_state = false;                                                                                           \
                                                                                                                                \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                   \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_greater_equals_size_t(boundary, actual)                                                                      \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        size_t boundary__ = (boundary);                                                                                         \
        size_t actual__ = (actual);                                                                                             \
                                                                                                                                \
        if (actual__ < boundary__)                                                                                              \
        {                                                                                                                       \
            cmc_assert_state = false;                                                                                           \
                                                                                                                                \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                   \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_greater_equals_float(boundary, actual)                                                   \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        float boundary__ = (boundary);                                                                      \
        float actual__ = (actual);                                                                          \
                                                                                                            \
        if (actual__ < boundary__)                                                                          \
        {                                                                                                   \
            cmc_assert_state = false;                                                                       \
                                                                                                            \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                               \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_greater_equals_double(boundary, actual)                                                    \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        double boundary__ = (boundary);                                                                       \
        double actual__ = (actual);                                                                           \
                                                                                                              \
        if (actual__ < boundary__)                                                                            \
        {                                                                                                     \
            cmc_assert_state = false;                                                                         \
                                                                                                              \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater or Equals: %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                 \
        }                                                                                                     \
                                                                                                              \
    } while (0)

/** ---------------------------------------------------------------------------
 *
 * Lesser
 *
 * ------------------------------------------------------------------------ **/

#define cmc_assert_lesser_int8_t(boundary, actual)                                                               \
    do                                                                                                           \
    {                                                                                                            \
        const char *str = #actual;                                                                               \
        int8_t boundary__ = (boundary);                                                                          \
        int8_t actual__ = (actual);                                                                              \
                                                                                                                 \
        if (actual__ >= boundary__)                                                                              \
        {                                                                                                        \
            cmc_assert_state = false;                                                                            \
                                                                                                                 \
            fprintf(stderr,                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                    \
        }                                                                                                        \
                                                                                                                 \
    } while (0)

#define cmc_assert_lesser_int16_t(boundary, actual)                                                                \
    do                                                                                                             \
    {                                                                                                              \
        const char *str = #actual;                                                                                 \
        int16_t boundary__ = (boundary);                                                                           \
        int16_t actual__ = (actual);                                                                               \
                                                                                                                   \
        if (actual__ >= boundary__)                                                                                \
        {                                                                                                          \
            cmc_assert_state = false;                                                                              \
                                                                                                                   \
            fprintf(stderr,                                                                                        \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                      \
        }                                                                                                          \
                                                                                                                   \
    } while (0)

#define cmc_assert_lesser_int32_t(boundary, actual)                                                                \
    do                                                                                                             \
    {                                                                                                              \
        const char *str = #actual;                                                                                 \
        int32_t boundary__ = (boundary);                                                                           \
        int32_t actual__ = (actual);                                                                               \
                                                                                                                   \
        if (actual__ >= boundary__)                                                                                \
        {                                                                                                          \
            cmc_assert_state = false;                                                                              \
                                                                                                                   \
            fprintf(stderr,                                                                                        \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                      \
        }                                                                                                          \
                                                                                                                   \
    } while (0)

#define cmc_assert_lesser_int64_t(boundary, actual)                                                                \
    do                                                                                                             \
    {                                                                                                              \
        const char *str = #actual;                                                                                 \
        int64_t boundary__ = (boundary);                                                                           \
        int64_t actual__ = (actual);                                                                               \
                                                                                                                   \
        if (actual__ >= boundary__)                                                                                \
        {                                                                                                          \
            cmc_assert_state = false;                                                                              \
                                                                                                                   \
            fprintf(stderr,                                                                                        \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                      \
        }                                                                                                          \
                                                                                                                   \
    } while (0)

#define cmc_assert_lesser_uint8_t(boundary, actual)                                                              \
    do                                                                                                           \
    {                                                                                                            \
        const char *str = #actual;                                                                               \
        uint8_t boundary__ = (boundary);                                                                         \
        uint8_t actual__ = (actual);                                                                             \
                                                                                                                 \
        if (actual__ >= boundary__)                                                                              \
        {                                                                                                        \
            cmc_assert_state = false;                                                                            \
                                                                                                                 \
            fprintf(stderr,                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                    \
        }                                                                                                        \
                                                                                                                 \
    } while (0)

#define cmc_assert_lesser_uint16_t(boundary, actual)                                                               \
    do                                                                                                             \
    {                                                                                                              \
        const char *str = #actual;                                                                                 \
        uint16_t boundary__ = (boundary);                                                                          \
        uint16_t actual__ = (actual);                                                                              \
                                                                                                                   \
        if (actual__ >= boundary__)                                                                                \
        {                                                                                                          \
            cmc_assert_state = false;                                                                              \
                                                                                                                   \
            fprintf(stderr,                                                                                        \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                      \
        }                                                                                                          \
                                                                                                                   \
    } while (0)

#define cmc_assert_lesser_uint32_t(boundary, actual)                                                               \
    do                                                                                                             \
    {                                                                                                              \
        const char *str = #actual;                                                                                 \
        uint32_t boundary__ = (boundary);                                                                          \
        uint32_t actual__ = (actual);                                                                              \
                                                                                                                   \
        if (actual__ >= boundary__)                                                                                \
        {                                                                                                          \
            cmc_assert_state = false;                                                                              \
                                                                                                                   \
            fprintf(stderr,                                                                                        \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                      \
        }                                                                                                          \
                                                                                                                   \
    } while (0)

#define cmc_assert_lesser_uint64_t(boundary, actual)                                                               \
    do                                                                                                             \
    {                                                                                                              \
        const char *str = #actual;                                                                                 \
        uint64_t boundary__ = (boundary);                                                                          \
        uint64_t actual__ = (actual);                                                                              \
                                                                                                                   \
        if (actual__ >= boundary__)                                                                                \
        {                                                                                                          \
            cmc_assert_state = false;                                                                              \
                                                                                                                   \
            fprintf(stderr,                                                                                        \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                      \
        }                                                                                                          \
                                                                                                                   \
    } while (0)

#define cmc_assert_lesser_intmax_t(boundary, actual)                                                                 \
    do                                                                                                               \
    {                                                                                                                \
        const char *str = #actual;                                                                                   \
        intmax_t boundary__ = (boundary);                                                                            \
        intmax_t actual__ = (actual);                                                                                \
                                                                                                                     \
        if (actual__ >= boundary__)                                                                                  \
        {                                                                                                            \
            cmc_assert_state = false;                                                                                \
                                                                                                                     \
            fprintf(stderr,                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                        \
        }                                                                                                            \
                                                                                                                     \
    } while (0)

#define cmc_assert_lesser_uintmax_t(boundary, actual)                                                                \
    do                                                                                                               \
    {                                                                                                                \
        const char *str = #actual;                                                                                   \
        uintmax_t boundary__ = (boundary);                                                                           \
        uintmax_t actual__ = (actual);                                                                               \
                                                                                                                     \
        if (actual__ >= boundary__)                                                                                  \
        {                                                                                                            \
            cmc_assert_state = false;                                                                                \
                                                                                                                     \
            fprintf(stderr,                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                        \
        }                                                                                                            \
                                                                                                                     \
    } while (0)

#define cmc_assert_lesser_size_t(boundary, actual)                                                                   \
    do                                                                                                               \
    {                                                                                                                \
        const char *str = #actual;                                                                                   \
        size_t boundary__ = (boundary);                                                                              \
        size_t actual__ = (actual);                                                                                  \
                                                                                                                     \
        if (actual__ >= boundary__)                                                                                  \
        {                                                                                                            \
            cmc_assert_state = false;                                                                                \
                                                                                                                     \
            fprintf(stderr,                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                        \
        }                                                                                                            \
                                                                                                                     \
    } while (0)

#define cmc_assert_lesser_float(boundary, actual)                                                \
    do                                                                                           \
    {                                                                                            \
        const char *str = #actual;                                                               \
        float boundary__ = (boundary);                                                           \
        float actual__ = (actual);                                                               \
                                                                                                 \
        if (actual__ >= boundary__)                                                              \
        {                                                                                        \
            cmc_assert_state = false;                                                            \
                                                                                                 \
            fprintf(stderr,                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                    \
        }                                                                                        \
                                                                                                 \
    } while (0)

#define cmc_assert_lesser_double(boundary, actual)                                                 \
    do                                                                                             \
    {                                                                                              \
        const char *str = #actual;                                                                 \
        double boundary__ = (boundary);                                                            \
        double actual__ = (actual);                                                                \
                                                                                                   \
        if (actual__ >= boundary__)                                                                \
        {                                                                                          \
            cmc_assert_state = false;                                                              \
                                                                                                   \
            fprintf(stderr,                                                                        \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser: %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                      \
        }                                                                                          \
                                                                                                   \
    } while (0)

/** ---------------------------------------------------------------------------
 *
 * Lesser equals
 *
 * ------------------------------------------------------------------------ **/

#define cmc_assert_lesser_equals_int8_t(boundary, actual)                                                                  \
    do                                                                                                                     \
    {                                                                                                                      \
        const char *str = #actual;                                                                                         \
        int8_t boundary__ = (boundary);                                                                                    \
        int8_t actual__ = (actual);                                                                                        \
                                                                                                                           \
        if (actual__ > boundary__)                                                                                         \
        {                                                                                                                  \
            cmc_assert_state = false;                                                                                      \
                                                                                                                           \
            fprintf(stderr,                                                                                                \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                              \
        }                                                                                                                  \
                                                                                                                           \
    } while (0)

#define cmc_assert_lesser_equals_int16_t(boundary, actual)                                                                   \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        int16_t boundary__ = (boundary);                                                                                     \
        int16_t actual__ = (actual);                                                                                         \
                                                                                                                             \
        if (actual__ > boundary__)                                                                                           \
        {                                                                                                                    \
            cmc_assert_state = false;                                                                                        \
                                                                                                                             \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_lesser_equals_int32_t(boundary, actual)                                                                   \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        int32_t boundary__ = (boundary);                                                                                     \
        int32_t actual__ = (actual);                                                                                         \
                                                                                                                             \
        if (actual__ > boundary__)                                                                                           \
        {                                                                                                                    \
            cmc_assert_state = false;                                                                                        \
                                                                                                                             \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_lesser_equals_int64_t(boundary, actual)                                                                   \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        int64_t boundary__ = (boundary);                                                                                     \
        int64_t actual__ = (actual);                                                                                         \
                                                                                                                             \
        if (actual__ > boundary__)                                                                                           \
        {                                                                                                                    \
            cmc_assert_state = false;                                                                                        \
                                                                                                                             \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_lesser_equals_uint8_t(boundary, actual)                                                                 \
    do                                                                                                                     \
    {                                                                                                                      \
        const char *str = #actual;                                                                                         \
        uint8_t boundary__ = (boundary);                                                                                   \
        uint8_t actual__ = (actual);                                                                                       \
                                                                                                                           \
        if (actual__ > boundary__)                                                                                         \
        {                                                                                                                  \
            cmc_assert_state = false;                                                                                      \
                                                                                                                           \
            fprintf(stderr,                                                                                                \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                              \
        }                                                                                                                  \
                                                                                                                           \
    } while (0)

#define cmc_assert_lesser_equals_uint16_t(boundary, actual)                                                                  \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        uint16_t boundary__ = (boundary);                                                                                    \
        uint16_t actual__ = (actual);                                                                                        \
                                                                                                                             \
        if (actual__ > boundary__)                                                                                           \
        {                                                                                                                    \
            cmc_assert_state = false;                                                                                        \
                                                                                                                             \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_lesser_equals_uint32_t(boundary, actual)                                                                  \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        uint32_t boundary__ = (boundary);                                                                                    \
        uint32_t actual__ = (actual);                                                                                        \
                                                                                                                             \
        if (actual__ > boundary__)                                                                                           \
        {                                                                                                                    \
            cmc_assert_state = false;                                                                                        \
                                                                                                                             \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_lesser_equals_uint64_t(boundary, actual)                                                                  \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        uint64_t boundary__ = (boundary);                                                                                    \
        uint64_t actual__ = (actual);                                                                                        \
                                                                                                                             \
        if (actual__ > boundary__)                                                                                           \
        {                                                                                                                    \
            cmc_assert_state = false;                                                                                        \
                                                                                                                             \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_lesser_equals_intmax_t(boundary, actual)                                                                    \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        intmax_t boundary__ = (boundary);                                                                                      \
        intmax_t actual__ = (actual);                                                                                          \
                                                                                                                               \
        if (actual__ > boundary__)                                                                                             \
        {                                                                                                                      \
            cmc_assert_state = false;                                                                                          \
                                                                                                                               \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                  \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_lesser_equals_uintmax_t(boundary, actual)                                                                   \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        uintmax_t boundary__ = (boundary);                                                                                     \
        uintmax_t actual__ = (actual);                                                                                         \
                                                                                                                               \
        if (actual__ > boundary__)                                                                                             \
        {                                                                                                                      \
            cmc_assert_state = false;                                                                                          \
                                                                                                                               \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                  \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_lesser_equals_size_t(boundary, actual)                                                                      \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        size_t boundary__ = (boundary);                                                                                        \
        size_t actual__ = (actual);                                                                                            \
                                                                                                                               \
        if (actual__ > boundary__)                                                                                             \
        {                                                                                                                      \
            cmc_assert_state = false;                                                                                          \
                                                                                                                               \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                                  \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_lesser_equals_float(boundary, actual)                                                   \
    do                                                                                                     \
    {                                                                                                      \
        const char *str = #actual;                                                                         \
        float boundary__ = (boundary);                                                                     \
        float actual__ = (actual);                                                                         \
                                                                                                           \
        if (actual__ > boundary__)                                                                         \
        {                                                                                                  \
            cmc_assert_state = false;                                                                      \
                                                                                                           \
            fprintf(stderr,                                                                                \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                              \
        }                                                                                                  \
                                                                                                           \
    } while (0)

#define cmc_assert_lesser_equals_double(boundary, actual)                                                    \
    do                                                                                                       \
    {                                                                                                        \
        const char *str = #actual;                                                                           \
        double boundary__ = (boundary);                                                                      \
        double actual__ = (actual);                                                                          \
                                                                                                             \
        if (actual__ > boundary__)                                                                           \
        {                                                                                                    \
            cmc_assert_state = false;                                                                        \
                                                                                                             \
            fprintf(stderr,                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser or Equals: %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, boundary__, actual__);                                \
        }                                                                                                    \
                                                                                                             \
    } while (0)

/** ---------------------------------------------------------------------------
 *
 * In range
 *
 * ------------------------------------------------------------------------ **/

#define cmc_assert_in_range_int8_t(lower_bound, upper_bound, actual)                                                          \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int8_t lower_bound__ = (lower_bound);                                                                                 \
        int8_t upper_bound__ = (upper_bound);                                                                                 \
        int8_t actual__ = (actual);                                                                                           \
                                                                                                                              \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                     \
        {                                                                                                                     \
            cmc_assert_state = false;                                                                                         \
                                                                                                                              \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRId8 ", %" PRId8 "] Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                               \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_in_range_int16_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        int16_t lower_bound__ = (lower_bound);                                                                                   \
        int16_t upper_bound__ = (upper_bound);                                                                                   \
        int16_t actual__ = (actual);                                                                                             \
                                                                                                                                 \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                        \
        {                                                                                                                        \
            cmc_assert_state = false;                                                                                            \
                                                                                                                                 \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRId16 ", %" PRId16 "] Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                  \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_int32_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        int32_t lower_bound__ = (lower_bound);                                                                                   \
        int32_t upper_bound__ = (upper_bound);                                                                                   \
        int32_t actual__ = (actual);                                                                                             \
                                                                                                                                 \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                        \
        {                                                                                                                        \
            cmc_assert_state = false;                                                                                            \
                                                                                                                                 \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRId32 ", %" PRId32 "] Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                  \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_int64_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        int64_t lower_bound__ = (lower_bound);                                                                                   \
        int64_t upper_bound__ = (upper_bound);                                                                                   \
        int64_t actual__ = (actual);                                                                                             \
                                                                                                                                 \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                        \
        {                                                                                                                        \
            cmc_assert_state = false;                                                                                            \
                                                                                                                                 \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRId32 ", %" PRId32 "] Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                  \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_uint8_t(lower_bound, upper_bound, actual)                                                         \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint8_t lower_bound__ = (lower_bound);                                                                                \
        uint8_t upper_bound__ = (upper_bound);                                                                                \
        uint8_t actual__ = (actual);                                                                                          \
                                                                                                                              \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                     \
        {                                                                                                                     \
            cmc_assert_state = false;                                                                                         \
                                                                                                                              \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIu8 ", %" PRIu8 "] Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                               \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_in_range_uint16_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        uint16_t lower_bound__ = (lower_bound);                                                                                  \
        uint16_t upper_bound__ = (upper_bound);                                                                                  \
        uint16_t actual__ = (actual);                                                                                            \
                                                                                                                                 \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                        \
        {                                                                                                                        \
            cmc_assert_state = false;                                                                                            \
                                                                                                                                 \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIu16 ", %" PRIu16 "] Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                  \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_uint32_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        uint32_t lower_bound__ = (lower_bound);                                                                                  \
        uint32_t upper_bound__ = (upper_bound);                                                                                  \
        uint32_t actual__ = (actual);                                                                                            \
                                                                                                                                 \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                        \
        {                                                                                                                        \
            cmc_assert_state = false;                                                                                            \
                                                                                                                                 \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIu32 ", %" PRIu32 "] Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                  \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_uint64_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        uint64_t lower_bound__ = (lower_bound);                                                                                  \
        uint64_t upper_bound__ = (upper_bound);                                                                                  \
        uint64_t actual__ = (actual);                                                                                            \
                                                                                                                                 \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                        \
        {                                                                                                                        \
            cmc_assert_state = false;                                                                                            \
                                                                                                                                 \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIu64 ", %" PRIu64 "] Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                  \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_intmax_t(lower_bound, upper_bound, actual)                                                              \
    do                                                                                                                              \
    {                                                                                                                               \
        const char *str = #actual;                                                                                                  \
        intmax_t lower_bound__ = (lower_bound);                                                                                     \
        intmax_t upper_bound__ = (upper_bound);                                                                                     \
        intmax_t actual__ = (actual);                                                                                               \
                                                                                                                                    \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                           \
        {                                                                                                                           \
            cmc_assert_state = false;                                                                                               \
                                                                                                                                    \
            fprintf(stderr,                                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIdMAX ", %" PRIdMAX "] Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                     \
        }                                                                                                                           \
                                                                                                                                    \
    } while (0)

#define cmc_assert_in_range_uintmax_t(lower_bound, upper_bound, actual)                                                             \
    do                                                                                                                              \
    {                                                                                                                               \
        const char *str = #actual;                                                                                                  \
        uintmax_t lower_bound__ = (lower_bound);                                                                                    \
        uintmax_t upper_bound__ = (upper_bound);                                                                                    \
        uintmax_t actual__ = (actual);                                                                                              \
                                                                                                                                    \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                           \
        {                                                                                                                           \
            cmc_assert_state = false;                                                                                               \
                                                                                                                                    \
            fprintf(stderr,                                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIuMAX ", %" PRIuMAX "] Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                     \
        }                                                                                                                           \
                                                                                                                                    \
    } while (0)

#define cmc_assert_in_range_size_t(lower_bound, upper_bound, actual)                                                                \
    do                                                                                                                              \
    {                                                                                                                               \
        const char *str = #actual;                                                                                                  \
        size_t lower_bound__ = (lower_bound);                                                                                       \
        size_t upper_bound__ = (upper_bound);                                                                                       \
        size_t actual__ = (actual);                                                                                                 \
                                                                                                                                    \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                                           \
        {                                                                                                                           \
            cmc_assert_state = false;                                                                                               \
                                                                                                                                    \
            fprintf(stderr,                                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIuMAX ", %" PRIuMAX "] Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                     \
        }                                                                                                                           \
                                                                                                                                    \
    } while (0)

#define cmc_assert_in_range_float(lower_bound, upper_bound, actual)                                   \
    do                                                                                                \
    {                                                                                                 \
        const char *str = #actual;                                                                    \
        float lower_bound__ = (lower_bound);                                                          \
        float upper_bound__ = (upper_bound);                                                          \
        float actual__ = (actual);                                                                    \
                                                                                                      \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                             \
        {                                                                                             \
            cmc_assert_state = false;                                                                 \
                                                                                                      \
            fprintf(stderr,                                                                           \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%f, %f] Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);       \
        }                                                                                             \
                                                                                                      \
    } while (0)

#define cmc_assert_in_range_double(lower_bound, upper_bound, actual)                                     \
    do                                                                                                   \
    {                                                                                                    \
        const char *str = #actual;                                                                       \
        double lower_bound__ = (lower_bound);                                                            \
        double upper_bound__ = (upper_bound);                                                            \
        double actual__ = (actual);                                                                      \
                                                                                                         \
        if ((actual__) < (lower_bound__) || (actual__) > (upper_bound__))                                \
        {                                                                                                \
            cmc_assert_state = false;                                                                    \
                                                                                                         \
            fprintf(stderr,                                                                              \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%lf, %lf] Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);          \
        }                                                                                                \
                                                                                                         \
    } while (0)

/** ---------------------------------------------------------------------------
 *
 * Not in range
 *
 * ------------------------------------------------------------------------ **/

#define cmc_assert_not_in_range_int8_t(lower_bound, upper_bound, actual)                                                          \
    do                                                                                                                            \
    {                                                                                                                             \
        const char *str = #actual;                                                                                                \
        int8_t lower_bound__ = (lower_bound);                                                                                     \
        int8_t upper_bound__ = (upper_bound);                                                                                     \
        int8_t actual__ = (actual);                                                                                               \
                                                                                                                                  \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                       \
        {                                                                                                                         \
            cmc_assert_state = false;                                                                                             \
                                                                                                                                  \
            fprintf(stderr,                                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRId8 ", %" PRId8 "] Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                   \
        }                                                                                                                         \
                                                                                                                                  \
    } while (0)

#define cmc_assert_not_in_range_int16_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        int16_t lower_bound__ = (lower_bound);                                                                                       \
        int16_t upper_bound__ = (upper_bound);                                                                                       \
        int16_t actual__ = (actual);                                                                                                 \
                                                                                                                                     \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                          \
        {                                                                                                                            \
            cmc_assert_state = false;                                                                                                \
                                                                                                                                     \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRId16 ", %" PRId16 "] Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                      \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_int32_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        int32_t lower_bound__ = (lower_bound);                                                                                       \
        int32_t upper_bound__ = (upper_bound);                                                                                       \
        int32_t actual__ = (actual);                                                                                                 \
                                                                                                                                     \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                          \
        {                                                                                                                            \
            cmc_assert_state = false;                                                                                                \
                                                                                                                                     \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRId32 ", %" PRId32 "] Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                      \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_int64_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        int64_t lower_bound__ = (lower_bound);                                                                                       \
        int64_t upper_bound__ = (upper_bound);                                                                                       \
        int64_t actual__ = (actual);                                                                                                 \
                                                                                                                                     \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                          \
        {                                                                                                                            \
            cmc_assert_state = false;                                                                                                \
                                                                                                                                     \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRId32 ", %" PRId32 "] Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                      \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_uint8_t(lower_bound, upper_bound, actual)                                                         \
    do                                                                                                                            \
    {                                                                                                                             \
        const char *str = #actual;                                                                                                \
        uint8_t lower_bound__ = (lower_bound);                                                                                    \
        uint8_t upper_bound__ = (upper_bound);                                                                                    \
        uint8_t actual__ = (actual);                                                                                              \
                                                                                                                                  \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                       \
        {                                                                                                                         \
            cmc_assert_state = false;                                                                                             \
                                                                                                                                  \
            fprintf(stderr,                                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIu8 ", %" PRIu8 "] Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                   \
        }                                                                                                                         \
                                                                                                                                  \
    } while (0)

#define cmc_assert_not_in_range_uint16_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        uint16_t lower_bound__ = (lower_bound);                                                                                      \
        uint16_t upper_bound__ = (upper_bound);                                                                                      \
        uint16_t actual__ = (actual);                                                                                                \
                                                                                                                                     \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                          \
        {                                                                                                                            \
            cmc_assert_state = false;                                                                                                \
                                                                                                                                     \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIu16 ", %" PRIu16 "] Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                      \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_uint32_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        uint32_t lower_bound__ = (lower_bound);                                                                                      \
        uint32_t upper_bound__ = (upper_bound);                                                                                      \
        uint32_t actual__ = (actual);                                                                                                \
                                                                                                                                     \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                          \
        {                                                                                                                            \
            cmc_assert_state = false;                                                                                                \
                                                                                                                                     \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIu32 ", %" PRIu32 "] Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                      \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_uint64_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        uint64_t lower_bound__ = (lower_bound);                                                                                      \
        uint64_t upper_bound__ = (upper_bound);                                                                                      \
        uint64_t actual__ = (actual);                                                                                                \
                                                                                                                                     \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                          \
        {                                                                                                                            \
            cmc_assert_state = false;                                                                                                \
                                                                                                                                     \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIu64 ", %" PRIu64 "] Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                      \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_intmax_t(lower_bound, upper_bound, actual)                                                              \
    do                                                                                                                                  \
    {                                                                                                                                   \
        const char *str = #actual;                                                                                                      \
        intmax_t lower_bound__ = (lower_bound);                                                                                         \
        intmax_t upper_bound__ = (upper_bound);                                                                                         \
        intmax_t actual__ = (actual);                                                                                                   \
                                                                                                                                        \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                             \
        {                                                                                                                               \
            cmc_assert_state = false;                                                                                                   \
                                                                                                                                        \
            fprintf(stderr,                                                                                                             \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIdMAX ", %" PRIdMAX "] Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                         \
        }                                                                                                                               \
                                                                                                                                        \
    } while (0)

#define cmc_assert_not_in_range_uintmax_t(lower_bound, upper_bound, actual)                                                             \
    do                                                                                                                                  \
    {                                                                                                                                   \
        const char *str = #actual;                                                                                                      \
        uintmax_t lower_bound__ = (lower_bound);                                                                                        \
        uintmax_t upper_bound__ = (upper_bound);                                                                                        \
        uintmax_t actual__ = (actual);                                                                                                  \
                                                                                                                                        \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                             \
        {                                                                                                                               \
            cmc_assert_state = false;                                                                                                   \
                                                                                                                                        \
            fprintf(stderr,                                                                                                             \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIuMAX ", %" PRIuMAX "] Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                         \
        }                                                                                                                               \
                                                                                                                                        \
    } while (0)

#define cmc_assert_not_in_range_size_t(lower_bound, upper_bound, actual)                                                                \
    do                                                                                                                                  \
    {                                                                                                                                   \
        const char *str = #actual;                                                                                                      \
        size_t lower_bound__ = (lower_bound);                                                                                           \
        size_t upper_bound__ = (upper_bound);                                                                                           \
        size_t actual__ = (actual);                                                                                                     \
                                                                                                                                        \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                                             \
        {                                                                                                                               \
            cmc_assert_state = false;                                                                                                   \
                                                                                                                                        \
            fprintf(stderr,                                                                                                             \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIuMAX ", %" PRIuMAX "] Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);                                         \
        }                                                                                                                               \
                                                                                                                                        \
    } while (0)

#define cmc_assert_not_in_range_float(lower_bound, upper_bound, actual)                                   \
    do                                                                                                    \
    {                                                                                                     \
        const char *str = #actual;                                                                        \
        float lower_bound__ = (lower_bound);                                                              \
        float upper_bound__ = (upper_bound);                                                              \
        float actual__ = (actual);                                                                        \
                                                                                                          \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                               \
        {                                                                                                 \
            cmc_assert_state = false;                                                                     \
                                                                                                          \
            fprintf(stderr,                                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%f, %f] Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);           \
        }                                                                                                 \
                                                                                                          \
    } while (0)

#define cmc_assert_not_in_range_double(lower_bound, upper_bound, actual)                                     \
    do                                                                                                       \
    {                                                                                                        \
        const char *str = #actual;                                                                           \
        double lower_bound__ = (lower_bound);                                                                \
        double upper_bound__ = (upper_bound);                                                                \
        double actual__ = (actual);                                                                          \
                                                                                                             \
        if ((actual__) >= (lower_bound__) && (actual__) <= (upper_bound__))                                  \
        {                                                                                                    \
            cmc_assert_state = false;                                                                        \
                                                                                                             \
            fprintf(stderr,                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%lf, %lf] Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound__, upper_bound__, actual__);              \
        }                                                                                                    \
                                                                                                             \
    } while (0)

#endif /* CMC_ASSERT_H */
