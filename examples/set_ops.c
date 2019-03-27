#include <stdio.h>
#include <stdlib.h>
#include "../src/macro_collections.h"

// change to hashset to have the same effects
COLLECTION_GENERATE(TREESET, PUBLIC, set, set, , , int)

void print_set(set *s)
{
    size_t index;
    int result;
    set_iter iter;
    set_iter_new(&iter, s);
    for (set_iter_tostart(&iter); !set_iter_end(&iter);)
    {
        set_iter_next(&iter, &result, &index);
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

int main(int argc, char const *argv[])
{
    set *set1 = set_new(intcmp);
    set *set2 = set_new(intcmp);

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
