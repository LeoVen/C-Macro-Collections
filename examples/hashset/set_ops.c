/**
 * set_ops.c
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

// Demonstration of set operations like:
// - Union
// - Intersection
// - Difference
// - Symmetric Difference

#include "cmc/hashset.h"
#include <stdio.h>
#include <stdlib.h>

HASHSET_GENERATE(set, set, /* static */, int)

void print_set(set *s)
{
    set_iter iter;
    set_iter_init(&iter, s);
    for (set_iter_to_start(&iter); !set_iter_end(&iter); set_iter_next(&iter))
    {
        int result = set_iter_value(&iter);
        size_t index = set_iter_index(&iter);

        if (index == 0)
            printf("[ %2d, ", result);
        else if (index == s->count - 1)
            printf("%2d ]\n", result);
        else
            printf("%2d, ", result);
    }
}

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

int main(int argc, char const *argv[])
{
    set *set1 = set_new(50, 0.9, intcmp, inthash);
    set *set2 = set_new(50, 0.9, intcmp, inthash);

    for (int i = 1; i <= 20; i++)
        set_insert(set1, i);

    for (int i = 11; i <= 30; i++)
        set_insert(set2, i);

    set *set3 = set_union(set1, set2);

    print_set(set1);
    printf("UNION\n");
    print_set(set2);
    printf("=\n");
    print_set(set3);

    printf("\n\n");

    set *set4 = set_intersection(set1, set2);

    print_set(set1);
    printf("INTERSECTION\n");
    print_set(set2);
    printf("=\n");
    print_set(set4);

    printf("\n\n");

    set *set5 = set_difference(set1, set2);

    print_set(set1);
    printf("DIFFERENCE\n");
    print_set(set2);
    printf("=\n");
    print_set(set5);

    printf("\n\n");

    set *set6 = set_symmetric_difference(set1, set2);

    print_set(set1);
    printf("SYMMETRIC DIFFERENCE\n");
    print_set(set2);
    printf("=\n");
    print_set(set6);

    set_free(set1);
    set_free(set2);
    set_free(set3);
    set_free(set4);
    set_free(set5);
    set_free(set6);

    return 0;
}
