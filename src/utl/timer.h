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

#define TIMER_START(timer) \
    timer.start = clock();

#define TIMER_STOP(timer) \
    timer.stop = clock();

#define TIMER_CALC(timer) \
    timer.result = (double)(((timer.stop - timer.start) * 1000.0) / CLOCKS_PER_SEC);

#endif /* CMC_TIMER_H */
