// Checks if the deque iterator can go back and forward in the deque without
// hitting invalid states or a null pointer. Check is done visually.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/deque.h"

DEQUE_GENERATE(d, deque, static, int)

int main(int argc, char const *argv[])
{
    size_t i;
    int r;

    // Initialize deque and add some elements
    deque *d = d_new(10);
    for (int i = 0; i < 10; i++)
    {
        i % 2 == 0 ? d_push_back(d, i) : d_push_front(d, i);
    }

    // Initialize iterator
    deque_iter iter;
    d_iter_new(&iter, d);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (d_iter_next(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n");
        while (d_iter_prev(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n\n");
    }

    for (d_iter_tostart(&iter); !d_iter_end(&iter); /**/)
    {
        d_iter_next(&iter, &r, &i);
        printf("D[%2d] = %2d\n", i, r);
    }

    printf("\n");

    for (d_iter_toend(&iter); !d_iter_start(&iter); /**/)
    {
        d_iter_prev(&iter, &r, &i);
        printf("D[%2d] = %2d\n", i, r);
    }

    d_free(d);

    return 0;
}
