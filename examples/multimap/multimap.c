#include "multimap.h"
#include "foreach.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int intcmp(int a, int b)
{
    return (a > b) - (a < b);
}

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

MULTIMAP_GENERATE(mm, multi_map, , int, int)

int main(int argc, char const *argv[])
{
    const int min = 1, max = 201;
    multi_map *map = mm_new(50, 0.8, intcmp, inthash);

    printf("Multimap actual capacity: %" PRIu64 "\n", map->capacity);
    printf("Adding items from %d to %d twice\n", min, max);

    for (int i = min; i < max; i++)
        if (!mm_insert(map, i, i))
            printf("ERROR with %d:%d\n", i, i);

    for (int i = min; i < max; i++)
        if (!mm_insert(map, i, i))
            printf("ERROR with %d:%d\n", i, i);

    printf("Multimap count    : %" PRIu64 "\n", map->count);
    printf("Multimap capacity : %" PRIu64 "\n", map->capacity);
    printf("Multimap load     : %lf\n", (double)map->count / (double)map->capacity);

    int64_t sum = 0;

    for (int i = min; i < max; i++)
        sum += mm_get(map, i);

    printf("Sum (unique): %" PRId64 "\n", sum);

    FOR_EACH_MAP(mm, multi_map, int, int, map, {
        printf("[%3" PRIu64 "] = { %3d : %3d }\n", index, key, value);
    })

    printf("Removing all items\n");

    for (int i = min; i < max; i++)
        if (!mm_remove_all(map, i))
            printf("ERROR with %d:%d\n", i, i);

    sum = 0;

    for (int i = min; i < max; i++)
        sum += mm_get(map, i);

    printf("Sum: %" PRId64 "\n", sum);

    FOR_EACH_MAP(mm, multi_map, int, int, map, {
        printf("[%2" PRIu64 "] = { %2d : %5d }\n", index, key, value);
    })

    mm_free(map);

    return 0;
}
