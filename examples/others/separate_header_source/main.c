#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include "macro_collections.h"
#include "containers.h"

double rrandom()
{
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

int8_t random_int8_t(int8_t min, int8_t max)
{
    return (int8_t)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

size_t random_size_t(size_t min, size_t max)
{
    return (size_t)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

char random_alpha()
{
    const char alpha[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                          'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                          'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                          'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
                          'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                          'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                          'Y', 'Z'};
    return alpha[random_int8_t(0, 61)];
}

int main(void)
{
    srand((unsigned)time(NULL));

    int_list *list = il_new(10);

    for (int i = 0; i < 100; i++)
    {
        il_push(list, i, list->count == 0 ? list->count : i % list->count);
    }

    printf("\n[ ");

    for (size_t i = 0; i < list->count - 1; i++)
        printf("%d, ", il_get(list, i));

    printf("%d ]\n", il_get(list, list->count - 1));

    il_free(list);

    list = il_new(10);

    for (int i = 0; i < 10; i++)
    {
        // Add elements
        il_push_back(list, i);

        // Print
        if (!il_empty(list))
        {
            // Print
            printf("\n[ ");
            for (size_t i = 0; i < list->count - 1; i++)
            {
                printf("%d, ", il_get(list, i));
            }
            printf("%d ]\n", il_get(list, list->count - 1));
        }
    }

    for (int i = 0; i < 10; i++)
    {
        // Remove elements
        il_pop_back(list);

        if (!il_empty(list))
        {
            // Print
            printf("\n[ ");
            for (size_t i = 0; i < list->count - 1; i++)
            {
                printf("%d, ", il_get(list, i));
            }
            printf("%d ]\n", il_get(list, list->count - 1));
        }
    }

    il_free(list);

    list = il_new(1000);

    for (int i = 0; i < 1000; i++)
    {
        il_push_if(list, i, list->count, i % 2 == 0);
    }

    // Print all even numbers
    if (!il_empty(list))
    {
        // Print
        printf("\n[ ");
        for (size_t i = 0; i < list->count - 1; i++)
        {
            printf("%d, ", il_get(list, i));
        }
        printf("%d ]\n", il_get(list, list->count - 1));
    }

    il_free(list);

    char_stack *stack = cs_new(100);

    for (int i = 0; i < 2000; i++)
    {
        cs_push(stack, random_alpha());
    }

    size_t s = cs_count(stack);
    size_t c = cs_capacity(stack);
    printf("Stack size     : %" PRIu64 "\n", s);
    printf("Stack capacity : %" PRIu64 "\n", c);

    for (size_t i = 0; i < s; i++)
    {
        printf("%c ", cs_top(stack));
        cs_pop(stack);
    }

    printf("\nStack size: %" PRIu64 "", cs_count(stack));
    printf("\nStack capacity : %" PRIu64 "\n", cs_capacity(stack));

    cs_free(stack);

    index_queue *idxs = queue_new(100);

    size_t sum0 = 0, sum1 = 0, curr;

    printf("\nQueue size     : %" PRIu64 "\n", queue_count(idxs));
    printf("Queue capacity : %" PRIu64 "\n", queue_capacity(idxs));

    for (int i = 1; i <= 600; i++)
    {
        curr = random_size_t(0, 100);
        if (queue_enqueue(idxs, curr))
            sum0 += curr;

        if (i % 11 == 0 && queue_count(idxs) > 0)
        {
            sum0 -= queue_peek(idxs);
            queue_dequeue(idxs);
        }
    }

    printf("\nQueue size     : %" PRIu64 "\n", queue_count(idxs));
    printf("Queue capacity : %" PRIu64 "\n", queue_capacity(idxs));

    while (!queue_empty(idxs))
    {
        sum1 += queue_peek(idxs);

        queue_dequeue(idxs);
    }

    printf("\nQueue size     : %" PRIu64 "\n", queue_count(idxs));
    printf("Queue capacity : %" PRIu64 "\n", queue_capacity(idxs));

    if (sum0 == sum1)
        printf("\nElements were preserved -> %" PRIu64 " : %" PRIu64 "\n", sum0, sum1);

    queue_free(idxs);

    printf("\nUsing ForEach\n\n");

    int_list *integers = il_new(1000);

    for (size_t i = 0; i < il_capacity(integers); i++)
        il_push_back(integers, i);

    int sum = 0;
    FOR_EACH(il, int_list, integers, {
        sum += il_iter_value(&iter);
    });

    printf("\n\nSUM: %d\n", sum);

    il_free(integers);

    deque *d = dq_new(1000);

    int sum2 = 0, numbers = 0, r = 0;

    for (int i = 0; numbers < 10000; i = rand())
    {
        if (i % 2 == 0 || dq_empty(d))
        {
            if (i % 4 == 0)
            {
                dq_push_front(d, ++numbers);
            }
            else
            {
                dq_push_back(d, ++numbers);
            }
        }
        else
        {
            if (i % 3 == 0)
            {
                r = dq_front(d);
                dq_pop_front(d);
            }
            else
            {
                r = dq_back(d);
                dq_pop_back(d);
            }

            sum2 += r;
        }
    }

    // Emptying the queue
    while (!dq_empty(d))
    {
        if (rand() % 2 == 0)
        {
            r = dq_front(d);
            dq_pop_front(d);
        }
        else
        {
            r = dq_back(d);
            dq_pop_back(d);
        }

        sum2 += r;
    }

    printf("Theoretical Sum: 50005000\nTotal Sum: %d\n", sum2);

    dq_free(d);

    d = dq_new(32);

    for (int i = 1; i < 1001; i++)
        i % 2 == 0 ? dq_push_front(d, i) : dq_push_back(d, i);

    int sum3 = 0;

    FOR_EACH(dq, deque, d, {
        sum3 += dq_iter_value(&iter);
    });

    printf("Deque sum: %d\n", sum3);

    return 0;
}
