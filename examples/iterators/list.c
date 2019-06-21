/**
 * list.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/list.h"
#include <stdio.h>

LIST_GENERATE(l, list, static, int)

int main(int argc, char const *argv[])
{
    // Initialize list and add some elements
    list *l = l_new(10);
    for (int i = 0; i < 10; i++)
        l_push_back(l, i);

    // Initialize iterator
    list_iter iter;
    l_iter_init(&iter, l);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (!l_iter_end(&iter))
        {
            printf("C[%2d] = %2d\n", l_iter_index(&iter), l_iter_value(&iter));
            l_iter_next(&iter);
        }
        printf("\n");
        while (!l_iter_start(&iter))
        {
            printf("C[%2d] = %2d\n", l_iter_index(&iter), l_iter_value(&iter));
            l_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (l_iter_to_start(&iter); !l_iter_end(&iter); l_iter_next(&iter))
    {
        printf("L[%2d] = %2d\n", l_iter_index(&iter), l_iter_value(&iter));
    }

    printf("\n");

    for (l_iter_to_end(&iter); !l_iter_start(&iter); l_iter_prev(&iter))
    {
        printf("L[%2d] = %2d\n", l_iter_index(&iter), l_iter_value(&iter));
    }

    l_free(l);

    return 0;
}
