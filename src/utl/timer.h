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

typedef struct cmc_timer_s
{
    clock_t start;
    clock_t stop;
    double result;
} cmc_timer;

#define timer_start(timer)       \
    do                           \
    {                            \
        cmc_timer *t = &(timer); \
        t->start = clock();      \
                                 \
    } while (0)

#define timer_stop(timer)       \
    do                          \
    {                           \
        cmc_timer *t = &(timer);\
        t->stop = clock();      \
                                \
    } while (0)

#define timer_calc(timer)                                                   \
    do                                                                      \
    {                                                                       \
        cmc_timer *t = &(timer);                                            \
        t->result = (double)(t->stop - t->start) * 1000.0 / CLOCKS_PER_SEC; \
                                                                            \
    } while (0)

#endif /* CMC_TIMER_H */
