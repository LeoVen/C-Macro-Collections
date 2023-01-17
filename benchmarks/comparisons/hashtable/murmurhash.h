// https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/libsupc%2B%2B/hash_bytes.cc

#ifndef MURMURHASH
#define MURMURHASH

#include <string.h>

inline size_t unaligned_load(const char *p)
{
    size_t result;
    memcpy(&result, p, sizeof(result));
    return result;
}

inline size_t load_bytes(const char *p, int n)
{
    size_t result = 0;
    --n;
    do
    {
        result = (result << 8) + (unsigned char)(p[n]);
    } while (--n >= 0);
    return result;
}

inline size_t shift_mix(size_t v)
{
    return v ^ (v >> 47);
}

size_t MurMurHash(const void *ptr, size_t len, size_t seed)
{
    static const size_t mul = (((size_t)0xc6a4a793UL) << 32UL) + (size_t)0x5bd1e995UL;
    const char *const buf = (const char *)(ptr);

    // Remove the bytes not divisible by the sizeof(size_t).  This
    // allows the main loop to process the data as 64-bit integers.
    const size_t len_aligned = len & ~(size_t)0x7;
    const char *const end = buf + len_aligned;
    size_t hash = seed ^ (len * mul);
    for (const char *p = buf; p != end; p += 8)
    {
        const size_t data = shift_mix(unaligned_load(p) * mul) * mul;
        hash ^= data;
        hash *= mul;
    }
    if ((len & 0x7) != 0)
    {
        const size_t data = load_bytes(end, len & 0x7);
        hash ^= data;
        hash *= mul;
    }
    hash = shift_mix(hash) * mul;
    hash = shift_mix(hash);
    return hash;
}

#endif /* MURMURHASH */
