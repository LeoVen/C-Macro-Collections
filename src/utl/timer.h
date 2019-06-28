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

typedef struct timer_s
{
    clock_t start;
    clock_t stop;
    double result;
} timer_t;

#define timer_start(timer)       \
    do                           \
    {                            \
        (timer).start = clock(); \
                                 \
    } while (0)

#define timer_stop(timer)       \
    do                          \
    {                           \
        (timer).stop = clock(); \
                                \
    } while (0)

#define timer_calc(timer)                                                   \
    do                                                                      \
    {                                                                       \
        timer_t *t = &(timer);                                              \
        t->result = (double)(t->stop - t->start) * 1000.0 / CLOCKS_PER_SEC; \
                                                                            \
    } while (0)

#endif /* CMC_TIMER_H */
