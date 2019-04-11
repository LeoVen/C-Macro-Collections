/**
 * heap.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

int intcmp(int a, int b)
{
    return a - b;
}

HEAP_GENERATE(h, heap, static, int)

int main(int argc, char const *argv[])
{
    size_t i;
    int r;

    // Initialize heap and add some elements
    heap *h = h_new(10, MinHeap, intcmp);
    for (int i = 9; i >= 0; i--)
        h_insert(h, i);

    // Initialize iterator
    heap_iter iter;
    h_iter_new(&iter, h);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (h_iter_next(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n");
        while (h_iter_prev(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n\n");
    }

    for (h_iter_tostart(&iter); !h_iter_end(&iter); /**/)
    {
        h_iter_next(&iter, &r, &i);
        printf("H[%2d] = %2d\n", i, r);
    }

    printf("\n");

    for (h_iter_toend(&iter); !h_iter_start(&iter); /**/)
    {
        h_iter_prev(&iter, &r, &i);
        printf("H[%2d] = %2d\n", i, r);
    }

    h_free(h);

    return 0;
}
