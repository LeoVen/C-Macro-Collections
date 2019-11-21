#include "cmc/multimap.h"
#include "utl/foreach.h"
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

CMC_GENERATE_MULTIMAP(mm, multi_map, int, int);
typedef struct multi_map multi_map;
typedef struct multi_map_iter multi_map_iter;


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

    CMC_FOR_EACH(mm, multi_map, map, {
        printf(
            "[%3" PRIu64 "] = { %3d : %3d }\n",
            iter.index,
            iter.curr_entry->key,
            iter.curr_entry->value
        );
    });

    printf("Removing all items\n");

    for (int i = min; i < max; i++)
        if (!mm_remove_all(map, i, NULL))
            printf("ERROR with %d:%d\n", i, i);

    sum = 0;

    for (int i = min; i < max; i++)
        sum += mm_get(map, i);

    printf("Sum: %" PRId64 "\n", sum);

    CMC_FOR_EACH(mm, multi_map, map, {
        printf("[%2" PRIu64 "] = { %2d : %5d }\n",
            iter.index,
            iter.curr_entry->key,
            iter.curr_entry->value
        );
    });

    mm_free(map, NULL);

    return 0;
}
