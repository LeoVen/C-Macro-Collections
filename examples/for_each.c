#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/macro_collections.h"

// Create containers to use only in this file
COLLECTION_GENERATE(LIST, PUBLIC, l, list, static, , int)
COLLECTION_GENERATE(LINKEDLIST, PUBLIC, ll, llist, static, , int)
COLLECTION_GENERATE(STACK, PUBLIC, s, stack, static, , int)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, static, , int)
COLLECTION_GENERATE(DEQUE, PUBLIC, d, deque, static, , int)
COLLECTION_GENERATE(TREESET, PUBLIC, ts, tset, static, , int)

int intcmp(int a, int b)
{
    return a - b;
}

int main(int argc, char const *argv[])
{
    // Initialize all of them with an internal capacity of 100
    list *l = l_new(100);
    llist *ll = ll_new();
    stack *s = s_new(100);
    queue *q = q_new(100);
    deque *d = d_new(100);
    tset *ts = ts_new(intcmp);

    // Add 20 elements to each of them
    for (int i = 0; i < 20; i++)
    {
        l_push_back(l, i);
        ll_push_back(ll, i);
        s_push(s, i);
        q_enqueue(q, i);
        d_push_back(d, i);
        ts_insert(ts, i);
    }

    // Some extra elements
    s_push(s, 21);
    q_enqueue(q, 21);
    q_enqueue(q, 22);
    d_push_back(d, 21);
    d_push_back(d, 22);
    d_push_back(d, 23);
    ll_push_back(ll, 21);
    ll_push_back(ll, 22);
    ll_push_back(ll, 23);
    ll_push_back(ll, 24);
    ts_insert(ts, 21);
    ts_insert(ts, 22);
    ts_insert(ts, 23);
    ts_insert(ts, 24);
    ts_insert(ts, 25);

    // All FOR_EACH macros contains three available variables:
    // - var : the elements that changes with each iteration
    // - index : iteration index (first iteration is 0 and last is count - 1)
    // - iter : iterator struct
    FOR_EACH(l, list, int, l, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    FOR_EACH(s, stack, int, s, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    FOR_EACH(q, queue, int, q, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    FOR_EACH(d, deque, int, d, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    FOR_EACH(ll, llist, int, ll, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    FOR_EACH(ts, tset, int, ts, {
        if (index == 0)
            printf("[ %d, ", var);
        else if (index == iter.target->count - 1)
            printf("%d ]\n", var);
        else
            printf("%d, ", var);
    })

    l_free(l);
    ll_free(ll);
    s_free(s);
    q_free(q);
    d_free(d);
    ts_free(ts);

    return 0;
}
