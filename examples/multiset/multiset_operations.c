/**
 * multiset_operations.c
 *
 * Creation Date: 01/08/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include <ext/multiset.h>
#include <utl/assert.h>
#include <inttypes.h>
#include <stdio.h>

MULTISET_GENERATE(ms, multiset, , char)

struct pair
{
    char value;
    size_t mul;
};

int charcmp(char a, char b)
{
    return (a > b) - (a < b);
}

size_t charhash(char a)
{
    return a;
}

void print_set(multiset *M)
{
    cmc_assert_not_equals(ptr, NULL, M);

    size_t S = ms_count(M);

    printf("%s : ", ms_to_string(M).s);

    if (S == 0)
    {
        printf("{ EMPTY SET }\n");
        return;
    }

    for (multiset_iter it = M->it_start(M); !ms_iter_end(&it); ms_iter_next(&it))
    {
        size_t index = ms_iter_index(&it);
        char value = ms_iter_value(&it);
        size_t mul = ms_iter_multiplicity(&it);

        if (index == 0)
            printf("{ %c:%" PRIuMAX ", ", value, mul);
        else if (index == S - 1)
            printf("%c:%" PRIuMAX " }\n", value, mul);
        else
            printf("%c:%" PRIuMAX ", ", value, mul);
    }

}

int main(int argc, char const *argv[])
{
    struct pair mset1[4] = {{'a', 2}, {'b', 1}, {'c', 3}, {'d', 1}};
    struct pair mset2[4] = {{'c', 1}, {'d', 3}, {'e', 1}, {'f', 2}};

    multiset *A = ms_new(20, 0.6, charcmp, charhash);
    multiset *B = ms_new(20, 0.6, charcmp, charhash);

    for (int i = 0; i < 4; i++)
    {
        cmc_assert(ms_insert_many(A, mset1[i].value, mset1[i].mul));
        cmc_assert(ms_insert_many(B, mset2[i].value, mset2[i].mul));
    }

    printf("Given two sets A and B:\n");
    print_set(A);
    print_set(B);

    multiset *U = ms_union(A, B);
    printf("\nA UNION B:\n");
    print_set(U);

    multiset *I = ms_intersection(A, B);
    printf("\nA INTERSECTION B:\n");
    print_set(I);

    multiset *DA = ms_difference(A, B);
    multiset *DB = ms_difference(B, A);
    printf("\nA - B:\n");
    print_set(DA);
    printf("\nB - A:\n");
    print_set(DB);

    multiset *AB = ms_summation(A, B);
    printf("\nA + B:\n");
    print_set(AB);

    multiset *BA = ms_summation(B, A);
    printf("\nB + A:\n");
    print_set(BA);

    multiset *SD = ms_symmetric_difference(A, B);
    printf("\nA SYM DIFF B:\n");
    print_set(SD);

    ms_free(A);
    ms_free(B);
    ms_free(U);
    ms_free(I);
    ms_free(DA);
    ms_free(DB);
    ms_free(AB);
    ms_free(BA);
    ms_free(SD);

    return 0;
}
