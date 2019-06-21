/**
 * even_odd.c
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */
#include "cmc/deque.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

DEQUE_GENERATE(d, deque, /* static */, size_t)

int main(int argc, char const *argv[])
{
    // 1- Add odd numbers to the start of the deque and even numbers to the end.
    // 2- Iterate over the deque, starting from the end, until index is half
    //    the deque count and stop.
    // 3- Sum of all even numbers

    deque *numbers = d_new(1000);

    for (size_t i = 1; i <= 100000; i++)
    {
        i % 2 == 0 ? d_push_back(numbers, i) : d_push_front(numbers, i);
    }

    deque_iter iter;
    d_iter_init(&iter, numbers);

    size_t result, index, sum = 0;

    for (d_iter_to_end(&iter); !d_iter_start(&iter); d_iter_prev(&iter))
    {
        result = d_iter_value(&iter);
        index = d_iter_index(&iter);

        if ((double)index < (double)d_count(numbers) / 2.0)
            break;

        sum += result;
    }

    printf("Total sum: %lu\n", sum);

    assert(sum == 2500050000);

    return 0;
}
