/**
 * timer.h
 *
 * Creation Date: 12/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/* Simple timer macros utility */

#ifndef CMC_TIMER_H
#define CMC_TIMER_H

#include <time.h>

struct cmc_timer
{
    clock_t start;
    clock_t stop;
    double result;
};

#define cmc_timer_start(timer)          \
    do                                  \
    {                                   \
        struct cmc_timer *t = &(timer); \
        t->start = clock();             \
                                        \
    } while (0)

#define cmc_timer_stop(timer)           \
    do                                  \
    {                                   \
        struct cmc_timer *t = &(timer); \
        t->stop = clock();              \
                                        \
    } while (0)

#define cmc_timer_calc(timer)                                               \
    do                                                                      \
    {                                                                       \
        struct cmc_timer *t = &(timer);                                     \
        t->result = (double)(t->stop - t->start) * 1000.0 / CLOCKS_PER_SEC; \
                                                                            \
    } while (0)

#endif /* CMC_TIMER_H */
