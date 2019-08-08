/**
 * hashmap.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/hashmap.h"
#include <stdio.h>

HASHMAP_GENERATE(hm, hmap, static, int, double)

size_t inthash(int t)
{
    size_t a = t;
    a += ~(a << 15);
    a ^= (a >> 10);
    a += (a << 3);
    a ^= (a >> 6);
    a += ~(a << 11);
    a ^= (a >> 16);
    return a;
}

int intcmp(int a, int b)
{
    return (a > b) - (a < b);
}

int main(int argc, char const *argv[])
{
    double varr[10] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    // Initialize hashmap and add some element
    hmap *hm = hm_new(50, 0.9, intcmp, inthash);
    for (int i = 0; i < 10; i++)
        hm_insert(hm, i, varr[i]);

    // Initialize iterator
    hmap_iter iter;
    hm_iter_init(&iter, hm);

    for (int j = 0; j < 4; j++)
    {
        while (!hm_iter_end(&iter))
        {
            printf("C[%2d] = { %2d : %2.2lf }\n", hm_iter_index(&iter), hm_iter_key(&iter), hm_iter_value(&iter));
            hm_iter_next(&iter);
        }
        printf("\n");
        while (!hm_iter_start(&iter))
        {
            printf("C[%2d] = { %2d : %2.2lf }\n", hm_iter_index(&iter), hm_iter_key(&iter), hm_iter_value(&iter));
            hm_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (hm_iter_to_start(&iter); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        printf("HM[%2d] = { %2d : %2.2lf }\n", hm_iter_index(&iter), hm_iter_key(&iter), hm_iter_value(&iter));
    }

    printf("\n");

    for (hm_iter_to_end(&iter); !hm_iter_start(&iter); hm_iter_prev(&iter))
    {
        printf("HM[%2d] = { %2d : %2.2lf }\n", hm_iter_index(&iter), hm_iter_key(&iter), hm_iter_value(&iter));
    }

    hm_free(hm);

    return 0;
}
