/**
 * heapsort.c
 *
 * Creation Date: 25/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include <stdio.h>

#include "cmc/cor/core.h"
#include "cmc/utl/futils.h"

#define PFX h
#define SNAME heap
#define V int
#define CMC_EXT_ITER
#include "cmc/heap.h"

int main()
{
    struct heap *h = h_new(32, CMC_MIN_HEAP, &(struct heap_fval){ .cmp = cmc_i32_cmp });

    for (int i = 1000; i > 0; i--)
    {
        if (!h_insert(h, i))
        {
            printf("Failed to insert: %s", cmc_flags_to_str[h_flag(h)]);
            return h_flag(h);
        }
    }

    size_t S = h_count(h);

    int *buff = malloc(sizeof(int) * S);

    int index = 0;
    while (!h_empty(h))
    {
        int tmp = h_peek(h);

        if (h_flag(h) == CMC_FLAG_OK)
        {
            buff[index++] = tmp;
            h_remove(h);
        }
        else
        {
            printf("Heap peek failed: %s", cmc_flags_to_str[h_flag(h)]);
            return h_flag(h);
        }
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
