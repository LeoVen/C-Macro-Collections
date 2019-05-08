/**
 * treeset.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "treeset.h"

int intcmp(int a, int b)
{
    return a - b;
}

TREESET_GENERATE(ts, tset, static, int)

int main(int argc, char const *argv[])
{
    size_t i;
    int r;

    // Initialize treeset and add some elements
    tset *ts = ts_new(intcmp);
    for (int i = 0; i < 10; i++)
        ts_insert(ts, i);

    // Initialize iterator
    tset_iter iter;
    ts_iter_init(&iter, ts);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (ts_iter_next(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n");
        while (ts_iter_prev(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n\n");
    }

    for (ts_iter_tostart(&iter); !ts_iter_end(&iter); /**/)
    {
        ts_iter_next(&iter, &r, &i);
        printf("S[%2d] = %2d\n", i, r);
    }

    printf("\n");

    for (ts_iter_toend(&iter); !ts_iter_start(&iter); /**/)
    {
        ts_iter_prev(&iter, &r, &i);
        printf("S[%2d] = %2d\n", i, r);
    }

    ts_free(ts);

    return 0;
}
