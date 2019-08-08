/**
 * deque.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/deque.h"
#include <stdio.h>

DEQUE_GENERATE(d, deque, static, int)

int main(int argc, char const *argv[])
{
    // Initialize deque and add some elements
    deque *d = d_new(10);
    for (int i = 0; i < 10; i++)
    {
        i % 2 == 0 ? d_push_back(d, i) : d_push_front(d, i);
    }

    // Initialize iterator
    deque_iter iter;
    d_iter_init(&iter, d);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (!d_iter_end(&iter))
        {
            printf("C[%2d] = %2d\n", d_iter_index(&iter), d_iter_value(&iter));
            d_iter_next(&iter);
        }
        printf("\n");
        while (!d_iter_start(&iter))
        {
            printf("C[%2d] = %2d\n", d_iter_index(&iter), d_iter_value(&iter));
            d_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (d_iter_to_start(&iter); !d_iter_end(&iter); d_iter_next(&iter))
    {
        printf("D[%2d] = %2d\n", d_iter_index(&iter), d_iter_value(&iter));
    }

    printf("\n");

    for (d_iter_to_end(&iter); !d_iter_start(&iter); d_iter_prev(&iter))
    {
        printf("D[%2d] = %2d\n", d_iter_index(&iter), d_iter_value(&iter));
    }

    d_free(d);

    return 0;
}
