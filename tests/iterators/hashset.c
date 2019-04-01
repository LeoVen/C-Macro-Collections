// Checks if the hash set iterator can go back and forward in the hash set without
// hitting invalid states or a null pointer. Check is done visually.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/hashset.h"

HASHSET_GENERATE(hs, hash_set, static, int)

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
    int r;

    // Initliazie hash set and add some elements
    hash_set *hs = hs_new(10, 0.9, intcmp, inthash);
    for (int i = 0; i < 10; i++)
        hs_insert(hs, i);

    // Initialize iterator
    hash_set_iter iter;
    hs_iter_new(&iter, hs);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (hs_iter_next(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n");
        while (hs_iter_prev(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n\n");
    }

    for (hs_iter_tostart(&iter); !hs_iter_end(&iter); /**/)
    {
        hs_iter_next(&iter, &r, &i);
        printf("S[%2d] = %2d\n", i, r);
    }

    printf("\n");

    for (hs_iter_toend(&iter); !hs_iter_start(&iter); /**/)
    {
        hs_iter_prev(&iter, &r, &i);
        printf("S[%2d] = %2d\n", i, r);
    }

    hs_free(hs);

    return 0;
}
