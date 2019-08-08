/**
 * heapsort.c
 *
 * Creation Date: 25/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/heap.h"
#include <stdio.h>

static int intcmp(int a, int b)
{
    return a - b;
}

HEAP_GENERATE(h, heap, static, int)

int main(int argc, char const *argv[])
{
    heap *h = h_new(32, cmc_max_heap, intcmp);

    for (int i = 1; i < 101; i++)
    {
        h_insert(h, i);
    }

    size_t S = h_count(h);

    int *buff = malloc(sizeof(int) * S);

    int index = 0;
    while (!h_empty(h))
    {
        int tmp;
        h_remove(h, &tmp);
        buff[index++] = tmp;
    }

    for (size_t i = 0; i < S; i++)
    {
        if (i == 0)
            printf("[ %d, ", buff[i]);
        else if (i == S - 1)
            printf("%d ]\n", buff[i]);
        else
            printf("%d, ", buff[i]);
    }

    free(buff);
    h_free(h);

    return 0;
}
