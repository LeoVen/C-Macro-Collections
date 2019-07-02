/* Utility folder to be included in the unit tests */
#ifndef CMC_UNIT_TEST_UTL__
#define CMC_UNIT_TEST_UTL__

#include <stdlib.h>

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

#endif /* CMC_UNIT_TEST_UTL__ */
