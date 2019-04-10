#include <stdio.h>
#include "multiset.h"

MULTISET_GENERATE(ms, multi_set, /* FMOD */, size_t)

int cmp(size_t a, size_t b)
{
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    return 0;
}

void prime_cof(const size_t number)
{
    multi_set *mset = ms_new(cmp);

    size_t num = number;

    for (size_t i = 2; num != 1; i++)
    {
        while (num % i == 0)
        {
            ms_insert(mset, i);
            num /= i;
        }
    }

    printf("The factorization of %llu is given by:\n", number);

    multi_set_iter iter;
    size_t result, mul, index;
    for (ms_iter_new(&iter, mset); !ms_iter_end(&iter);)
    {
        if (!ms_iter_next(&iter, &result, &mul, &index))
        {
            printf("ERROR\n");
            break;
        }

        if (ms_count(mset) == 1)
            printf("(%llu^%llu)\n", result, mul);
        else if (index == 0)
            printf("(%llu^%llu ", result, mul);
        else if (index == ms_count(mset) - 1)
            printf("* %llu^%llu)\n", result, mul);
        else
            printf("* %llu^%llu ", result, mul);
    }

    printf("Set Cardinality: %llu\n\n", ms_cardinality(mset));

    ms_free(mset);
}

int main(int argc, char const *argv[])
{
    prime_cof(360);
    prime_cof(55777680);
    prime_cof(4294967296);
    prime_cof(31415926535);
    prime_cof(27182818284);
    prime_cof(16180339887);

    return 0;
}
