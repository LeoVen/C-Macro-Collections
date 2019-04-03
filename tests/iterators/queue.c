// Checks if the queue iterator can go back and forward in the queue without
// hitting invalid states or a null pointer. Check is done visually.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/queue.h"

QUEUE_GENERATE(q, queue, static, int)

int main(int argc, char const *argv[])
{
    size_t i;
    int r;

    // Initialize queue and add some elements
    queue *q = q_new(10);
    for (int i = 0; i < 10; i++)
        q_enqueue(q, i);

    // Initialize iterator
    queue_iter iter;
    q_iter_new(&iter, q);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (q_iter_next(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n");
        while (q_iter_prev(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n\n");
    }

    for (q_iter_tostart(&iter); !q_iter_end(&iter); /**/)
    {
        q_iter_next(&iter, &r, &i);
        printf("Q[%2d] = %2d\n", i, r);
    }

    printf("\n");

    for (q_iter_toend(&iter); !q_iter_start(&iter); /**/)
    {
        q_iter_prev(&iter, &r, &i);
        printf("Q[%2d] = %2d\n", i, r);
    }

    q_free(q);

    return 0;
}
