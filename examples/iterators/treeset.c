/**
 * treeset.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/treeset.h"
#include <stdio.h>

int intcmp(int a, int b)
{
    return (a > b) - (a < b);
}

TREESET_GENERATE(ts, tset, static, int)

int main(int argc, char const *argv[])
{
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
        while (!ts_iter_end(&iter))
        {
            printf("C[%2d] = %2d\n", ts_iter_index(&iter), ts_iter_value(&iter));
            ts_iter_next(&iter);
        }
        printf("\n");
        while (!ts_iter_start(&iter))
        {
            printf("C[%2d] = %2d\n", ts_iter_index(&iter), ts_iter_value(&iter));
            ts_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        printf("TS[%2d] = %2d\n", ts_iter_index(&iter), ts_iter_value(&iter));
    }

    printf("\n");

    for (ts_iter_to_end(&iter); !ts_iter_start(&iter); ts_iter_prev(&iter))
    {
        printf("TS[%2d] = %2d\n", ts_iter_index(&iter), ts_iter_value(&iter));
    }

    ts_free(ts);

    return 0;
}
