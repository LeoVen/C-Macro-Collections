#include "cmc/bitset.h"
#include <math.h>

CMC_CMC_BITSET_CORE(PUBLIC, HEADER, (bs, bitset))
CMC_CMC_BITSET_CORE(PUBLIC, SOURCE, (bs, bitset))

// All the prime numbers up to 1 billion
// With -O3:
//     28.583 s runtime
//     120 MB RAM
//     490 MB file size
int main(void)
{
    const size_t n = 1000000000;

    struct bitset *set = bs_new(n);

    if (!set)
        return 1;

    bs_set_range(set, 0, n);

    size_t up_to = sqrt((double)n) + 1;
    for (size_t i = 2; i < up_to; ++i)
    {
        if (bs_get(set, i))
        {
            for (size_t j = 2 * i; j < n; j += i)
                bs_clear(set, j);
        }
    }

    FILE *file = fopen("primes.txt", "w");

    for (size_t i = 2; i < n; i++)
    {
        if (bs_get(set, i))
            fprintf(file, "%" PRIuMAX " ", i);
    }

    fclose(file);
}
