/**
 * queue.c
 *
 * Creation Date: 24/06/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/* Showing off how different queue implementations affect runtime cost */

#include "utl/timer.h"
#include <stdio.h>
#include <inttypes.h>

#define MAX 200000

#if defined(CMC_QUEUE)
#include "cmc/queue.h"
QUEUE_GENERATE(q, queue, , int)
#define TARGET "QUEUE"
#elif defined(SAC_QUEUE)
#include "sac/queue.h"
SAC_QUEUE_GENERATE(q, queue, , int, MAX)
#define TARGET "SAC QUEUE"
#elif defined(EXT_QUEUE)
#include "ext/linkedqueue.h"
LINKEDQUEUE_GENERATE(q, queue, , int)
#define TARGET "LINKED QUEUE"
#else
#error Please define either CMC_QUEUE, SAC_QUEUE or EXT_QUEUE
#endif

int main(void)
{

#if defined(CMC_QUEUE)
    queue *q = q_new(MAX);
#elif defined(SAC_QUEUE)
    queue _q = q_new();
    queue_ptr q = &_q;
#elif defined(EXT_QUEUE)
    queue *q = q_new();
#endif

    size_t sum = 0;
    int i = 0;

    timer_t timer;
    TIMER_START(timer);

    while (q_count(q) != MAX)
    {
        q_enqueue(q, i++);

        if (i % 10 == 0)
        {
            for (size_t i = 0; i < 9; i++)
            {
                sum += q_peek(q);
                q_dequeue(q);
            }
        }

    }

    TIMER_STOP(timer);
    TIMER_CALC(timer);

    printf("----------------------------------------\n");
    printf("%s\n", TARGET);
    printf("Execution time: %.0lf milliseconds\n", timer.result);
    printf("SUM: %" PRIuMAX "\n", sum);
    printf("----------------------------------------\n");

#if defined(CMC_QUEUE) || defined(EXT_QUEUE)
    q_free(q);
#endif

    return 0;
}
