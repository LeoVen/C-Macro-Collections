/**
 * utl_timer.h
 *
 * Creation Date: 12/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 *  Simple timer macros utility for timing code execution.
 */

#ifndef CMC_UTL_TIMER_H
#define CMC_UTL_TIMER_H

#include <time.h>

struct cmc_timer
{
    clock_t start;
    clock_t stop;
    double result; /* The result is given in milliseconds */
};

#define cmc_timer_start(timer) \
    do \
    { \
        struct cmc_timer *t__ = &(timer); \
        t__->start = clock(); \
    } while (0)

#define cmc_timer_stop(timer) \
    do \
    { \
        struct cmc_timer *t__ = &(timer); \
        t__->stop = clock(); \
        t__->result = (double)(t__->stop - t__->start) * 1000.0 / CLOCKS_PER_SEC; \
    } while (0)

#endif /* CMC_UTL_TIMER_H */
