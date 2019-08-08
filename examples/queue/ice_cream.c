/**
 * ice_cream.c
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

QUEUE_GENERATE(q, queue, /* static */, const char *)

int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));

    // 1- People are lining up to get some ice cream but there is not enough
    //    for everyone! Hurry! Get in the queue!

    size_t total_icecreams = 20;

    queue *lineup = q_new(10);

    q_enqueue(lineup, "Albert");
    q_enqueue(lineup, "Newton");
    q_enqueue(lineup, "Marie");
    q_enqueue(lineup, "Charles");
    q_enqueue(lineup, "Nikola");
    q_enqueue(lineup, "Thomas");
    q_enqueue(lineup, "Niels");
    q_enqueue(lineup, "Galileo");
    q_enqueue(lineup, "Stephen");
    q_enqueue(lineup, "Leonardo");
    q_enqueue(lineup, "Max");
    q_enqueue(lineup, "Dmitri");

    while (total_icecreams > 0)
    {
        size_t catch = rand() % 5;

        const char *name = q_peek(lineup);

        if (!q_dequeue(lineup))
            break;

        printf("%-8s got %lu ice cream(s) from the ice cream truck\n", name, catch);

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
        printf("\n%lu ice creams remaining to be sold on the next day.\n");
    }

    q_free(lineup);

    return 0;
}
