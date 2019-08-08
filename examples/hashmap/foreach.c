/**
 * iterator.c
 *
 * Creation Date: 23/05/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Foreach macro example
 */
#include "cmc/hashmap.h"
#include "utl/foreach.h"
#include <stdio.h>

// Int hash function
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

// Int compare function
int intcmp(int a, int b)
{
    return (a > b) - (a < b);
}

// Generate all the code for the hashmap
HASHMAP_GENERATE(hm, hashmap, /* FMOD */, int, double)

int main(void)
{
    // Create a new hashmap with custom hash function
    // The comparison function is only required to know if the key looked for was found
    hashmap *map = hm_new(1000, 0.6, intcmp, inthash);

    // Add keys mapped to a value
    for (int i = 0; i < 5000; i++)
    {
        hm_insert(map, i, (double)i / 1000.0);
    }

    // Iterate over the HashMap without the FOR_EACH macro
    for (hashmap_iter iter = map->it_start(map); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        printf("MAP[%4d] = %.3lf\n", hm_iter_key(&iter), hm_iter_value(&iter));
    }

    // Iterate over the HashMap using FOR_EACH macro
    FOR_EACH(hm, hashmap, map, {
        // Inside, the variable 'iter' will be available and from it you can access
        // the key, value or index.
        printf("MAP[%4d] = %.3lf\n", hm_iter_key(&iter), hm_iter_value(&iter));
    });

    hm_free(map);

    return 0;
}
