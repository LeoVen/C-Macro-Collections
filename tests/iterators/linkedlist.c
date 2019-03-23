// Checks if the linked list iterator can go back and forward in the list
// without hitting invalid states or a null pointer. Check is done visually.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/macro_collections.h"

CONTAINER_GENERATE(LINKEDLIST, PUBLIC, ll, linked, static, , int)

int main(int argc, char const *argv[])
{
    size_t i;
    int r;

    // Initliazie linked list and add some elements
    linked *ll = ll_new();
    for (int i = 0; i < 10; i++)
        ll_push_back(ll, i);

    // Initialize iterator
    linked_iter iter;
    ll_iter(&iter, ll);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (ll_iter_next(&iter, &r, &i))
            printf("list[%2d] = %2d\n", i, r);
        printf("\n");
        while (ll_iter_prev(&iter, &r, &i))
            printf("list[%2d] = %2d\n", i, r);
        printf("\n\n");
    }

    ll_free(ll);

    return 0;
}
