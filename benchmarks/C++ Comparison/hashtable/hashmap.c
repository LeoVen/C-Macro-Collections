#ifndef COUNT
#error "Please define COUNT"
#endif

#include "murmurhash.h"

#include "macro_collections.h"

#include "../../util/twister.c"

CMC_CMC_HASHMAP_CORE(PUBLIC, HEADER, (hm, hashmap, , uint64_t, uint64_t))
CMC_CMC_HASHMAP_CORE(PUBLIC, SOURCE, (hm, hashmap, , uint64_t, uint64_t))

int cmp(uint64_t a, uint64_t b)
{
    return (a > b) - (a < b);
}

// size_t hash(uint64_t a)
// {
//     size_t x = (size_t)a;
//     x = ((x >> 16) ^ x) * 0x45d9f3b;
//     x = ((x >> 16) ^ x) * 0x45d9f3b;
//     x = (x >> 16) ^ x;
//     return x;
// }

size_t hash(uint64_t k)
{
    static size_t seed = 211106232532969;
    return MurMurHash(&k, sizeof(k), seed);
}

struct hashmap_fkey *fkey = &(struct hashmap_fkey) {
    .cmp = cmp,
    .hash = hash,
    NULL
};

struct hashmap_fval *fval = &(struct hashmap_fval) {
    .cmp = cmp,
    .hash = hash,
    NULL
};

int main(void)
{
    mt_state tw;
    twist_init(&tw, 50331653);

    struct hashmap *map = hm_new(COUNT, 0.6, fkey, fval);

    for (uint64_t i = 0; i < COUNT; i++)
    {
        uint64_t v = twist_uint64(&tw);
        hm_insert(map, v, v);
    }

    hm_free(map);

    return 0;
}
