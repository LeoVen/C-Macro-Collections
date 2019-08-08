/**
 * stack.c
 *
 * Creation Date: 11/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/stack.h"
#include <stdio.h>

STACK_GENERATE(s, stack, static, int)

int main(int argc, char const *argv[])
{
    // Initialize stack and add some elements
    stack *s = s_new(10);
    for (int i = 0; i < 10; i++)
        s_push(s, i);

    // Initialize iterator
    stack_iter iter;
    s_iter_init(&iter, s);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (!s_iter_end(&iter))
        {
            printf("C[%2d] = %2d\n", s_iter_index(&iter), s_iter_value(&iter));
            s_iter_next(&iter);
        }
        printf("\n");
        while (!s_iter_start(&iter))
        {
            printf("C[%2d] = %2d\n", s_iter_index(&iter), s_iter_value(&iter));
            s_iter_prev(&iter);
        }
        printf("\n\n");
    }

    for (s_iter_to_start(&iter); !s_iter_end(&iter); s_iter_next(&iter))
    {
        printf("S[%2d] = %2d\n", s_iter_index(&iter), s_iter_value(&iter));
    }

    printf("\n");

    for (s_iter_to_end(&iter); !s_iter_start(&iter); s_iter_prev(&iter))
    {
        printf("S[%2d] = %2d\n", s_iter_index(&iter), s_iter_value(&iter));
    }

    s_free(s);

    return 0;
}
