/**
 * utl/test.h
 *
 * Creation Date: 26/06/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Simple Unit Test Utility
 *
 * Required imports:
 *     - assert.h (to integrate with the cmc_assert_state variable)
 *     - timer.h  (to calculate the execution time for the unit test)
 *
 * CMC_CREATE_UNIT
 *     Create a UnitTest
 *     Parameters:
 *         - UNAME   : UnitTest name (const char *)
 *         - VERBOSE : Print PASSED / FAILED messages if true (bool)
 *         - BODY    : Body containing all tests related to this UnitTest
 *
 * CMC_CREATE_TEST
 *     Create a Test inside a UnitTest.
 *     Parameters:
 *         - TNAME : Test name (const char *)
 *         - BODY  : Block of code containing a specific test
 *     Inside the BODY use the macros in ./utl/assert.h. The unit test will keep
 *     track of cmc_assert_state and automatically pass or fail tests.
 *
 * CMC_TEST_COLOR
 *      Define this macro to allow colored output.
 */

#ifndef CMC_UTL_TEST_H
#define CMC_UTL_TEST_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../cor/core.h"
#include "assert.h"
#include "timer.h"

struct cmc_test_info
{
    uintmax_t total;
    uintmax_t passed;
    uintmax_t failed;
    uintmax_t assert_total;
    uintmax_t assert_failed;
    bool aborted;
    bool verbose;
};

