// Checks if the list iterator can go back and forward in the list without
// hitting invalid states or a null pointer. Check is done visually.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../src/list.h"

LIST_GENERATE(l, list, static, , int)

int main(int argc, char const *argv[])
{
    size_t i;
    int r;

    // Initliazie list and add some elements
    list *l = l_new(10);
    for (int i = 0; i < 10; i++)
        l_push_back(l, i);

    // Initialize iterator
    list_iter iter;
    l_iter_new(&iter, l);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (l_iter_next(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n");
        while (l_iter_prev(&iter, &r, &i))
            printf("C[%2d] = %2d\n", i, r);
        printf("\n\n");
    }

    for (l_iter_tostart(&iter); !l_iter_end(&iter); /**/)
    {
        l_iter_next(&iter, &r, &i);
        printf("L[%2d] = %2d\n", i, r);
    }

    printf("\n");

    for (l_iter_toend(&iter); !l_iter_start(&iter); /**/)
    {
        l_iter_prev(&iter, &r, &i);
        printf("L[%2d] = %2d\n", i, r);
    }

    l_free(l);

    return 0;
}
