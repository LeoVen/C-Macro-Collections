#include <stdio.h>
#include "../src/macro_collections.h"

int intcmp(int a, int b)
{
    return a - b;
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

COLLECTION_GENERATE(TREESET, PUBLIC, set, hash_set, static, , int)

int main(int argc, char const *argv[])
{
    //hash_set *set = set_new(50000, 0.9, intcmp, inthash);
    hash_set *set = set_new(intcmp);

    for (int i = -1000000; i <= 1000000; i++)
        set_insert(set, i);

    hash_set_iter iter;

    size_t index;
    int result, total_sum = 0;
    for (set_iter_new(&iter, set); !set_iter_end(&iter);)
    {
        set_iter_next(&iter, &result, &index);
        total_sum += result;
    }

    set_free(set);

    return 0;
}
