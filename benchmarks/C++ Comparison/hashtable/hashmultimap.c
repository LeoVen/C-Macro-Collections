#ifndef COUNT
#error "Please define COUNT"
#endif

#include "murmurhash.h"

#include "macro_collections.h"

#include "../../util/twister.c"

C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm, hashmultimap, , uint64_t, uint64_t))

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

struct hashmultimap_fkey *fkey = &(struct hashmultimap_fkey) {
    .cmp = cmp,
    .hash = hash,
    NULL
};

struct hashmultimap_fval *fval = &(struct hashmultimap_fval) {
    .cmp = cmp,
    .hash = hash,
    NULL
};

int main(void)
{
    mt_state tw;
    twist_init(&tw, 50331653);

    struct hashmultimap *map = hmm_new(COUNT, 0.6, fkey, fval);

    for (uint64_t i = 0; i < COUNT; i++)
    {
        uint64_t v = twist_uint64(&tw);
        hmm_insert(map, v, v);
    }

    hmm_free(map);

    return 0;
}
