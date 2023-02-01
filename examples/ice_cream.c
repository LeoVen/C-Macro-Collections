/**
 * ice_cream.c
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PFX q
#define SNAME queue
#define V const char *
#define CMC_EXT_ITER
#include "cmc/queue.h"

int main()
{
    srand((unsigned)time(NULL));

    // 1- People are lining up to get some ice cream but there is not enough
    //    for everyone! Hurry! Get in the queue!

    size_t total_icecreams = 20;

    struct queue *lineup = q_new(10, &(struct queue_fval){ 0 });

    bool ok = true;

    ok = ok && q_enqueue(lineup, "Albert");
    ok = ok && q_enqueue(lineup, "Newton");
    ok = ok && q_enqueue(lineup, "Marie");
    ok = ok && q_enqueue(lineup, "Charles");
    ok = ok && q_enqueue(lineup, "Nikola");
    ok = ok && q_enqueue(lineup, "Thomas");
    ok = ok && q_enqueue(lineup, "Niels");
    ok = ok && q_enqueue(lineup, "Galileo");
    ok = ok && q_enqueue(lineup, "Stephen");
    ok = ok && q_enqueue(lineup, "Leonardo");
    ok = ok && q_enqueue(lineup, "Max");
    ok = ok && q_enqueue(lineup, "Dmitri");

    if (!ok)
    {
        fprintf(stderr, "Something went wrong!");
        return 1;
    }

    while (total_icecreams > 0)
    {
        size_t catch = rand() % 5;

        const char *name = q_peek(lineup);

        if (!q_dequeue(lineup))
            break;

        printf("%-8s got %zu ice cream(s) from the ice cream truck\n", name, catch);

        total_icecreams = catch > total_icecreams ? 0 : total_icecreams - catch;
    }

    if (!q_empty(lineup))
    {
        printf("\nOh no! Someone didn't get their ice cream!\n\n");

        while (!q_empty(lineup))
        {
            const char *name = q_peek(lineup);

            q_dequeue(lineup);

            printf("%-8s got no ice cream and he is sad\n", name);
        }
    }
    else if (total_icecreams > 0)
    {
        printf("\n%zu ice creams remaining to be sold on the next day.\n", total_icecreams);
    }

    q_free(lineup);

    return 0;
}
