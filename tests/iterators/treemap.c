// Checks if the treemap iterator can go back and forward in the treemap without
// hitting invalid states or a null pointer. Check is done visually.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/treemap.h"

int intcmp(int a, int b)
{
    return a - b;
}

TREEMAP_GENERATE(tm, tmap, static, int, double)

int main(int argc, char const *argv[])
{
    size_t i;
    int k;
    double v, varr[10] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    // Initliazie treemap and add some elementm
    tmap *tm = tm_new(intcmp);
    for (int i = 0; i < 10; i++)
        tm_insert(tm, i, varr[i]);

    // Initialize iterator
    tmap_iter iter;
    tm_iter_new(&iter, tm);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (tm_iter_next(&iter, &k, &v, &i))
            printf("S[%2d] = { %2d : %2.2lf }\n", i, k, v);
        printf("\n");
        while (tm_iter_prev(&iter, &k, &v, &i))
            printf("S[%2d] = { %2d : %2.2lf }\n", i, k, v);
        printf("\n\n");
    }

    for (tm_iter_tostart(&iter); !tm_iter_end(&iter); /**/)
    {
        tm_iter_next(&iter, &k, &v, &i);
        printf("M[%2d] = { %2d : %2.2lf }\n", i, k, v);
    }

    printf("\n");

    for (tm_iter_toend(&iter); !tm_iter_start(&iter); /**/)
    {
        tm_iter_prev(&iter, &k, &v, &i);
        printf("M[%2d] = { %2d : %2.2lf }\n", i, k, v);
    }

    tm_free(tm);

    return 0;
}
