// Checks if the hashmap iterator can go back and forward in the hashmap without
// hitting invalid states or a null pointer. Check is done visually.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/hashmap.h"

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
    return a - b;
}

int main(int argc, char const *argv[])
{
    size_t i;
    int k;
    double v, varr[10] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    // Initialize hashmap and add some element
    hmap *hm = hm_new(50, 0.9, intcmp, inthash);
    for (int i = 0; i < 10; i++)
        hm_insert(hm, i, varr[i]);

    // Initialize iterator
    hmap_iter iter;
    hm_iter_new(&iter, hm);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (hm_iter_next(&iter, &k, &v, &i))
            printf("S[%2d] = { %2d : %2.2lf }\n", i, k, v);
        printf("\n");
        while (hm_iter_prev(&iter, &k, &v, &i))
            printf("S[%2d] = { %2d : %2.2lf }\n", i, k, v);
        printf("\n\n");
    }

    for (hm_iter_tostart(&iter); !hm_iter_end(&iter); /**/)
    {
        hm_iter_next(&iter, &k, &v, &i);
        printf("M[%2d] = { %2d : %2.2lf }\n", i, k, v);
    }

    printf("\n");

    for (hm_iter_toend(&iter); !hm_iter_start(&iter); /**/)
    {
        hm_iter_prev(&iter, &k, &v, &i);
        printf("M[%2d] = { %2d : %2.2lf }\n", i, k, v);
    }

    hm_free(hm);

    return 0;
}
