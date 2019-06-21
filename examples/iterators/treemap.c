/**
 * treemap.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/treemap.h"
#include <stdio.h>

int intcmp(int a, int b)
{
    return (a > b) - (a < b);
}

TREEMAP_GENERATE(tm, tmap, static, int, double)

int main(int argc, char const *argv[])
{
    double varr[10] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    // Initialize treemap and add some element
    tmap *hm = tm_new(intcmp);
    for (int i = 0; i < 10; i++)
        tm_insert(hm, i, varr[i]);

    // Initialize iterator
    tmap_iter iter;
    tm_iter_init(&iter, hm);

    for (int j = 0; j < 4; j++)
    {
        while (!tm_iter_end(&iter))
        {
            printf("C[%2d] = { %2d : %2.2lf }\n", tm_iter_index(&iter), tm_iter_key(&iter), tm_iter_value(&iter));
            tm_iter_next(&iter);
        }
        printf("\n");
        while (!tm_iter_start(&iter))
        {
            printf("C[%2d] = { %2d : %2.2lf }\n", tm_iter_index(&iter), tm_iter_key(&iter), tm_iter_value(&iter));
            tm_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (tm_iter_to_start(&iter); !tm_iter_end(&iter); tm_iter_next(&iter))
    {
        printf("TM[%2d] = { %2d : %2.2lf }\n", tm_iter_index(&iter), tm_iter_key(&iter), tm_iter_value(&iter));
    }

    printf("\n");

    for (tm_iter_to_end(&iter); !tm_iter_start(&iter); tm_iter_prev(&iter))
    {
        printf("TM[%2d] = { %2d : %2.2lf }\n", tm_iter_index(&iter), tm_iter_key(&iter), tm_iter_value(&iter));
    }

    tm_free(hm);

    return 0;
}
