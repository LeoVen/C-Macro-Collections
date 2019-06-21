/**
 * linkedlist.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/linkedlist.h"
#include <stdio.h>

LINKEDLIST_GENERATE(ll, linkedlist, static, int)

int main(int argc, char const *argv[])
{
    // Initialize linkedlist and add some elements
    linkedlist *ll = ll_new();
    for (int i = 0; i < 10; i++)
        ll_push_back(ll, i);

    // Initialize iterator
    linkedlist_iter iter;
    ll_iter_init(&iter, ll);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (!ll_iter_end(&iter))
        {
            printf("C[%2d] = %2d\n", ll_iter_index(&iter), ll_iter_value(&iter));
            ll_iter_next(&iter);
        }
        printf("\n");
        while (!ll_iter_start(&iter))
        {
            printf("C[%2d] = %2d\n", ll_iter_index(&iter), ll_iter_value(&iter));
            ll_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (ll_iter_to_start(&iter); !ll_iter_end(&iter); ll_iter_next(&iter))
    {
        printf("LL[%2d] = %2d\n", ll_iter_index(&iter), ll_iter_value(&iter));
    }

    printf("\n");

    for (ll_iter_to_end(&iter); !ll_iter_start(&iter); ll_iter_prev(&iter))
    {
        printf("LL[%2d] = %2d\n", ll_iter_index(&iter), ll_iter_value(&iter));
    }

    ll_free(ll);

    return 0;
}
