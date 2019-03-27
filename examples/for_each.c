#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/macro_collections.h"

// Create containers to use only in this file
COLLECTION_GENERATE(LIST, PUBLIC, l, list, static, , int)
COLLECTION_GENERATE(STACK, PUBLIC, s, stack, static, , int)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, static, , int)
COLLECTION_GENERATE(DEQUE, PUBLIC, d, deque, static, , int)

int main(int argc, char const *argv[])
{
    // Initialize all of them with an internal capacity of 100
    list *l = l_new(100);
    stack *s = s_new(100);
    queue *q = q_new(100);
    deque *d = d_new(100);

    // Add 20 elements to each of them
    for (int i = 0; i < 20; i++)
    {
        l_push_back(l, i);
        s_push(s, i);
        q_enqueue(q, i);
        d_push_back(d, i);
    }

    // Some extra elements
    s_push(s, 21);
    q_enqueue(q, 21);
    q_enqueue(q, 22);
    d_push_back(d, 21);
    d_push_back(d, 22);
    d_push_back(d, 23);

    // All FOR_EACH macros contains three available variables:
    // - var : the elements that changes with each iteration
    // - index : iteration index (first iteration is 0 and last is count - 1)
    // - iter : iterator struct
    FOR_EACH(l, list, , int, l, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    FOR_EACH(s, stack, , int, s, {
        if (index == iter.target->count - 1)
            printf("[ %d, ", var);
        else if (index == 0)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    FOR_EACH(q, queue, , int, q, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    FOR_EACH(d, deque, , int, d, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    l_free(l);
    s_free(s);
    q_free(q);
    d_free(d);

    return 0;
}
