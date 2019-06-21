/**
 * queue.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/queue.h"
#include <stdio.h>

QUEUE_GENERATE(q, queue, static, int)

int main(int argc, char const *argv[])
{
    // Initialize queue and add some elements
    queue *q = q_new(10);
    for (int i = 0; i < 10; i++)
        q_enqueue(q, i);

    // Initialize iterator
    queue_iter iter;
    q_iter_init(&iter, q);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (!q_iter_end(&iter))
        {
            printf("C[%2d] = %2d\n", q_iter_index(&iter), q_iter_value(&iter));
            q_iter_next(&iter);
        }
        printf("\n");
        while (!q_iter_start(&iter))
        {
            printf("C[%2d] = %2d\n", q_iter_index(&iter), q_iter_value(&iter));
            q_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (q_iter_to_start(&iter); !q_iter_end(&iter); q_iter_next(&iter))
    {
        printf("Q[%2d] = %2d\n", q_iter_index(&iter), q_iter_value(&iter));
    }

    printf("\n");

    for (q_iter_to_end(&iter); !q_iter_start(&iter); q_iter_prev(&iter))
    {
        printf("Q[%2d] = %2d\n", q_iter_index(&iter), q_iter_value(&iter));
    }

    q_free(q);

    return 0;
}
