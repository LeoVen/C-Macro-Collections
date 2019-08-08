/**
 * test.h
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
 *     Inside the BODY use the following macros:
 *         - CMC_TEST_PASS
 *             Pass current test.
 *         - CMC_TEST_FAIL
 *             Fail current test.
 *         - CMC_TEST_PASS_ELSE_FAIL
 *             If given expression is true, pass current test, else fail.
 *         - CMC_TEST_ABORT
 *             Abort current unit test and displays error message. All tests
 *             after the abort won't be called.
 *     All of these macros can only be called once per test.
 *
 * CMC_TEST_COLOR
 *      Define this macro to allow colored output.
 */

#ifndef CMC_TEST_H
#define CMC_TEST_H

#include "timer.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CMC_TEST_COLOR
static const char *cmc_test_color[] = {"\x1b[32m", "\x1b[31m", "\x1b[35m"};
#endif

typedef struct cmc_test_info_s
{
    uintmax_t total;
    uintmax_t passed;
    uintmax_t failed;
    bool aborted;
    bool verbose;
} cmc_test_info;

/* Logging function */
static void cmc_test_log(const char *unit_name, const char *current_test, bool aborted, bool passed)
{
    if (aborted)
    {
#ifdef CMC_TEST_COLOR
        printf("UNIT_TEST %s %sABORTED\x1b[0m AT %s\n",
               unit_name,
               cmc_test_color[2],
               current_test);
#else
        printf("UNIT_TEST %s ABORTED at %s\n", unit_name, current_test);
#endif
    }
    else
    {
#ifdef CMC_TEST_COLOR
        printf("Test from %s %s%7s\x1b[0m %s\n",
               unit_name,
               passed ? cmc_test_color[0] : cmc_test_color[1],
               passed ? "PASSED" : "FAILED",
               current_test);
#else
        printf("Test from %s %7s %s\n", unit_name, passed ? "PASSED" : "FAILED", current_test);
#endif
    }
}

#define CMC_CREATE_UNIT(UNAME, VERBOSE, BODY)                                               \
    void UNAME(void)                                                                        \
    {                                                                                       \
        const char *unit_name = #UNAME;                                                     \
        const char *current_test = NULL;                                                    \
                                                                                            \
        cmc_test_info tinfo = {0};                                                          \
        cmc_timer timer = {0};                                                              \
                                                                                            \
        tinfo.verbose = VERBOSE;                                                            \
                                                                                            \
        /* Tests */                                                                         \
        timer_start(timer);                                                                 \
        BODY;                                                                               \
        timer_stop(timer);                                                                  \
        timer_calc(timer);                                                                  \
                                                                                            \
        unittest_abort:                                                                     \
        if (tinfo.aborted)                                                                  \
        {                                                                                   \
            cmc_test_log(unit_name, current_test, true, false);                             \
        }                                                                                   \
                                                                                            \
        printf("+--------------------------------------------------+\n");                   \
        printf("| Unit Test Report : %-30s|\n", unit_name);                                 \
        printf("|   TOTAL  : %10" PRIuMAX "                            |\n", tinfo.total);  \
        printf("|   PASSED : %10" PRIuMAX "                            |\n", tinfo.passed); \
        printf("|   FAILED : %10" PRIuMAX "                            |\n", tinfo.failed); \
        printf("|                                                  |\n");                   \
        printf("| Total Unit Test Runtime : %9.0f milliseconds |\n", timer.result);         \
        printf("+--------------------------------------------------+\n");                   \
    }

#define CMC_CREATE_TEST(TNAME, BODY) \
    do                               \
    {                                \
        current_test = #TNAME;       \
                                     \
        tinfo.total += 1;            \
                                     \
        BODY;                        \
                                     \
    } while (0)

#define CMC_TEST_PASS()                                         \
    do                                                          \
    {                                                           \
        tinfo.passed += 1;                                      \
        if (tinfo.verbose)                                      \
            cmc_test_log(unit_name, current_test, false, true); \
                                                                \
    } while (0)

#define CMC_TEST_FAIL()                                          \
    do                                                           \
    {                                                            \
        tinfo.failed += 1;                                       \
        if (tinfo.verbose)                                       \
            cmc_test_log(unit_name, current_test, false, false); \
                                                                 \
    } while (0)

#define CMC_TEST_PASS_ELSE_FAIL(EXPRESSION) \
    do                                      \
    {                                       \
        if (EXPRESSION)                     \
            CMC_TEST_PASS();                \
        else                                \
            CMC_TEST_FAIL();                \
                                            \
    } while (0)

#define CMC_TEST_ABORT()      \
    do                        \
    {                         \
                              \
        tinfo.aborted = true; \
        tinfo.total -= 1;     \
        goto unittest_abort;  \
                              \
    } while (0)

#endif /* CMC_TEST_H */
