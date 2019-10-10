/* Utility file to be included in the unit tests */
#ifndef CMC_UNIT_TEST_UTL__
#define CMC_UNIT_TEST_UTL__

#include <stdlib.h>
#include <cmc/hashmap.h>

int cmp(size_t a, size_t b)
{
    return (a > b) - (a < b);
}

size_t hash(size_t a)
{
    a += ~(a << 15);
    a ^= (a >> 10);
    a += (a << 3);
    a ^= (a >> 6);
    a += ~(a << 11);
    a ^= (a >> 16);
    return a;
}

size_t numhash(size_t a)
{
    return a;
}

size_t hashcapminus1(size_t a)
{
    return cmc_hashtable_primes[0] - 1;
}

size_t hashcapminus4(size_t a)
{
    return cmc_hashtable_primes[0] - 1;
}

size_t hash0(size_t a)
{
    return 0;
}

#endif /* CMC_UNIT_TEST_UTL__ */
