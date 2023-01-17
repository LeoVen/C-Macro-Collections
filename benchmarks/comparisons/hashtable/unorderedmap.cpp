#ifndef COUNT
#error "Please define COUNT"
#endif

#include "murmurhash.h"

#include <unordered_map>

#include "../../util/twister.c"

struct IntHasher
{
    std::size_t operator()(const uint64_t &k) const
    {
        static size_t seed = 211106232532969;
        return MurMurHash(&k, sizeof(k), seed);
    }
};

int main(void)
{
    mt_state tw;
    twist_init(&tw, 50331653);

    std::unordered_map<uint64_t, uint64_t, IntHasher> map(COUNT);

    for (uint64_t i = 0; i < COUNT; i++)
    {
        uint64_t v = twist_uint64(&tw);
        map.insert({ v, v });
    }

    return 0;
}
