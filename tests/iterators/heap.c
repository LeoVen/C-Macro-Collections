// Checks if the heap iterator can go back and forward in the heap without
// hitting invalid states or a null pointer. Check is done visually.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/heap.h"

int intcmp(int a, int b)
{
    return a - b;
}

HEAP_GENERATE(h, heap, static, , int)

int main(int argc, char const *argv[])
{
    size_t i;
    int r;

    // Initliazie heap and add some elements
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
        printf("L[%2d] = %2d\n", i, r);
    }

    printf("\n");

    for (h_iter_toend(&iter); !h_iter_start(&iter); /**/)
    {
        h_iter_prev(&iter, &r, &i);
        printf("L[%2d] = %2d\n", i, r);
    }

    h_free(h);

    return 0;
}
