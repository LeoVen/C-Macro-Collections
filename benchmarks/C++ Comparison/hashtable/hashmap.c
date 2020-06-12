#ifndef COUNT
#error "Please define COUNT"
#endif

#include "cmc/hashmap.h"

CMC_CMC_HASHMAP_CORE(PUBLIC, HEADER, (hmi32, hashmapi32, , int32_t, int32_t))
CMC_CMC_HASHMAP_CORE(PUBLIC, SOURCE, (hmi32, hashmapi32, , int32_t, int32_t))

int cmp(int32_t a, int32_t b)
{
    return (a > b) - (a < b);
}

size_t hash(int32_t a)
{
    size_t x = (size_t)a;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

struct hashmapi32_fkey *fkey = &(struct hashmapi32_fkey) {
    .cmp = cmp,
    .hash = hash,
    NULL
};

struct hashmapi32_fval *fval = &(struct hashmapi32_fval) {
    .cmp = cmp,
    .hash = hash,
    NULL
};

int main(void)
{
    struct hashmapi32 *map = hmi32_new(COUNT, 0.6, fkey, fval);

    for (int32_t i = 0; i < COUNT; i++)
        hmi32_insert(map, i, i);

    hmi32_free(map);

    return 0;
}
