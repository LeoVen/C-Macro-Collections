/**
 * heap.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/heap.h"
#include <stdio.h>

int intcmp(int a, int b)
{
    return (a > b) - (a < b);
}

HEAP_GENERATE(h, heap, static, int)

int main(int argc, char const *argv[])
{
    // Initialize heap and add some elements
    heap *h = h_new(10, MaxHeap, intcmp);
    for (int i = 0; i < 10; i++)
        h_insert(h, i);

    // Initialize iterator
    heap_iter iter;
    h_iter_init(&iter, h);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (!h_iter_end(&iter))
        {
            printf("C[%2d] = %2d\n", h_iter_index(&iter), h_iter_value(&iter));
            h_iter_next(&iter);
        }
        printf("\n");
        while (!h_iter_start(&iter))
        {
            printf("C[%2d] = %2d\n", h_iter_index(&iter), h_iter_value(&iter));
            h_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (h_iter_to_start(&iter); !h_iter_end(&iter); h_iter_next(&iter))
    {
        printf("H[%2d] = %2d\n", h_iter_index(&iter), h_iter_value(&iter));
    }

    printf("\n");

    for (h_iter_to_end(&iter); !h_iter_start(&iter); h_iter_prev(&iter))
    {
        printf("H[%2d] = %2d\n", h_iter_index(&iter), h_iter_value(&iter));
    }

    h_free(h);

    return 0;
}