#ifdef CMC_TEST_COLOR
static inline const char *cmc_test_color(int i)
{
    // 0 - Default, all off
    // 1 - Failed (red)
    // 2 - Passed (green)
    // 3 - Box    (gray)
    // 4 - Name   (cyan)
    // 5 - Total  (yellow)
    static const char *cmc_test_colors[] = { "\x1b[0m", "\x1b[31m", "\x1b[32m", "\x1b[90m", "\x1b[36m", "\x1b[33m" };
    return cmc_test_colors[i];
#else
static inline const char *cmc_test_color(int i)
{
    (void)i;
    return "";
#endif
}

/* Logging function */
CMC_UNUSED static void cmc_test_log(const char *unit_name, const char *current_test, bool aborted, bool passed)
{
    if (aborted)
    {
        printf("    X UNIT TEST");
        printf("%s", cmc_test_color(4));
        printf(" %s ", unit_name);
        printf("%sABORTED%s", cmc_test_color(5), cmc_test_color(0));
        printf(" at %s\n", current_test);
    }
    else
    {
        printf("%s", passed ? cmc_test_color(2) : cmc_test_color(1));
        printf("    |");
        printf("%s", cmc_test_color(0));
        printf(" %s", current_test);
        size_t str_len = strlen(current_test);
        if (str_len < 53)
        {
            printf("%s", cmc_test_color(3));
            for (size_t i = 0; i < 53 - str_len; i++)
                putchar('.');
        }
        printf("%s", passed ? cmc_test_color(2) : cmc_test_color(1));
        printf("%s", passed ? "PASSED" : "FAILED");
        printf("%s\n", cmc_test_color(0));
    }
}

#define CMC_CREATE_UNIT(UNAME, VERBOSE, BODY) \
    uintmax_t UNAME(void) \
    { \
        const char *unit_name = #UNAME; \
        const char *current_test = NULL; \
\
        struct cmc_test_info tinfo = { 0 }; \
        struct cmc_timer timer = { 0 }; \
\
        tinfo.verbose = VERBOSE; \
        tinfo.assert_total = cmc_assert_total; \
        tinfo.assert_failed = cmc_assert_failed; \
\
        printf("%s", cmc_test_color(3)); \
        printf(" +---------------------------------------------------------------+\n"); \
        printf(" | "); \
        printf("%s", cmc_test_color(0)); \
        printf("Unit "); \
        printf("%s", cmc_test_color(4)); \
        printf("%-56s", unit_name); \
        printf("%s", cmc_test_color(3)); \
        printf(" |\n +---------------------------------------------------------------+"); \
        printf("%s\n", cmc_test_color(0)); \
\
        /* Tests */ \
        cmc_timer_start(timer); \
\
        BODY; \
\
        cmc_timer_stop(timer); \
\
    unittest_abort: \
        if (tinfo.aborted) \
        { \
            cmc_test_log(unit_name, current_test, true, false); \
        } \
\
        tinfo.assert_total = cmc_assert_total - tinfo.assert_total; \
        tinfo.assert_failed = cmc_assert_failed - tinfo.assert_failed; \
\
        printf("%s", cmc_test_color(3)); \
        printf(" +---------------------------------------------------------------+\n | "); \
        printf("%s", cmc_test_color(0)); \
        printf("Summary : "); \
        printf("%s", cmc_test_color(4)); \
        printf("%-52s", unit_name); \
        printf("%s|\n", cmc_test_color(3)); \
        printf(" +---------++----------------+-----------------+-----------------+\n | "); \
        printf("%s", cmc_test_color(0)); \
        printf("Tests   "); \
        printf("%s", cmc_test_color(3)); \
        printf("||"); \
        printf("%s", cmc_test_color(5)); \
        printf(" Total %8" PRIuMAX "", tinfo.total); \
        printf("%s", cmc_test_color(3)); \
        printf(" | "); \
        printf("%s", cmc_test_color(2)); \
        printf("Passed %8" PRIuMAX "", tinfo.passed); \
        printf("%s", cmc_test_color(3)); \
        printf(" | "); \
        printf("%s", cmc_test_color(1)); \
        printf("Failed %8" PRIuMAX "", tinfo.failed); \
        printf("%s", cmc_test_color(3)); \
        printf(" |\n | "); \
        printf("%s", cmc_test_color(0)); \
        printf("Asserts "); \
        printf("%s", cmc_test_color(3)); \
        printf("||"); \
        printf("%s", cmc_test_color(5)); \
        printf(" Total %8" PRIuMAX "", tinfo.assert_total); \
        printf("%s", cmc_test_color(3)); \
        printf(" | "); \
        printf("%s", cmc_test_color(2)); \
        printf("Passed %8" PRIuMAX "", tinfo.assert_total - tinfo.assert_failed); \
        printf("%s", cmc_test_color(3)); \
        printf(" | "); \
        printf("%s", cmc_test_color(1)); \
        printf("Failed %8" PRIuMAX "", tinfo.assert_failed); \
        printf("%s", cmc_test_color(3)); \
        printf(" |\n"); \
        printf(" +---------++----------------+-----------------+-----------------+\n | "); \
        printf("%s", cmc_test_color(0)); \
        printf("Total Runtime : %32.0f milliseconds", timer.result); \
        printf(" %s|\n", cmc_test_color(3)); \
        printf(" +---------------------------------------------------------------+\n"); \
        printf("%s", cmc_test_color(0)); \
        printf("\n"); \
\
        return tinfo.failed; \
    }

#define CMC_CREATE_TEST(TNAME, BODY) \
    do \
    { \
        current_test = #TNAME; \
\
        tinfo.total += 1; \
\
        /* Provided by assert.h */ \
        cmc_assert_state = true; \
\
        BODY; \
\
        if (tinfo.verbose) \
        { \
            if (!cmc_assert_state) \
            { \
                tinfo.failed += 1; \
                cmc_test_log(unit_name, current_test, false, false); \
            } \
            else \
            { \
                tinfo.passed += 1; \
                cmc_test_log(unit_name, current_test, false, true); \
            } \
        } \
\
    } while (0)

#define CMC_TEST_ABORT() \
    do \
    { \
        tinfo.aborted = true; \
        tinfo.total -= 1; \
        goto unittest_abort; \
\
    } while (0)

#endif /* CMC_UTL_TEST_H */
