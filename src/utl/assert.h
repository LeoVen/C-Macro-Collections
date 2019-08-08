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
 * There are 4 main macros:
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
 */

#ifndef CMC_ASSERT_H
#define CMC_ASSERT_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CMC_ASSERT_GLUE_(dtype, assertion) cmc_assert_##assertion##_##dtype
#define CMC_ASSERT_GLUE(dtype, assertion) CMC_ASSERT_GLUE_(dtype, assertion)

/**
 * expression : An expression that is expected to be evaluated to true.
 */
#define cmc_assert(expression)                                   \
    do                                                           \
    {                                                            \
        const char *str = #expression;                           \
        if (!(expression))                                       \
        {                                                        \
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

#define cmc_assert_equals_int8_t(expected, actual)                                                        \
    do                                                                                                    \
    {                                                                                                     \
        const char *str = #actual;                                                                        \
        int8_t expected_ = (expected);                                                                    \
        int8_t actual_ = (actual);                                                                        \
                                                                                                          \
        if ((expected_) != (actual_))                                                                     \
        {                                                                                                 \
            fprintf(stderr,                                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                               \
        }                                                                                                 \
                                                                                                          \
    } while (0)

#define cmc_assert_equals_int16_t(expected, actual)                                                         \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        int16_t expected_ = (expected);                                                                     \
        int16_t actual_ = (actual);                                                                         \
                                                                                                            \
        if ((expected_) != (actual_))                                                                       \
        {                                                                                                   \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                 \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_int32_t(expected, actual)                                                         \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        int32_t expected_ = (expected);                                                                     \
        int32_t actual_ = (actual);                                                                         \
                                                                                                            \
        if ((expected_) != (actual_))                                                                       \
        {                                                                                                   \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                 \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_int64_t(expected, actual)                                                         \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        int64_t expected_ = (expected);                                                                     \
        int64_t actual_ = (actual);                                                                         \
                                                                                                            \
        if ((expected_) != (actual_))                                                                       \
        {                                                                                                   \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                 \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_uint8_t(expected, actual)                                                       \
    do                                                                                                    \
    {                                                                                                     \
        const char *str = #actual;                                                                        \
        uint8_t expected_ = (expected);                                                                   \
        uint8_t actual_ = (actual);                                                                       \
                                                                                                          \
        if ((expected_) != (actual_))                                                                     \
        {                                                                                                 \
            fprintf(stderr,                                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                               \
        }                                                                                                 \
                                                                                                          \
    } while (0)

#define cmc_assert_equals_uint16_t(expected, actual)                                                        \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        uint16_t expected_ = (expected);                                                                    \
        uint16_t actual_ = (actual);                                                                        \
                                                                                                            \
        if ((expected_) != (actual_))                                                                       \
        {                                                                                                   \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                 \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_uint32_t(expected, actual)                                                        \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        uint32_t expected_ = (expected);                                                                    \
        uint32_t actual_ = (actual);                                                                        \
                                                                                                            \
        if ((expected_) != (actual_))                                                                       \
        {                                                                                                   \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                 \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_uint64_t(expected, actual)                                                        \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        uint64_t expected_ = (expected);                                                                    \
        uint64_t actual_ = (actual);                                                                        \
                                                                                                            \
        if ((expected_) != (actual_))                                                                       \
        {                                                                                                   \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                 \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_equals_intmax_t(expected, actual)                                                          \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        intmax_t expected_ = (expected);                                                                      \
        intmax_t actual_ = (actual);                                                                          \
                                                                                                              \
        if ((expected_) != (actual_))                                                                         \
        {                                                                                                     \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                   \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_equals_uintmax_t(expected, actual)                                                         \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        uintmax_t expected_ = (expected);                                                                     \
        uintmax_t actual_ = (actual);                                                                         \
                                                                                                              \
        if ((expected_) != (actual_))                                                                         \
        {                                                                                                     \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                   \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_equals_size_t(expected, actual)                                                            \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        size_t expected_ = (expected);                                                                        \
        size_t actual_ = (actual);                                                                            \
                                                                                                              \
        if ((expected_) != (actual_))                                                                         \
        {                                                                                                     \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                                   \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_equals_float(expected, actual)                                         \
    do                                                                                    \
    {                                                                                     \
        const char *str = #actual;                                                        \
        float expected_ = (expected);                                                     \
        float actual_ = (actual);                                                         \
                                                                                          \
        if ((expected_) != (actual_))                                                     \
        {                                                                                 \
            fprintf(stderr,                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);               \
        }                                                                                 \
                                                                                          \
    } while (0)

#define cmc_assert_equals_double(expected, actual)                                          \
    do                                                                                      \
    {                                                                                       \
        const char *str = #actual;                                                          \
        double expected_ = (expected);                                                      \
        double actual_ = (actual);                                                          \
                                                                                            \
        if ((expected_) != (actual_))                                                       \
        {                                                                                   \
            fprintf(stderr,                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                 \
        }                                                                                   \
                                                                                            \
    } while (0)

#define cmc_assert_equals_ptr(expected, actual)                                           \
    do                                                                                    \
    {                                                                                     \
        const char *str = #actual;                                                        \
        void *expected_ = (expected);                                                     \
        void *actual_ = (actual);                                                         \
                                                                                          \
        if ((expected_) != (actual_))                                                     \
        {                                                                                 \
            fprintf(stderr,                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %p Actual: %p\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);               \
        }                                                                                 \
                                                                                          \
    } while (0)

#define cmc_assert_equals__Bool(expected, actual)                                          \
    do                                                                                    \
    {                                                                                     \
        const char *str = #actual;                                                        \
        bool expected_ = (expected);                                                      \
        bool actual_ = (actual);                                                          \
                                                                                          \
        if ((expected_) != (actual_))                                                     \
        {                                                                                 \
            fprintf(stderr,                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected: %d Actual: %d\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);               \
        }                                                                                 \
                                                                                          \
    } while (0)

#define cmc_assert_not_equals_int8_t(not_expected, actual)                                                    \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        int8_t not_expected_ = (not_expected);                                                                \
        int8_t actual_ = (actual);                                                                            \
                                                                                                              \
        if ((not_expected_) == (actual_))                                                                     \
        {                                                                                                     \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                               \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_not_equals_int16_t(not_expected, actual)                                                     \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        int16_t not_expected_ = (not_expected);                                                                 \
        int16_t actual_ = (actual);                                                                             \
                                                                                                                \
        if ((not_expected_) == (actual_))                                                                       \
        {                                                                                                       \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                 \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_int32_t(not_expected, actual)                                                     \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        int32_t not_expected_ = (not_expected);                                                                 \
        int32_t actual_ = (actual);                                                                             \
                                                                                                                \
        if ((not_expected_) == (actual_))                                                                       \
        {                                                                                                       \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                 \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_int64_t(not_expected, actual)                                                     \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        int64_t not_expected_ = (not_expected);                                                                 \
        int64_t actual_ = (actual);                                                                             \
                                                                                                                \
        if ((not_expected_) == (actual_))                                                                       \
        {                                                                                                       \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                 \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_uint8_t(not_expected, actual)                                                   \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        uint8_t not_expected_ = (not_expected);                                                               \
        uint8_t actual_ = (actual);                                                                           \
                                                                                                              \
        if ((not_expected_) == (actual_))                                                                     \
        {                                                                                                     \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                               \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_not_equals_uint16_t(not_expected, actual)                                                    \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        uint16_t not_expected_ = (not_expected);                                                                \
        uint16_t actual_ = (actual);                                                                            \
                                                                                                                \
        if ((not_expected_) == (actual_))                                                                       \
        {                                                                                                       \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                 \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_uint32_t(not_expected, actual)                                                    \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        uint32_t not_expected_ = (not_expected);                                                                \
        uint32_t actual_ = (actual);                                                                            \
                                                                                                                \
        if ((not_expected_) == (actual_))                                                                       \
        {                                                                                                       \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                 \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_uint64_t(not_expected, actual)                                                    \
    do                                                                                                          \
    {                                                                                                           \
        const char *str = #actual;                                                                              \
        uint64_t not_expected_ = (not_expected);                                                                \
        uint64_t actual_ = (actual);                                                                            \
                                                                                                                \
        if ((not_expected_) == (actual_))                                                                       \
        {                                                                                                       \
            fprintf(stderr,                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                 \
        }                                                                                                       \
                                                                                                                \
    } while (0)

#define cmc_assert_not_equals_intmax_t(not_expected, actual)                                                      \
    do                                                                                                            \
    {                                                                                                             \
        const char *str = #actual;                                                                                \
        intmax_t not_expected_ = (not_expected);                                                                  \
        intmax_t actual_ = (actual);                                                                              \
                                                                                                                  \
        if ((not_expected_) == (actual_))                                                                         \
        {                                                                                                         \
            fprintf(stderr,                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                   \
        }                                                                                                         \
                                                                                                                  \
    } while (0)

#define cmc_assert_not_equals_uintmax_t(not_expected, actual)                                                     \
    do                                                                                                            \
    {                                                                                                             \
        const char *str = #actual;                                                                                \
        uintmax_t not_expected_ = (not_expected);                                                                 \
        uintmax_t actual_ = (actual);                                                                             \
                                                                                                                  \
        if ((not_expected_) == (actual_))                                                                         \
        {                                                                                                         \
            fprintf(stderr,                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                   \
        }                                                                                                         \
                                                                                                                  \
    } while (0)

#define cmc_assert_not_equals_size_t(not_expected, actual)                                                        \
    do                                                                                                            \
    {                                                                                                             \
        const char *str = #actual;                                                                                \
        size_t not_expected_ = (not_expected);                                                                    \
        size_t actual_ = (actual);                                                                                \
                                                                                                                  \
        if ((not_expected_) == (actual_))                                                                         \
        {                                                                                                         \
            fprintf(stderr,                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                                   \
        }                                                                                                         \
                                                                                                                  \
    } while (0)

#define cmc_assert_not_equals_float(not_expected, actual)                                     \
    do                                                                                        \
    {                                                                                         \
        const char *str = #actual;                                                            \
        float not_expected_ = (not_expected);                                                 \
        float actual_ = (actual);                                                             \
                                                                                              \
        if ((not_expected_) == (actual_))                                                     \
        {                                                                                     \
            fprintf(stderr,                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);               \
        }                                                                                     \
                                                                                              \
    } while (0)

#define cmc_assert_not_equals_double(not_expected, actual)                                      \
    do                                                                                          \
    {                                                                                           \
        const char *str = #actual;                                                              \
        double not_expected_ = (not_expected);                                                  \
        double actual_ = (actual);                                                              \
                                                                                                \
        if ((not_expected_) == (actual_))                                                       \
        {                                                                                       \
            fprintf(stderr,                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);                 \
        }                                                                                       \
                                                                                                \
    } while (0)

#define cmc_assert_not_equals_ptr(not_expected, actual)                                       \
    do                                                                                        \
    {                                                                                         \
        const char *str = #actual;                                                            \
        void *not_expected_ = (not_expected);                                                 \
        void *actual_ = (actual);                                                             \
                                                                                              \
        if ((not_expected_) == (actual_))                                                     \
        {                                                                                     \
            fprintf(stderr,                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %p Actual: %p\n", \
                    __FILE__, __func__, __LINE__, str, not_expected_, actual_);               \
        }                                                                                     \
                                                                                              \
    } while (0)

#define cmc_assert_not_equals__Bool(expected, actual)                                          \
    do                                                                                        \
    {                                                                                         \
        const char *str = #actual;                                                            \
        bool expected_ = (expected);                                                          \
        bool actual_ = (actual);                                                              \
                                                                                              \
        if ((expected_) != (actual_))                                                         \
        {                                                                                     \
            fprintf(stderr,                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected: %d Actual: %d\n", \
                    __FILE__, __func__, __LINE__, str, expected_, actual_);                   \
        }                                                                                     \
                                                                                              \
    } while (0)

#define cmc_assert_greater_int8_t(boundary, actual)                                                                          \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        int8_t boundary_ = (boundary);                                                                                       \
        int8_t actual_ = (actual);                                                                                           \
                                                                                                                             \
        if ((actual_) <= (boundary_))                                                                                        \
        {                                                                                                                    \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                  \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_greater_int16_t(boundary, actual)                                                                           \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        int16_t boundary_ = (boundary);                                                                                        \
        int16_t actual_ = (actual);                                                                                            \
                                                                                                                               \
        if ((actual_) <= (boundary_))                                                                                          \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_int32_t(boundary, actual)                                                                           \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        int32_t boundary_ = (boundary);                                                                                        \
        int32_t actual_ = (actual);                                                                                            \
                                                                                                                               \
        if ((actual_) <= (boundary_))                                                                                          \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_int64_t(boundary, actual)                                                                           \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        int64_t boundary_ = (boundary);                                                                                        \
        int64_t actual_ = (actual);                                                                                            \
                                                                                                                               \
        if ((actual_) <= (boundary_))                                                                                          \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_uint8_t(boundary, actual)                                                                         \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        uint8_t boundary_ = (boundary);                                                                                      \
        uint8_t actual_ = (actual);                                                                                          \
                                                                                                                             \
        if ((actual_) <= (boundary_))                                                                                        \
        {                                                                                                                    \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                  \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_greater_uint16_t(boundary, actual)                                                                          \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        uint16_t boundary_ = (boundary);                                                                                       \
        uint16_t actual_ = (actual);                                                                                           \
                                                                                                                               \
        if ((actual_) <= (boundary_))                                                                                          \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_uint32_t(boundary, actual)                                                                          \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        uint32_t boundary_ = (boundary);                                                                                       \
        uint32_t actual_ = (actual);                                                                                           \
                                                                                                                               \
        if ((actual_) <= (boundary_))                                                                                          \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_uint64_t(boundary, actual)                                                                          \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        uint64_t boundary_ = (boundary);                                                                                       \
        uint64_t actual_ = (actual);                                                                                           \
                                                                                                                               \
        if ((actual_) <= (boundary_))                                                                                          \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_intmax_t(boundary, actual)                                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        intmax_t boundary_ = (boundary);                                                                                         \
        intmax_t actual_ = (actual);                                                                                             \
                                                                                                                                 \
        if ((actual_) <= (boundary_))                                                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                      \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_greater_uintmax_t(boundary, actual)                                                                           \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        uintmax_t boundary_ = (boundary);                                                                                        \
        uintmax_t actual_ = (actual);                                                                                            \
                                                                                                                                 \
        if ((actual_) <= (boundary_))                                                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                      \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_greater_size_t(boundary, actual)                                                                              \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        size_t boundary_ = (boundary);                                                                                           \
        size_t actual_ = (actual);                                                                                               \
                                                                                                                                 \
        if ((actual_) <= (boundary_))                                                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                      \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_greater_float(boundary, actual)                                                           \
    do                                                                                                       \
    {                                                                                                        \
        const char *str = #actual;                                                                           \
        float boundary_ = (boundary);                                                                        \
        float actual_ = (actual);                                                                            \
                                                                                                             \
        if ((actual_) <= (boundary_))                                                                        \
        {                                                                                                    \
            fprintf(stderr,                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                  \
        }                                                                                                    \
                                                                                                             \
    } while (0)

#define cmc_assert_greater_double(boundary, actual)                                                            \
    do                                                                                                         \
    {                                                                                                          \
        const char *str = #actual;                                                                             \
        double boundary_ = (boundary);                                                                         \
        double actual_ = (actual);                                                                             \
                                                                                                               \
        if ((actual_) <= (boundary_))                                                                          \
        {                                                                                                      \
            fprintf(stderr,                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (excluded): %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                    \
        }                                                                                                      \
                                                                                                               \
    } while (0)

#define cmc_assert_greater_equals_int8_t(boundary, actual)                                                                   \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        int8_t boundary_ = (boundary);                                                                                       \
        int8_t actual_ = (actual);                                                                                           \
                                                                                                                             \
        if ((actual_) < (boundary_))                                                                                         \
        {                                                                                                                    \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                  \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_greater_equals_int16_t(boundary, actual)                                                                    \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        int16_t boundary_ = (boundary);                                                                                        \
        int16_t actual_ = (actual);                                                                                            \
                                                                                                                               \
        if ((actual_) < (boundary_))                                                                                           \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_equals_int32_t(boundary, actual)                                                                    \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        int32_t boundary_ = (boundary);                                                                                        \
        int32_t actual_ = (actual);                                                                                            \
                                                                                                                               \
        if ((actual_) < (boundary_))                                                                                           \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_equals_int64_t(boundary, actual)                                                                    \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        int64_t boundary_ = (boundary);                                                                                        \
        int64_t actual_ = (actual);                                                                                            \
                                                                                                                               \
        if ((actual_) < (boundary_))                                                                                           \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_equals_uint8_t(boundary, actual)                                                                  \
    do                                                                                                                       \
    {                                                                                                                        \
        const char *str = #actual;                                                                                           \
        uint8_t boundary_ = (boundary);                                                                                      \
        uint8_t actual_ = (actual);                                                                                          \
                                                                                                                             \
        if ((actual_) < (boundary_))                                                                                         \
        {                                                                                                                    \
            fprintf(stderr,                                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                  \
        }                                                                                                                    \
                                                                                                                             \
    } while (0)

#define cmc_assert_greater_equals_uint16_t(boundary, actual)                                                                   \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        uint16_t boundary_ = (boundary);                                                                                       \
        uint16_t actual_ = (actual);                                                                                           \
                                                                                                                               \
        if ((actual_) < (boundary_))                                                                                           \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_equals_uint32_t(boundary, actual)                                                                   \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        uint32_t boundary_ = (boundary);                                                                                       \
        uint32_t actual_ = (actual);                                                                                           \
                                                                                                                               \
        if ((actual_) < (boundary_))                                                                                           \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_equals_uint64_t(boundary, actual)                                                                   \
    do                                                                                                                         \
    {                                                                                                                          \
        const char *str = #actual;                                                                                             \
        uint64_t boundary_ = (boundary);                                                                                       \
        uint64_t actual_ = (actual);                                                                                           \
                                                                                                                               \
        if ((actual_) < (boundary_))                                                                                           \
        {                                                                                                                      \
            fprintf(stderr,                                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                    \
        }                                                                                                                      \
                                                                                                                               \
    } while (0)

#define cmc_assert_greater_equals_intmax_t(boundary, actual)                                                                     \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        intmax_t boundary_ = (boundary);                                                                                         \
        intmax_t actual_ = (actual);                                                                                             \
                                                                                                                                 \
        if ((actual_) < (boundary_))                                                                                             \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                      \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_greater_equals_uintmax_t(boundary, actual)                                                                    \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        uintmax_t boundary_ = (boundary);                                                                                        \
        uintmax_t actual_ = (actual);                                                                                            \
                                                                                                                                 \
        if ((actual_) < (boundary_))                                                                                             \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                      \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_greater_equals_size_t(boundary, actual)                                                                       \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        size_t boundary_ = (boundary);                                                                                           \
        size_t actual_ = (actual);                                                                                               \
                                                                                                                                 \
        if ((actual_) < (boundary_))                                                                                             \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                      \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_greater_equals_float(boundary, actual)                                                    \
    do                                                                                                       \
    {                                                                                                        \
        const char *str = #actual;                                                                           \
        float boundary_ = (boundary);                                                                        \
        float actual_ = (actual);                                                                            \
                                                                                                             \
        if ((actual_) < (boundary_))                                                                         \
        {                                                                                                    \
            fprintf(stderr,                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                  \
        }                                                                                                    \
                                                                                                             \
    } while (0)

#define cmc_assert_greater_equals_double(boundary, actual)                                                     \
    do                                                                                                         \
    {                                                                                                          \
        const char *str = #actual;                                                                             \
        double boundary_ = (boundary);                                                                         \
        double actual_ = (actual);                                                                             \
                                                                                                               \
        if ((actual_) < (boundary_))                                                                           \
        {                                                                                                      \
            fprintf(stderr,                                                                                    \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Greater (included): %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                    \
        }                                                                                                      \
                                                                                                               \
    } while (0)

#define cmc_assert_lesser_int8_t(boundary, actual)                                                                          \
    do                                                                                                                      \
    {                                                                                                                       \
        const char *str = #actual;                                                                                          \
        int8_t boundary_ = (boundary);                                                                                      \
        int8_t actual_ = (actual);                                                                                          \
                                                                                                                            \
        if ((actual_) >= (boundary_))                                                                                       \
        {                                                                                                                   \
            fprintf(stderr,                                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                 \
        }                                                                                                                   \
                                                                                                                            \
    } while (0)

#define cmc_assert_lesser_int16_t(boundary, actual)                                                                           \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int16_t boundary_ = (boundary);                                                                                       \
        int16_t actual_ = (actual);                                                                                           \
                                                                                                                              \
        if ((actual_) >= (boundary_))                                                                                         \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_int32_t(boundary, actual)                                                                           \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int32_t boundary_ = (boundary);                                                                                       \
        int32_t actual_ = (actual);                                                                                           \
                                                                                                                              \
        if ((actual_) >= (boundary_))                                                                                         \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_int64_t(boundary, actual)                                                                           \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int64_t boundary_ = (boundary);                                                                                       \
        int64_t actual_ = (actual);                                                                                           \
                                                                                                                              \
        if ((actual_) >= (boundary_))                                                                                         \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_uint8_t(boundary, actual)                                                                         \
    do                                                                                                                      \
    {                                                                                                                       \
        const char *str = #actual;                                                                                          \
        uint8_t boundary_ = (boundary);                                                                                     \
        uint8_t actual_ = (actual);                                                                                         \
                                                                                                                            \
        if ((actual_) >= (boundary_))                                                                                       \
        {                                                                                                                   \
            fprintf(stderr,                                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                 \
        }                                                                                                                   \
                                                                                                                            \
    } while (0)

#define cmc_assert_lesser_uint16_t(boundary, actual)                                                                          \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint16_t boundary_ = (boundary);                                                                                      \
        uint16_t actual_ = (actual);                                                                                          \
                                                                                                                              \
        if ((actual_) >= (boundary_))                                                                                         \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_uint32_t(boundary, actual)                                                                          \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint32_t boundary_ = (boundary);                                                                                      \
        uint32_t actual_ = (actual);                                                                                          \
                                                                                                                              \
        if ((actual_) >= (boundary_))                                                                                         \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_uint64_t(boundary, actual)                                                                          \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint64_t boundary_ = (boundary);                                                                                      \
        uint64_t actual_ = (actual);                                                                                          \
                                                                                                                              \
        if ((actual_) >= (boundary_))                                                                                         \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_intmax_t(boundary, actual)                                                                            \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        intmax_t boundary_ = (boundary);                                                                                        \
        intmax_t actual_ = (actual);                                                                                            \
                                                                                                                                \
        if ((actual_) >= (boundary_))                                                                                           \
        {                                                                                                                       \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                     \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_lesser_uintmax_t(boundary, actual)                                                                           \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        uintmax_t boundary_ = (boundary);                                                                                       \
        uintmax_t actual_ = (actual);                                                                                           \
                                                                                                                                \
        if ((actual_) >= (boundary_))                                                                                           \
        {                                                                                                                       \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                     \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_lesser_size_t(boundary, actual)                                                                              \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        size_t boundary_ = (boundary);                                                                                          \
        size_t actual_ = (actual);                                                                                              \
                                                                                                                                \
        if ((actual_) >= (boundary_))                                                                                           \
        {                                                                                                                       \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                     \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_lesser_float(boundary, actual)                                                           \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        float boundary_ = (boundary);                                                                       \
        float actual_ = (actual);                                                                           \
                                                                                                            \
        if ((actual_) >= (boundary_))                                                                       \
        {                                                                                                   \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                 \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_lesser_double(boundary, actual)                                                            \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        double boundary_ = (boundary);                                                                        \
        double actual_ = (actual);                                                                            \
                                                                                                              \
        if ((actual_) >= (boundary_))                                                                         \
        {                                                                                                     \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (excluded): %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                   \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_lesser_equals_int8_t(boundary, actual)                                                                   \
    do                                                                                                                      \
    {                                                                                                                       \
        const char *str = #actual;                                                                                          \
        int8_t boundary_ = (boundary);                                                                                      \
        int8_t actual_ = (actual);                                                                                          \
                                                                                                                            \
        if ((actual_) > (boundary_))                                                                                        \
        {                                                                                                                   \
            fprintf(stderr,                                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRId8 " Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                 \
        }                                                                                                                   \
                                                                                                                            \
    } while (0)

#define cmc_assert_lesser_equals_int16_t(boundary, actual)                                                                    \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int16_t boundary_ = (boundary);                                                                                       \
        int16_t actual_ = (actual);                                                                                           \
                                                                                                                              \
        if ((actual_) > (boundary_))                                                                                          \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRId16 " Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_equals_int32_t(boundary, actual)                                                                    \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int32_t boundary_ = (boundary);                                                                                       \
        int32_t actual_ = (actual);                                                                                           \
                                                                                                                              \
        if ((actual_) > (boundary_))                                                                                          \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRId32 " Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_equals_int64_t(boundary, actual)                                                                    \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int64_t boundary_ = (boundary);                                                                                       \
        int64_t actual_ = (actual);                                                                                           \
                                                                                                                              \
        if ((actual_) > (boundary_))                                                                                          \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRId64 " Actual: %" PRId64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_equals_uint8_t(boundary, actual)                                                                  \
    do                                                                                                                      \
    {                                                                                                                       \
        const char *str = #actual;                                                                                          \
        uint8_t boundary_ = (boundary);                                                                                     \
        uint8_t actual_ = (actual);                                                                                         \
                                                                                                                            \
        if ((actual_) > (boundary_))                                                                                        \
        {                                                                                                                   \
            fprintf(stderr,                                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRIu8 " Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                 \
        }                                                                                                                   \
                                                                                                                            \
    } while (0)

#define cmc_assert_lesser_equals_uint16_t(boundary, actual)                                                                   \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint16_t boundary_ = (boundary);                                                                                      \
        uint16_t actual_ = (actual);                                                                                          \
                                                                                                                              \
        if ((actual_) > (boundary_))                                                                                          \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRIu16 " Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_equals_uint32_t(boundary, actual)                                                                   \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint32_t boundary_ = (boundary);                                                                                      \
        uint32_t actual_ = (actual);                                                                                          \
                                                                                                                              \
        if ((actual_) > (boundary_))                                                                                          \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRIu32 " Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_equals_uint64_t(boundary, actual)                                                                   \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint64_t boundary_ = (boundary);                                                                                      \
        uint64_t actual_ = (actual);                                                                                          \
                                                                                                                              \
        if ((actual_) > (boundary_))                                                                                          \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRIu64 " Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                   \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_lesser_equals_intmax_t(boundary, actual)                                                                     \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        intmax_t boundary_ = (boundary);                                                                                        \
        intmax_t actual_ = (actual);                                                                                            \
                                                                                                                                \
        if ((actual_) > (boundary_))                                                                                            \
        {                                                                                                                       \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRIdMAX " Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                     \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_lesser_equals_uintmax_t(boundary, actual)                                                                    \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        uintmax_t boundary_ = (boundary);                                                                                       \
        uintmax_t actual_ = (actual);                                                                                           \
                                                                                                                                \
        if ((actual_) > (boundary_))                                                                                            \
        {                                                                                                                       \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                     \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_lesser_equals_size_t(boundary, actual)                                                                       \
    do                                                                                                                          \
    {                                                                                                                           \
        const char *str = #actual;                                                                                              \
        size_t boundary_ = (boundary);                                                                                          \
        size_t actual_ = (actual);                                                                                              \
                                                                                                                                \
        if ((actual_) > (boundary_))                                                                                            \
        {                                                                                                                       \
            fprintf(stderr,                                                                                                     \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %" PRIuMAX " Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                                     \
        }                                                                                                                       \
                                                                                                                                \
    } while (0)

#define cmc_assert_lesser_equals_float(boundary, actual)                                                    \
    do                                                                                                      \
    {                                                                                                       \
        const char *str = #actual;                                                                          \
        float boundary_ = (boundary);                                                                       \
        float actual_ = (actual);                                                                           \
                                                                                                            \
        if ((actual_) > (boundary_))                                                                        \
        {                                                                                                   \
            fprintf(stderr,                                                                                 \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %f Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                 \
        }                                                                                                   \
                                                                                                            \
    } while (0)

#define cmc_assert_lesser_equals_double(boundary, actual)                                                     \
    do                                                                                                        \
    {                                                                                                         \
        const char *str = #actual;                                                                            \
        double boundary_ = (boundary);                                                                        \
        double actual_ = (actual);                                                                            \
                                                                                                              \
        if ((actual_) > (boundary_))                                                                          \
        {                                                                                                     \
            fprintf(stderr,                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Lesser (included): %lf Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, boundary_, actual_);                                   \
        }                                                                                                     \
                                                                                                              \
    } while (0)

#define cmc_assert_in_range_int8_t(lower_bound, upper_bound, actual)                                                          \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        int8_t lower_bound_ = (lower_bound);                                                                                  \
        int8_t upper_bound_ = (upper_bound);                                                                                  \
        int8_t actual_ = (actual);                                                                                            \
                                                                                                                              \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                         \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRId8 ", %" PRId8 "] Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                  \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_in_range_int16_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        int16_t lower_bound_ = (lower_bound);                                                                                    \
        int16_t upper_bound_ = (upper_bound);                                                                                    \
        int16_t actual_ = (actual);                                                                                              \
                                                                                                                                 \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRId16 ", %" PRId16 "] Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                     \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_int32_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        int32_t lower_bound_ = (lower_bound);                                                                                    \
        int32_t upper_bound_ = (upper_bound);                                                                                    \
        int32_t actual_ = (actual);                                                                                              \
                                                                                                                                 \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRId32 ", %" PRId32 "] Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                     \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_int64_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        int64_t lower_bound_ = (lower_bound);                                                                                    \
        int64_t upper_bound_ = (upper_bound);                                                                                    \
        int64_t actual_ = (actual);                                                                                              \
                                                                                                                                 \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRId32 ", %" PRId32 "] Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                     \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_uint8_t(lower_bound, upper_bound, actual)                                                         \
    do                                                                                                                        \
    {                                                                                                                         \
        const char *str = #actual;                                                                                            \
        uint8_t lower_bound_ = (lower_bound);                                                                                 \
        uint8_t upper_bound_ = (upper_bound);                                                                                 \
        uint8_t actual_ = (actual);                                                                                           \
                                                                                                                              \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                         \
        {                                                                                                                     \
            fprintf(stderr,                                                                                                   \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIu8 ", %" PRIu8 "] Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                  \
        }                                                                                                                     \
                                                                                                                              \
    } while (0)

#define cmc_assert_in_range_uint16_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        uint16_t lower_bound_ = (lower_bound);                                                                                   \
        uint16_t upper_bound_ = (upper_bound);                                                                                   \
        uint16_t actual_ = (actual);                                                                                             \
                                                                                                                                 \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIu16 ", %" PRIu16 "] Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                     \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_uint32_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        uint32_t lower_bound_ = (lower_bound);                                                                                   \
        uint32_t upper_bound_ = (upper_bound);                                                                                   \
        uint32_t actual_ = (actual);                                                                                             \
                                                                                                                                 \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIu32 ", %" PRIu32 "] Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                     \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_uint64_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                           \
    {                                                                                                                            \
        const char *str = #actual;                                                                                               \
        uint64_t lower_bound_ = (lower_bound);                                                                                   \
        uint64_t upper_bound_ = (upper_bound);                                                                                   \
        uint64_t actual_ = (actual);                                                                                             \
                                                                                                                                 \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                            \
        {                                                                                                                        \
            fprintf(stderr,                                                                                                      \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIu64 ", %" PRIu64 "] Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                     \
        }                                                                                                                        \
                                                                                                                                 \
    } while (0)

#define cmc_assert_in_range_intmax_t(lower_bound, upper_bound, actual)                                                              \
    do                                                                                                                              \
    {                                                                                                                               \
        const char *str = #actual;                                                                                                  \
        intmax_t lower_bound_ = (lower_bound);                                                                                      \
        intmax_t upper_bound_ = (upper_bound);                                                                                      \
        intmax_t actual_ = (actual);                                                                                                \
                                                                                                                                    \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                               \
        {                                                                                                                           \
            fprintf(stderr,                                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIdMAX ", %" PRIdMAX "] Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                        \
        }                                                                                                                           \
                                                                                                                                    \
    } while (0)

#define cmc_assert_in_range_uintmax_t(lower_bound, upper_bound, actual)                                                             \
    do                                                                                                                              \
    {                                                                                                                               \
        const char *str = #actual;                                                                                                  \
        uintmax_t lower_bound_ = (lower_bound);                                                                                     \
        uintmax_t upper_bound_ = (upper_bound);                                                                                     \
        uintmax_t actual_ = (actual);                                                                                               \
                                                                                                                                    \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                               \
        {                                                                                                                           \
            fprintf(stderr,                                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIuMAX ", %" PRIuMAX "] Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                        \
        }                                                                                                                           \
                                                                                                                                    \
    } while (0)

#define cmc_assert_in_range_size_t(lower_bound, upper_bound, actual)                                                                \
    do                                                                                                                              \
    {                                                                                                                               \
        const char *str = #actual;                                                                                                  \
        size_t lower_bound_ = (lower_bound);                                                                                        \
        size_t upper_bound_ = (upper_bound);                                                                                        \
        size_t actual_ = (actual);                                                                                                  \
                                                                                                                                    \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                                               \
        {                                                                                                                           \
            fprintf(stderr,                                                                                                         \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%" PRIuMAX ", %" PRIuMAX "] Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                        \
        }                                                                                                                           \
                                                                                                                                    \
    } while (0)

#define cmc_assert_in_range_float(lower_bound, upper_bound, actual)                                   \
    do                                                                                                \
    {                                                                                                 \
        const char *str = #actual;                                                                    \
        float lower_bound_ = (lower_bound);                                                           \
        float upper_bound_ = (upper_bound);                                                           \
        float actual_ = (actual);                                                                     \
                                                                                                      \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                 \
        {                                                                                             \
            fprintf(stderr,                                                                           \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%f, %f] Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);          \
        }                                                                                             \
                                                                                                      \
    } while (0)

#define cmc_assert_in_range_double(lower_bound, upper_bound, actual)                                     \
    do                                                                                                   \
    {                                                                                                    \
        const char *str = #actual;                                                                       \
        double lower_bound_ = (lower_bound);                                                             \
        double upper_bound_ = (upper_bound);                                                             \
        double actual_ = (actual);                                                                       \
                                                                                                         \
        if ((actual_) < (lower_bound_) || (actual_) > (upper_bound_))                                    \
        {                                                                                                \
            fprintf(stderr,                                                                              \
                    "Assertion Failed at %s:%s:%u for { %s }: Expected Range: [%lf, %lf] Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);             \
        }                                                                                                \
                                                                                                         \
    } while (0)

#define cmc_assert_not_in_range_int8_t(lower_bound, upper_bound, actual)                                                          \
    do                                                                                                                            \
    {                                                                                                                             \
        const char *str = #actual;                                                                                                \
        int8_t lower_bound_ = (lower_bound);                                                                                      \
        int8_t upper_bound_ = (upper_bound);                                                                                      \
        int8_t actual_ = (actual);                                                                                                \
                                                                                                                                  \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                           \
        {                                                                                                                         \
            fprintf(stderr,                                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRId8 ", %" PRId8 "] Actual: %" PRId8 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                      \
        }                                                                                                                         \
                                                                                                                                  \
    } while (0)

#define cmc_assert_not_in_range_int16_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        int16_t lower_bound_ = (lower_bound);                                                                                        \
        int16_t upper_bound_ = (upper_bound);                                                                                        \
        int16_t actual_ = (actual);                                                                                                  \
                                                                                                                                     \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                              \
        {                                                                                                                            \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRId16 ", %" PRId16 "] Actual: %" PRId16 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                         \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_int32_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        int32_t lower_bound_ = (lower_bound);                                                                                        \
        int32_t upper_bound_ = (upper_bound);                                                                                        \
        int32_t actual_ = (actual);                                                                                                  \
                                                                                                                                     \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                              \
        {                                                                                                                            \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRId32 ", %" PRId32 "] Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                         \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_int64_t(lower_bound, upper_bound, actual)                                                            \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        int64_t lower_bound_ = (lower_bound);                                                                                        \
        int64_t upper_bound_ = (upper_bound);                                                                                        \
        int64_t actual_ = (actual);                                                                                                  \
                                                                                                                                     \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                              \
        {                                                                                                                            \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRId32 ", %" PRId32 "] Actual: %" PRId32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                         \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_uint8_t(lower_bound, upper_bound, actual)                                                         \
    do                                                                                                                            \
    {                                                                                                                             \
        const char *str = #actual;                                                                                                \
        uint8_t lower_bound_ = (lower_bound);                                                                                     \
        uint8_t upper_bound_ = (upper_bound);                                                                                     \
        uint8_t actual_ = (actual);                                                                                               \
                                                                                                                                  \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                           \
        {                                                                                                                         \
            fprintf(stderr,                                                                                                       \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIu8 ", %" PRIu8 "] Actual: %" PRIu8 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                      \
        }                                                                                                                         \
                                                                                                                                  \
    } while (0)

#define cmc_assert_not_in_range_uint16_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        uint16_t lower_bound_ = (lower_bound);                                                                                       \
        uint16_t upper_bound_ = (upper_bound);                                                                                       \
        uint16_t actual_ = (actual);                                                                                                 \
                                                                                                                                     \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                              \
        {                                                                                                                            \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIu16 ", %" PRIu16 "] Actual: %" PRIu16 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                         \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_uint32_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        uint32_t lower_bound_ = (lower_bound);                                                                                       \
        uint32_t upper_bound_ = (upper_bound);                                                                                       \
        uint32_t actual_ = (actual);                                                                                                 \
                                                                                                                                     \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                              \
        {                                                                                                                            \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIu32 ", %" PRIu32 "] Actual: %" PRIu32 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                         \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_uint64_t(lower_bound, upper_bound, actual)                                                           \
    do                                                                                                                               \
    {                                                                                                                                \
        const char *str = #actual;                                                                                                   \
        uint64_t lower_bound_ = (lower_bound);                                                                                       \
        uint64_t upper_bound_ = (upper_bound);                                                                                       \
        uint64_t actual_ = (actual);                                                                                                 \
                                                                                                                                     \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                              \
        {                                                                                                                            \
            fprintf(stderr,                                                                                                          \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIu64 ", %" PRIu64 "] Actual: %" PRIu64 "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                         \
        }                                                                                                                            \
                                                                                                                                     \
    } while (0)

#define cmc_assert_not_in_range_intmax_t(lower_bound, upper_bound, actual)                                                              \
    do                                                                                                                                  \
    {                                                                                                                                   \
        const char *str = #actual;                                                                                                      \
        intmax_t lower_bound_ = (lower_bound);                                                                                          \
        intmax_t upper_bound_ = (upper_bound);                                                                                          \
        intmax_t actual_ = (actual);                                                                                                    \
                                                                                                                                        \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                                 \
        {                                                                                                                               \
            fprintf(stderr,                                                                                                             \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIdMAX ", %" PRIdMAX "] Actual: %" PRIdMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                            \
        }                                                                                                                               \
                                                                                                                                        \
    } while (0)

#define cmc_assert_not_in_range_uintmax_t(lower_bound, upper_bound, actual)                                                             \
    do                                                                                                                                  \
    {                                                                                                                                   \
        const char *str = #actual;                                                                                                      \
        uintmax_t lower_bound_ = (lower_bound);                                                                                         \
        uintmax_t upper_bound_ = (upper_bound);                                                                                         \
        uintmax_t actual_ = (actual);                                                                                                   \
                                                                                                                                        \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                                 \
        {                                                                                                                               \
            fprintf(stderr,                                                                                                             \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIuMAX ", %" PRIuMAX "] Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                            \
        }                                                                                                                               \
                                                                                                                                        \
    } while (0)

#define cmc_assert_not_in_range_size_t(lower_bound, upper_bound, actual)                                                                \
    do                                                                                                                                  \
    {                                                                                                                                   \
        const char *str = #actual;                                                                                                      \
        size_t lower_bound_ = (lower_bound);                                                                                            \
        size_t upper_bound_ = (upper_bound);                                                                                            \
        size_t actual_ = (actual);                                                                                                      \
                                                                                                                                        \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                                                 \
        {                                                                                                                               \
            fprintf(stderr,                                                                                                             \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%" PRIuMAX ", %" PRIuMAX "] Actual: %" PRIuMAX "\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                                            \
        }                                                                                                                               \
                                                                                                                                        \
    } while (0)

#define cmc_assert_not_in_range_float(lower_bound, upper_bound, actual)                                   \
    do                                                                                                    \
    {                                                                                                     \
        const char *str = #actual;                                                                        \
        float lower_bound_ = (lower_bound);                                                               \
        float upper_bound_ = (upper_bound);                                                               \
        float actual_ = (actual);                                                                         \
                                                                                                          \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                   \
        {                                                                                                 \
            fprintf(stderr,                                                                               \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%f, %f] Actual: %f\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);              \
        }                                                                                                 \
                                                                                                          \
    } while (0)

#define cmc_assert_not_in_range_double(lower_bound, upper_bound, actual)                                     \
    do                                                                                                       \
    {                                                                                                        \
        const char *str = #actual;                                                                           \
        double lower_bound_ = (lower_bound);                                                                 \
        double upper_bound_ = (upper_bound);                                                                 \
        double actual_ = (actual);                                                                           \
                                                                                                             \
        if ((actual_) >= (lower_bound_) && (actual_) <= (upper_bound_))                                      \
        {                                                                                                    \
            fprintf(stderr,                                                                                  \
                    "Assertion Failed at %s:%s:%u for { %s }: Not Expected Range: [%lf, %lf] Actual: %lf\n", \
                    __FILE__, __func__, __LINE__, str, lower_bound_, upper_bound_, actual_);                 \
        }                                                                                                    \
                                                                                                             \
    } while (0)

#endif /* CMC_ASSERT_H */
